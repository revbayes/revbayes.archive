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
#include "Integer.h"
#include "Simulate.h"
#include "MemberFunction.h"
#include "Model.h"
#include "FileMonitor.h"
#include "ObjectMonitor.h"
#include "Move.h"
#include "Natural.h"
#include "ParserDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RangeRule.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RbVector.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/** Constructor passes member rules and method inits to base class */
Simulate::Simulate(void) : MemberObject(getMemberRules()), model( NULL ) {
}

/** Copy constructor */
Simulate::Simulate(const Simulate &x) : MemberObject(x), model( x.model ) {
    
    if ( model != NULL ) {
        extractDagNodesFromModel( static_cast<const Model&>( model->getValue() ) );
    }
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
//    else if ( name == "getMonitors" ) {
//        return monitors;
//    }

    return MemberObject::executeOperationSimple( name, args );
}


/**
 * Clone the model and extract the DAG nodes, moves and monitors.
 *
 * First we clone the entire DAG, then we clone the moves and the monitors and set the new nodes appropriately.
 */
void Simulate::extractDagNodesFromModel(const Model& source) {
    
    const std::set<const DAGNode*> sourceNodes = source.getSourceNodes();
    
    // if the var is not NULL we pull the DAG from it
    std::map<const DAGNode*, RbDagNodePtr> nodesMap;
    
    for (std::set<const DAGNode*>::const_iterator it = sourceNodes.begin(); it != sourceNodes.end(); it++) {
        const DAGNode* theSourceNode = *it;
        
        if (theSourceNode == NULL)
            throw RbException("Cannot instantiate a model with a NULL DAG node.");
        
        theSourceNode->cloneDAG(nodesMap);
    }
    
    /* insert new nodes in dagNodes member frame and direct access vector */
    std::map<const DAGNode*, RbDagNodePtr>::iterator i = nodesMap.begin();
    
    while ( i != nodesMap.end() ) {
        
        DAGNode* theNewNode = (*i).second;
        
        // increment the iterator;
        ++i;
        
        // insert in direct access vector
        dagNodes.push_back( theNewNode );
        
    }
    
    ////////////////////////////
    // now we clone the monitors
    
    // we first empty our current vector of monitors
    monitors.clear();
    
    // first we create a map from the old moves to the new ones
    std::map<Monitor*, Monitor*> monitorMap;
    
    // next, we iterate over all dag nodes to collect the move
//    for (std::map<const DAGNode*, RbDagNodePtr>::iterator i = nodesMap.begin(); i != nodesMap.end(); ++i) {
//        // get all moves for this node
//        const DAGNode* orgNode = i->first;
//        
//        const std::vector<Monitor*>& orgMonitors = orgNode->getMonitors();
//        
//        for (std::vector<Monitor*>::const_iterator j = orgMonitors.begin(); j != orgMonitors.end(); ++j) {
//            // check if we already have this move in our list
//            std::map<Monitor*,Monitor*>::iterator k = monitorMap.find( *j );
//            
//            Monitor* clonedMonitor = NULL;
//            // create a new move if necessary
//            if ( k == monitorMap.end() ) {
//                clonedMonitor = (*j)->clone();
//                
//                // add the new monitor to the monitors map
//                monitorMap.insert( std::pair<Monitor*, Monitor*>(*j, clonedMonitor) );
//                
//                // add the new monitor to our monitors vector
//                monitors.push_back( clonedMonitor );
//            }
//            else {
//                clonedMonitor = k->second;
//            }
//            
//            // add the clone DAG node to the cloned monitor
//            clonedMonitor->addDagNode( i->second );
//            
//            // add the cloned monitor to the cloned DAG node
//            i->second->addMonitor( clonedMonitor );
//            
//        }
//    }
    
}


/** Get class name of object */
const std::string& Simulate::getClassName(void) { 
    
    static std::string rbClassName = "Simulate";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Simulate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
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
        methods.addFunction("getMonitors", new MemberFunction( RbVector<Monitor>::getClassTypeSpec(), getMonitorsRules) );
        
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/** Allow only constant member variables */
void Simulate::setMemberVariable(const std::string& name, const Variable* var) {
    
    if ( name == "model" ) {
        model = var;
        extractDagNodesFromModel( static_cast<const Model&>( model->getValue() ) );
    }
    else {
        MemberObject::setMemberVariable(name, var);
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
        std::set<DAGNode*>::const_iterator it;
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
        
    /* Get the stochastic nodes in an ordered manner */
    std::vector<StochasticNode*> orderedStochasticNodes; 
    std::set<DAGNode*> visitedNodes;

    getOrderedStochasticNodes(dagNodes[0], orderedStochasticNodes, visitedNodes);
    
    /* Open the output file and print headers */
    std::cerr << "Opening file and printing headers ..." << std::endl;
    for (size_t i=0; i<monitors.size(); i++) {
        // get the monitor
        if ( typeid(*monitors[i]) == typeid(FileMonitor) ) {
            
            FileMonitor* theMonitor = static_cast<FileMonitor*>( monitors[i] );
            
            // open the file stream for the monitor
            theMonitor->openStream();
            
            // print the header information
            theMonitor->printHeader();
        }
        
    }

        
    /* Run the chain */
    std::cerr << "Running the simulation ..." << std::endl;
    
    std::cout << std::endl;    
    for (unsigned int data=1; data<=ndata; data++) {

        //Random draws from the ordered stochastic nodes
        for (size_t i = 0; i < orderedStochasticNodes.size() ; i++) {
            ParserDistribution& dist = orderedStochasticNodes[i]->getDistribution();
            dist.rv();
            orderedStochasticNodes[i]->setValue( dist.getTemplateRandomVariable().clone() );
            // we need to call keep so that the values get recalculated properly
            orderedStochasticNodes[i]->keep();
        }
        
        
        /* Monitor */
        for (size_t i=0; i<monitors.size(); i++) {
            monitors[i]->monitor(data);
        }

                
    }
    
    std::cerr << "Finished simulating data" << std::endl;
    
    std::cout << std::endl;
}
