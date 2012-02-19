/**
 * @file
 * This file contains the implementation of Simulate, which is used to hold
 * information about and run a simulation given a model and monitors.
 *
 * @brief Implementation of Simulate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Simulate
 * @package distributions
 *
 * $Id: Simulate.cpp 211 2009-12-14 14:31 boussau $
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


// Definition of the static type spec member
const TypeSpec Simulate::typeSpec(Simulate_name);

/** Constructor passes member rules and method inits to base class */
Simulate::Simulate(void) : ConstantMemberObject(getMemberRules()), model( TypeSpec(Model_name) ), monitors( TypeSpec( Vector_name ) ) {
}

/** Copy constructor */
Simulate::Simulate(const Simulate &x) : ConstantMemberObject(x), model( x.model ), monitors( x.monitors ) {
    
}


/** Clone object */
Simulate* Simulate::clone(void) const {
    
    return new Simulate(*this);
}


/** Map calls to member methods */
const RbLanguageObject& Simulate::executeOperationSimple(const std::string& name, Environment& args) {
    
    if (name == "run") {
        RbLanguageObject& argument = args[0].getValue();
        int n = static_cast<Natural&>( argument ).getValue();
        run(n);
        return RbNullObject::getInstance();
    }
    if (name == "getValues") {
        RbLanguageObject& argument = args[0].getValue();
        const RbString& n = static_cast<RbString&>( argument ).getValue();
        Vector* vec = getValues(n);
        return *vec;
    }

    return MemberObject::executeOperationSimple( name, args );
}


/** Get class vector describing type of object */
const VectorString& Simulate::getClass(void) const {
    
    static VectorString rbClass = VectorString(Simulate_name) + ConstantMemberObject::getClass();
    return rbClass;
}


/** Get member rules */
const MemberRules& Simulate::getMemberRules(void) const {
    
    static MemberRules memberRules = ArgumentRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ValueRule ( "model"     , Model_name    ) );
        memberRules.push_back( new ValueRule ( "monitors"  , TypeSpec(Vector_name, new TypeSpec(Monitor_name) ) ) );
//        memberRules.push_back( new ValueRule ( "objectMonitors", TypeSpec(Vector_name, new TypeSpec(ObjectMonitor_name) ) ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get methods */
const MethodTable& Simulate::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* updateArgRules = new ArgumentRules();
    static ArgumentRules* getValuesArgRules = new ArgumentRules();

    static bool          methodsSet = false;
    
    if (!methodsSet) {
        
        updateArgRules->push_back( new ValueRule( "dataElements", Natural_name     ) );
        methods.addFunction("run", new MemberFunction( RbVoid_name, updateArgRules ) );
        getValuesArgRules->push_back( new ValueRule( "varName", RbString_name     ) );
        methods.addFunction("getValues", new MemberFunction( RbVoid_name, getValuesArgRules ) );
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Simulate::getTypeSpec(void) const {
    return typeSpec;
}


/** Allow only constant member variables */
void Simulate::setMemberDagNode(const std::string& name, DAGNode* var) {
    
        if ( name == "fileMonitors" ) {
        // get the DAG nodes
        const Model& theModel = static_cast<Model&>( model.getValue() );
        
        Vector* monitors = static_cast<Vector*>(var->getValue().convertTo(TypeSpec(Vector_name, new TypeSpec(FileMonitor_name) ) ) );
            for (size_t i=0; i<monitors->size(); i++) {
                // get the monitor #i
                FileMonitor& theMonitor = static_cast<FileMonitor&>( monitors->getElement(i) );
                
                // get the DAG node for this monitor
                std::vector<RbDagNodePtr> &theOldNodes = theMonitor.getDagNodes();
                
                // convert the old nodes from Stochastic nodes to DAGNode
                std::vector<DAGNode*> oldNodes;
                for (std::vector<RbDagNodePtr>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
                    oldNodes.push_back( *it );
                }
                // get the DAG node which corresponds in the model to the cloned original node
                std::vector<DAGNode*> theNewNodes = theModel.getClonedDagNodes(oldNodes);
                
                // clone the move and replace the node
                FileMonitor* newMonitor = theMonitor.clone();
                // convert the new nodes from DAGNode to Stochastic Nodes
                std::vector<VariableNode*> newNodes;
                for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
                    newNodes.push_back( static_cast<VariableNode*>( *it ) );
                }
                newMonitor->replaceDagNodes(newNodes);
                
                monitors->setElement(i, newMonitor );
            
        }
        
        setMemberDagNode(name, new ConstantNode(monitors) );
    }
        else if ( name == "objectMonitors" ) {
            // get the DAG nodes
            const Model& theModel = static_cast<Model&>( model.getValue() );
            
            Vector* monitors = static_cast<Vector*>(var->getValue().convertTo(TypeSpec(Vector_name, new TypeSpec(ObjectMonitor_name) ) ) );
            for (size_t i=0; i<monitors->size(); i++) {
                // get the monitor #i
                ObjectMonitor& theMonitor = static_cast<ObjectMonitor&>( monitors->getElement(i) );
                
                // get the DAG node for this monitor
                std::vector<RbDagNodePtr> &theOldNodes = theMonitor.getDagNodes();
                
                // convert the old nodes from Stochastic nodes to DAGNode
                std::vector<DAGNode*> oldNodes;
                for (std::vector<RbDagNodePtr>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
                    oldNodes.push_back( *it );
                }
                // get the DAG node which corresponds in the model to the cloned original node
                std::vector<DAGNode*> theNewNodes = theModel.getClonedDagNodes(oldNodes);
                
                // clone the move and replace the node
                ObjectMonitor* newMonitor = theMonitor.clone();
                // convert the new nodes from DAGNode to Stochastic Nodes
                std::vector<VariableNode*> newNodes;
                for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
                    newNodes.push_back( static_cast<VariableNode*>( *it ) );
                }
                newMonitor->replaceDagNodes(newNodes);
                
                monitors->setElement(i, newMonitor );
                
            }
            
            setMemberDagNode(name, new ConstantNode( monitors ) );
        }
    else {
        ConstantMemberObject::setMemberDagNode(name, var);
    }
}

