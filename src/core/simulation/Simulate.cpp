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
#include "MoveSchedule.h"
#include "Natural.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
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
Simulate::Simulate(void) : ConstantMemberObject(getMemberRules()) {
}

/** Copy constructor */
Simulate::Simulate(const Simulate &x) : ConstantMemberObject(x) {
    
}


/** Clone object */
Simulate* Simulate::clone(void) const {
    
    return new Simulate(*this);
}


/** Map calls to member methods */
RbPtr<RbLanguageObject> Simulate::executeOperationSimple(const std::string& name, const RbPtr<Environment>& args) {
    
    if (name == "run") {
        const RbPtr<const RbLanguageObject>& argument = (*args)[0]->getValue();
        int n = static_cast<const Natural*>( (const RbObject*)argument )->getValue();
        run(n);
        return RbPtr<RbLanguageObject>::getNullPtr();
    }
    if (name == "getValues") {
        const RbPtr<const RbLanguageObject>& argument = (*args)[0]->getValue();
        RbString n = static_cast<const RbString*>( (const RbObject*)argument )->getValue();
        const RbPtr<Vector>& vec = getValues(n);
        return RbPtr<RbLanguageObject>(vec);
    }

    return MemberObject::executeOperationSimple( name, args );
}


/** Get class vector describing type of object */
const VectorString& Simulate::getClass(void) const {
    
    static VectorString rbClass = VectorString(Simulate_name) + ConstantMemberObject::getClass();
    return rbClass;
}


/** Get member rules */
RbPtr<const MemberRules> Simulate::getMemberRules(void) const {
    
    static RbPtr<MemberRules> memberRules( new ArgumentRules() );
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule ( "model"    , Model_name    ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule ( "fileMonitors" , TypeSpec(Vector_name, RbPtr<TypeSpec>( new TypeSpec(FileMonitor_name) ) ) ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule ( "objectMonitors" , TypeSpec(Vector_name, RbPtr<TypeSpec>( new TypeSpec(ObjectMonitor_name) ) ) ) ) );
        
        rulesSet = true;
    }
    
    return RbPtr<const ArgumentRules>( memberRules );
}


/** Get methods */
RbPtr<const MethodTable> Simulate::getMethods(void) const {
    
    static RbPtr<MethodTable> methods( new MethodTable() );
    static RbPtr<ArgumentRules> updateArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> getValuesArgRules( new ArgumentRules() );

    static bool          methodsSet = false;
    
    if (!methodsSet) {
        
        updateArgRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "dataElements", Natural_name     ) ) );
        methods->addFunction("run", RbPtr<RbFunction>( new MemberFunction( RbVoid_name, updateArgRules ) ) );
        getValuesArgRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "varName", RbString_name     ) ) );
        methods->addFunction("getValues", RbPtr<RbFunction>( new MemberFunction( RbVoid_name, getValuesArgRules ) ) );
        methods->setParentTable( RbPtr<const FunctionTable>( MemberObject::getMethods() ) );
        methodsSet = true;
    }
    
    return RbPtr<const MethodTable>( methods );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Simulate::getTypeSpec(void) const {
    return typeSpec;
}


