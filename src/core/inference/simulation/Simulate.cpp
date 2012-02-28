/**
 * @file
 * This file contains the implementation of Simulate, which is used to hold
 * information about and run a simulation given a model and monitors.
 *
 * @brief Implementation of Simulate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Simulate
 * @package distributions
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "Integer.h"
#include "Simulate.h"
#include "MemberFunction.h"
#include "Model.h"
#include "FileMonitor.h"
#include "ObjectMonitor.h"
#include "Move.h"
#include "Natural.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RangeRule.h"
#include "RbUtil.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "Vector.h"
#include "VectorString.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/** Constructor passes member rules and method inits to base class */
Simulate::Simulate(void) : ConstantMemberObject(getMemberRules()), model( NULL ), monitors( NULL ) {
}

/** Copy constructor */
Simulate::Simulate(const Simulate &x) : ConstantMemberObject(x), model( x.model ), monitors( x.monitors ) {
    
}


/** Clone object */
Simulate* Simulate::clone(void) const {
    
    return new Simulate(*this);
}


/** Map calls to member methods */
const RbLanguageObject& Simulate::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
    
    if (name == "run") {
        const RbLanguageObject& argument = args[0].getVariable().getValue();
        int n = static_cast<const Natural&>( argument ).getValue();
        run(n);
        return RbNullObject::getInstance();
    }
    else if ( name == "getMonitors" ) {
        Vector& mons = static_cast<Vector&>( monitors->getValue() );
        return mons;
    }

    return MemberObject::executeOperationSimple( name, args );
}


