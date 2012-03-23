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
//        Vector& mons = static_cast<Vector&>( monitors->getValue() );
//        return mons;
//    }

    return MemberObject::executeOperationSimple( name, args );
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
//        memberRules.push_back( new ValueRule ( "monitors" , TypeSpec(Vector::getClassTypeSpec(), new TypeSpec(Monitor::getClassTypeSpec()) ) ) );
        
        
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
    
    /* Get the dag nodes from the model */
    std::vector<RbDagNodePtr> dagNodes = static_cast<const Model&>( model->getValue() ).getDAGNodes();
    
    /* Get the stochastic nodes in an ordered manner */
    std::vector<StochasticNode*> orderedStochasticNodes; 
    std::set<DAGNode*> visitedNodes;

    getOrderedStochasticNodes(dagNodes[0], orderedStochasticNodes, visitedNodes);
    
    /* Get the monitors */
    const std::vector<Monitor*>& theMonitors = static_cast<const Model&>( model->getValue() ).getMonitors();
    /* Open the output file and print headers */
    std::cerr << "Opening file and printing headers ..." << std::endl;
    for (size_t i=0; i<theMonitors.size(); i++) {
        // get the monitor
        if (theMonitors[i]->isTypeSpec( FileMonitor::getClassTypeSpec() ) ) {
            
            FileMonitor* theMonitor = static_cast<FileMonitor*>( theMonitors[i] );
            
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
            Distribution& dist = orderedStochasticNodes[i]->getDistribution();
            orderedStochasticNodes[i]->setValue( dist.rv().clone() );
            // we need to call keep so that the values get recalculated properly
            orderedStochasticNodes[i]->keep();
        }
        
        
        /* Monitor */
        for (size_t i=0; i<theMonitors.size(); i++) {
            theMonitors[i]->monitor(data);
        }

                
    }
    
    std::cerr << "Finished simulating data" << std::endl;
    
    std::cout << std::endl;
}