/** Allow only constant member variables */
void Simulate::setMemberVariable(const std::string& name, RbPtr<Variable> var) {
    
        if ( name == "fileMonitors" ) {
        // get the DAG nodes
        const RbPtr<Model> theModel( static_cast<Model*>( (RbObject*)getMemberValue("model") ) );
        
        RbPtr<Vector> monitors( static_cast<Vector*>(var->getValue()->convertTo(TypeSpec(Vector_name, RbPtr<TypeSpec>( new TypeSpec(FileMonitor_name) ) ) ) ) );
            for (size_t i=0; i<monitors->size(); i++) {
                // get the monitor #i
                RbPtr<FileMonitor> theMonitor( static_cast<FileMonitor*>( (RbObject*)monitors->getElement(i) ) );
                
                // get the DAG node for this monitor
                std::vector<RbPtr<VariableNode> > &theOldNodes = theMonitor->getDagNodes();
                
                // convert the old nodes from Stochastic nodes to DAGNode
                std::vector<RbPtr<DAGNode> > oldNodes;
                for (std::vector<RbPtr<VariableNode> >::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
                    oldNodes.push_back( RbPtr<DAGNode>( (VariableNode*)(*it) ) );
                }
                // get the DAG node which corresponds in the model to the cloned original node
                std::vector<RbPtr<DAGNode> > theNewNodes = theModel->getClonedDagNodes(oldNodes);
                
                // clone the move and replace the node
                RbPtr<FileMonitor> newMonitor( theMonitor->clone() );
                // convert the new nodes from DAGNode to Stochastic Nodes
                std::vector<RbPtr<VariableNode> > newNodes;
                for (std::vector<RbPtr<DAGNode> >::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
                    newNodes.push_back( RbPtr<VariableNode>( static_cast<VariableNode*>( (DAGNode*)(*it) ) ) );
                }
                newMonitor->replaceDagNodes(newNodes);
                
                monitors->setElement(i, RbPtr<RbLanguageObject>( newMonitor ) );
            
        }
        
        setMemberDagNode(name, RbPtr<DAGNode>( new ConstantNode(RbPtr<RbLanguageObject>( monitors ) ) ) );
    }
        else if ( name == "objectMonitors" ) {
            // get the DAG nodes
            const RbPtr<Model> theModel( static_cast<Model*>( (RbObject*)getMemberValue("model") ) );
            
            RbPtr<Vector> monitors( static_cast<Vector*>(var->getValue()->convertTo(TypeSpec(Vector_name, RbPtr<TypeSpec>( new TypeSpec(ObjectMonitor_name) ) ) ) ) );
            for (size_t i=0; i<monitors->size(); i++) {
                // get the monitor #i
                RbPtr<ObjectMonitor> theMonitor( static_cast<ObjectMonitor*>( (RbObject*)monitors->getElement(i) ) );
                
                // get the DAG node for this monitor
                std::vector<RbPtr<VariableNode> > &theOldNodes = theMonitor->getDagNodes();
                
                // convert the old nodes from Stochastic nodes to DAGNode
                std::vector<RbPtr<DAGNode> > oldNodes;
                for (std::vector<RbPtr<VariableNode> >::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
                    oldNodes.push_back( RbPtr<DAGNode>( (VariableNode*)(*it) ) );
                }
                // get the DAG node which corresponds in the model to the cloned original node
                std::vector<RbPtr<DAGNode> > theNewNodes = theModel->getClonedDagNodes(oldNodes);
                
                // clone the move and replace the node
                RbPtr<ObjectMonitor> newMonitor( theMonitor->clone() );
                // convert the new nodes from DAGNode to Stochastic Nodes
                std::vector<RbPtr<VariableNode> > newNodes;
                for (std::vector<RbPtr<DAGNode> >::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
                    newNodes.push_back( RbPtr<VariableNode>( static_cast<VariableNode*>( (DAGNode*)(*it) ) ) );
                }
                newMonitor->replaceDagNodes(newNodes);
                
                monitors->setElement(i, RbPtr<RbLanguageObject>( newMonitor ) );
                
            }
            
            setMemberDagNode(name, RbPtr<DAGNode>( new ConstantNode(RbPtr<RbLanguageObject>( monitors ) ) ) );
        }
    else {
        ConstantMemberObject::setMemberVariable(name, var);
    }
}