/** Get class name of object */
const std::string& Simulate::getClassName(void) { 
    
    static std::string rbClassName = "Simulate";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Simulate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ConstantMemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Simulate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member rules */
const MemberRules& Simulate::getMemberRules(void) const {
    
    static MemberRules memberRules = ArgumentRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ValueRule ( "model"    , Model::getClassTypeSpec()    ) );
        memberRules.push_back( new ValueRule ( "monitors" , TypeSpec(Vector::getClassTypeSpec(), new TypeSpec(Monitor::getClassTypeSpec()) ) ) );
        
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get methods */
const MethodTable& Simulate::getMethods(void) const {
    
    static MethodTable methods = MethodTable();

    static bool          methodsSet = false;
    
    if (!methodsSet) {
        
        ArgumentRules* updateArgRules = new ArgumentRules();
        updateArgRules->push_back( new ValueRule( "dataElements", Natural::getClassTypeSpec()     ) );
        methods.addFunction("run", new MemberFunction( RbVoid_name, updateArgRules ) );
        
        // get Monitors
        ArgumentRules* getMonitorsRules = new ArgumentRules();
        methods.addFunction("getMonitors", new MemberFunction( TypeSpec(Vector::getClassTypeSpec(), new TypeSpec(Monitor::getClassTypeSpec()) ), getMonitorsRules) );
        
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/** Allow only constant member variables */
void Simulate::setMemberVariable(const std::string& name, Variable* var) {
    
    // we need to change the DAG nodes to which the moves are pointing to
    // when the moves where created they pointed to DAG nodes in the workspace
    // but the model created clones of theses nodes.
    // Hence we need to set the DAG nodes of the moves to these clones.
    if ( name == "monitors" ) {
        
        //        Vector* monitors = static_cast<Vector*>(var->getValue().convertTo(TypeSpec(Vector::getClassName(), NULL, new TypeSpec(FileMonitor::getClassTypeSpec()) ) ) );
        // The parser should have made sure that we get a vector of moves, even if it is only a single move.
        Vector& monitors = static_cast<Vector&>( var->getValue() );
        // Our new vector of moves
        Vector* myMonitors = new Vector( Monitor::getClassTypeSpec() ); 
        for (size_t i=0; i<monitors.size(); i++) {
            // get the monitor #i
            Monitor& theMonitor = static_cast<Monitor&>( monitors.getElement(i) );
            
            // clone the move and replace the node
            Monitor* newMonitor = theMonitor.clone();
            
            // add this monitor to the list of my monitors
            myMonitors->push_back(newMonitor );
            
        }
        
        this->monitors = RbVariablePtr( new Variable ( new ConstantNode( myMonitors ) ) );
        
        if (model != NULL) {
            // get the DAG nodes
            const Model& theModel = dynamic_cast<Model&>( model->getValue() );
            for (size_t i=0; i<myMonitors->size(); i++) {
                // get the move #i
                Monitor& theMonitor = static_cast<Monitor&>( myMonitors->getElement(i) );
                
                // get the DAG node for this move
                std::vector<RbVariablePtr> &theOldNodes = theMonitor.getDagNodes();
                
                // convert the old nodes from Stochastic nodes to DAGNode
                std::vector<DAGNode*> oldNodes;
                for (std::vector<RbVariablePtr>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
                    oldNodes.push_back( (*it)->getDagNode() );
                }
                
                // test whether this node is already a cloned one
                if (!theModel.areDagNodesCloned(oldNodes) ) {
                    // get the DAG node which corresponds in the model to the cloned original node
                    std::vector<DAGNode*> theNewNodes = theModel.getClonedDagNodes(oldNodes);
                    
                    // convert the new nodes from DAGNode to Stochastic Nodes
                    std::vector<VariableNode*> newNodes;
                    for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
                        newNodes.push_back( static_cast<VariableNode*>( *it ) );
                    }
                    theMonitor.replaceDagNodes(newNodes);
                }
                
            }
        }
        
    } 
    else if ( name == "model" ) {
        model = var;
        
        // update the monitors
        if (monitors != NULL) {
            // get the DAG nodes
            Vector& myMonitors = static_cast<Vector&>( monitors->getValue() );
            const Model& theModel = dynamic_cast<Model&>( model->getValue() );
            for (size_t i=0; i<myMonitors.size(); i++) {
                // get the move #i
                Monitor& theMonitor = static_cast<Monitor&>( myMonitors.getElement(i) );
                
                // get the DAG node for this move
                std::vector<RbVariablePtr> &theOldNodes = theMonitor.getDagNodes();
                
                // convert the old nodes from Stochastic nodes to DAGNode
                std::vector<DAGNode*> oldNodes;
                for (std::vector<RbVariablePtr>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
                    oldNodes.push_back( (*it)->getDagNode() );
                }
                
                // test whether this node is already a cloned one
                if (!theModel.areDagNodesCloned(oldNodes) ) {
                    // get the DAG node which corresponds in the model to the cloned original node
                    std::vector<DAGNode*> theNewNodes = theModel.getClonedDagNodes(oldNodes);
                    
                    // convert the new nodes from DAGNode to Stochastic Nodes
                    std::vector<VariableNode*> newNodes;
                    for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
                        newNodes.push_back( static_cast<VariableNode*>( *it ) );
                    }
                    theMonitor.replaceDagNodes(newNodes);
                }
            }
        }
    }
    else {
        ConstantMemberObject::setMemberVariable(name, var);
        
    }
}

/** Creates a vector of stochastic nodes, starting from the source nodes to the sink nodes */
void Simulate::getOrderedStochasticNodes(DAGNode* dagNode,  std::vector<StochasticNode*>& orderedStochasticNodes, std::set<DAGNode*>& visitedNodes) {
    if (visitedNodes.find(dagNode) != visitedNodes.end()) { //The node has been visited before
        //we do nothing
        return;
    }
    if (dagNode->getTypeSpec() ==  ConstantNode::getClassTypeSpec()) { //if the node is constant: no parents to visit
        std::set<VariableNode*> children = dagNode->getChildren() ;
        visitedNodes.insert(dagNode);
        std::set<VariableNode*>::iterator it;
        for ( it = children.begin() ; it != children.end(); it++ )
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
    }
    else if (dagNode->getTypeSpec() ==  StochasticNode::getClassTypeSpec() || dagNode->getTypeSpec() ==  DeterministicNode::getClassTypeSpec()) { //if the node is stochastic or deterministic
        //First I have to visit my parents
        const std::set<DAGNode*>& parents = dagNode->getParents() ;
        std::set<DAGNode*>::iterator it;
        for ( it=parents.begin() ; it != parents.end(); it++ ) 
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
        
        //Then I can add myself to the nodes visited, and to the ordered vector of stochastic nodes
        visitedNodes.insert(dagNode);
        if (dagNode->getTypeSpec() ==  StochasticNode::getClassTypeSpec()) //if the node is stochastic
            orderedStochasticNodes.push_back(static_cast<StochasticNode*>( dagNode ) );
        
        //Finally I will visit my children
        std::set<VariableNode*> children = dagNode->getChildren() ;
        std::set<VariableNode*>::iterator it2;
        for ( it2 = children.begin() ; it2 != children.end(); it2++ ) 
            getOrderedStochasticNodes(*it2, orderedStochasticNodes, visitedNodes);
        }
    return; 
}


/** Perform the simulation */
void Simulate::run(size_t ndata) {
    
    std::cerr << "Initializing the simulation ..." << std::endl;
    
    /* Get the dag nodes from the model */
    std::vector<RbDagNodePtr> dagNodes = static_cast<Model&>( model->getValue() ).getDAGNodes();
    
    /* Get the stochastic nodes in an ordered manner */
    std::vector<StochasticNode*> orderedStochasticNodes; 
    std::set<DAGNode*> visitedNodes;

    getOrderedStochasticNodes(dagNodes[0], orderedStochasticNodes, visitedNodes);
    
    /* Get the monitors */
    Vector& theMonitors = static_cast<Vector&>( monitors->getValue() );
    /* Open the output file and print headers */
    std::cerr << "Opening file and printing headers ..." << std::endl;
    for (size_t i=0; i<theMonitors.size(); i++) {
        // get the monitor
        if (theMonitors.getElement(i).isTypeSpec( FileMonitor::getClassTypeSpec() ) ) {
            
            FileMonitor& theMonitor = static_cast<FileMonitor&>( theMonitors.getElement(i) );
            
            // open the file stream for the monitor
            theMonitor.openStream();
            
            // print the header information
            theMonitor.printHeader();
        }
        
    }

        
    /* Run the chain */
    std::cerr << "Running the simulation ..." << std::endl;
    
    std::cout << std::endl;    
    for (unsigned int data=1; data<=ndata; data++) {

        //Random draws from the ordered stochastic nodes
        for (size_t i = 0; i < orderedStochasticNodes.size() ; i++) {
            Distribution& dist = orderedStochasticNodes[i]->getDistribution();
            orderedStochasticNodes[i]->setValue( dist.rv().clone() );
            // we need to call keep so that the values get recalculated properly
            orderedStochasticNodes[i]->keep();
        }
        
        
        /* Monitor */
        for (size_t i=0; i<theMonitors.size(); i++) {
            static_cast<Monitor&>( theMonitors.getElement(i) ).monitor(data);
        }

                
    }
    
    std::cerr << "Finished simulating data" << std::endl;
    
    std::cout << std::endl;
}