/** Creates a vector of stochastic nodes, starting from the source nodes to the sink nodes */
void Simulate::getOrderedStochasticNodes(DAGNode* dagNode,  std::vector<StochasticNode*>& orderedStochasticNodes, std::set<DAGNode*>& visitedNodes) {
    if (visitedNodes.find(dagNode) != visitedNodes.end()) { //The node has been visited before
        //we do nothing
        return;
    }
    if (dagNode->getTypeSpec() ==  ConstantNode_name) { //if the node is constant: no parents to visit
        std::set<VariableNode*> children = dagNode->getChildren() ;
        visitedNodes.insert(dagNode);
        std::set<VariableNode*>::iterator it;
        for ( it = children.begin() ; it != children.end(); it++ )
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
    }
    else if (dagNode->getTypeSpec() ==  StochasticNode_name || dagNode->getTypeSpec() ==  DeterministicNode_name) { //if the node is stochastic or deterministic
        //First I have to visit my parents
        const std::set<RbDagNodePtr >& parents = dagNode->getParents() ;
        std::set<RbDagNodePtr >::iterator it;
        for ( it=parents.begin() ; it != parents.end(); it++ ) 
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
        
        //Then I can add myself to the nodes visited, and to the ordered vector of stochastic nodes
        visitedNodes.insert(dagNode);
        if (dagNode->getTypeSpec() ==  StochasticNode_name) //if the node is stochastic
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
    std::vector<RbDagNodePtr > dagNodes = static_cast<Model&>( model.getValue() ).getDAGNodes();
    
    /* Get the stochastic nodes in an ordered manner */
    std::vector<StochasticNode*> orderedStochasticNodes; 
    std::set<DAGNode*> visitedNodes;

    getOrderedStochasticNodes(dagNodes[0], orderedStochasticNodes, visitedNodes);

    /* Get the monitors */
//    Vector& fileMonitors    = static_cast<Vector&>( getMemberDagNode( "fileMonitors" )->getValue() );
    Vector& m  = static_cast<Vector&>( monitors.getValue() );

    
    /* Get the chain settings */
    std::cerr << "Getting the random generator ..." << std::endl;
    
    /* Open the output file and print headers */
    std::cerr << "Opening file and printing headers ..." << std::endl;
    for (size_t i=0; i<m.size(); i++) {
        // get the monitor
        FileMonitor& theMonitor = static_cast<FileMonitor&>( m.getElement(i) );
        
        // open the file stream for the monitor
        theMonitor.openStream();
        
        // print the header information
        theMonitor.printHeader();
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
//        for (size_t i=0; i<fileMonitors.size(); i++) {
//            static_cast<FileMonitor&>( fileMonitors.getElement(i) ).monitor(data);
//        }
//        for (size_t i=0; i<objectMonitors.size(); i++) {
//            static_cast<ObjectMonitor&>( objectMonitors.getElement(i) ).monitor(data);
//        }

                
    }
    
    std::cerr << "Finished simulating data" << std::endl;
    
    std::cout << std::endl;
}

/** Get the values for variable with name varName */
Vector* Simulate::getValues(RbString varName) {
    Vector& objectMonitors = static_cast<Vector&>( monitors.getValue() );
    if (objectMonitors.size() == 0) {
        std::cerr << "Error: No objectMonitor is included in the simulate object. We therefore cannot get the values of variable "<< varName <<std::endl;
        return NULL;
    }
    for (size_t i=0; i<objectMonitors.size(); i++) {
        if (static_cast<ObjectMonitor&>( objectMonitors[i] ).monitorsVariable(varName) ) {
            Vector* toReturn = static_cast<ObjectMonitor&>( objectMonitors[i] ).getValues(varName) ;
            return (toReturn); 
        }
    }
    std::cerr << "Error: variable "<< varName << " has not been monitored." <<std::endl;
    return NULL;
}