/** Creates a vector of stochastic nodes, starting from the source nodes to the sink nodes */
void Simulate::getOrderedStochasticNodes(const RbPtr<DAGNode>& dagNode,  std::vector<RbPtr<StochasticNode> >& orderedStochasticNodes, std::set<RbPtr<DAGNode> >& visitedNodes) {
    if (visitedNodes.find(dagNode) != visitedNodes.end()) { //The node has been visited before
        //we do nothing
        return;
    }
    if (dagNode->getTypeSpec() ==  ConstantNode_name) { //if the node is constant: no parents to visit
        std::set<VariableNode*> children = dagNode->getChildren() ;
        visitedNodes.insert(dagNode);
        std::set<VariableNode*>::iterator it;
        for ( it = children.begin() ; it != children.end(); it++ )
            getOrderedStochasticNodes(RbPtr<DAGNode >(*it), orderedStochasticNodes, visitedNodes);
    }
    else if (dagNode->getTypeSpec() ==  StochasticNode_name || dagNode->getTypeSpec() ==  DeterministicNode_name) { //if the node is stochastic or deterministic
        //First I have to visit my parents
        std::set<RbPtr<DAGNode> > parents = dagNode->getParents() ;
        std::set<RbPtr<DAGNode> >::iterator it;
        for ( it=parents.begin() ; it != parents.end(); it++ ) 
            getOrderedStochasticNodes(RbPtr<DAGNode >(*it), orderedStochasticNodes, visitedNodes);
        
        //Then I can add myself to the nodes visited, and to the ordered vector of stochastic nodes
        visitedNodes.insert(dagNode);
        if (dagNode->getTypeSpec() ==  StochasticNode_name) //if the node is stochastic
            orderedStochasticNodes.push_back(RbPtr<StochasticNode>(static_cast<StochasticNode*>( (DAGNode*)dagNode ) ) );
        
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
    std::vector<RbPtr<DAGNode> >& dagNodes = (static_cast<Model*>( (RbObject*)getMemberValue("model") ) )->getDAGNodes();
    
    /* Get the stochastic nodes in an ordered manner */
    std::vector<RbPtr<StochasticNode> > orderedStochasticNodes; 
    std::set<RbPtr<DAGNode> > visitedNodes;

    getOrderedStochasticNodes(dagNodes[0], orderedStochasticNodes, visitedNodes);

    /* Get the monitors */
    RbPtr<Vector> fileMonitors( static_cast<Vector*>( (RbObject*)getMemberDagNode( "fileMonitors" )->getValue() ) );
    RbPtr<Vector> objectMonitors( static_cast<Vector*>( (RbObject*)getMemberDagNode( "objectMonitors" )->getValue() ) );

    
    /* Get the chain settings */
    std::cerr << "Getting the random generator ..." << std::endl;
    
    RbPtr<RandomNumberGenerator> rng        = GLOBAL_RNG;
    
    /* Open the output file and print headers */
    std::cerr << "Opening file and printing headers ..." << std::endl;
    for (size_t i=0; i<fileMonitors->size(); i++) {
        // get the monitor
        RbPtr<FileMonitor> theMonitor( static_cast<FileMonitor*>( (RbObject*)fileMonitors->getElement(i) ) );
        
        // open the file stream for the monitor
        theMonitor->openStream();
        
        // print the header information
        theMonitor->printHeader();
    }

        
    /* Run the chain */
    std::cerr << "Running the simulation ..." << std::endl;
    
    std::cout << std::endl;    
    for (unsigned int data=1; data<=ndata; data++) {

        //Random draws from the ordered stochastic nodes
        for (size_t i = 0; i < orderedStochasticNodes.size() ; i++) {
            std::set<RbPtr<StochasticNode> > affectedNodes;
            RbPtr<Distribution> dist = orderedStochasticNodes[i]->getDistribution();
            orderedStochasticNodes[i]->setValue( dist->rv(), affectedNodes );
            // we need to call keep so that the values get recalculated properly
            orderedStochasticNodes[i]->keep();
            orderedStochasticNodes[i]->keepAffected();
        }
        
        /* Monitor */
        for (size_t i=0; i<fileMonitors->size(); i++) {
            static_cast<FileMonitor*>( (RbObject*)fileMonitors->getElement(i) )->monitor(data);
        }
        for (size_t i=0; i<objectMonitors->size(); i++) {
            static_cast<ObjectMonitor*>( (RbObject*)objectMonitors->getElement(i) )->monitor(data);
        }

                
    }
    
    std::cerr << "Finished simulating data" << std::endl;
    
    std::cout << std::endl;
}

/** Get the values for variable with name varName */
RbPtr<Vector> Simulate::getValues(RbString varName) {
    RbPtr<Vector> objectMonitors( static_cast<Vector*>( (RbObject*)getMemberDagNode( "objectMonitors" )->getValue() ) );
    if (objectMonitors->size() == 0) {
        std::cerr << "Error: No objectMonitor is included in the simulate object. We therefore cannot get the values of variable "<< varName <<std::endl;
        return RbPtr<Vector>::getNullPtr();
    }
    for (size_t i=0; i<objectMonitors->size(); i++) {
        if (static_cast<ObjectMonitor*>( (RbLanguageObject*)(*objectMonitors)[i] )->monitorsVariable(varName) ) {
            const RbPtr<Vector>& toReturn = static_cast<ObjectMonitor*>( (RbLanguageObject*)(*objectMonitors)[i] )->getValues(varName) ;
            return (toReturn); 
        }
    }
    std::cerr << "Error: variable "<< varName << " has not been monitored." <<std::endl;
    return RbPtr<Vector>::getNullPtr();
}