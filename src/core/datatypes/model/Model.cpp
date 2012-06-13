/**
 * @file
 * This file contains the implementation of Model, which is used to hold
 * DAGs copied from the working environment.
 *
 * @brief Implementation of Model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ConstructorFunction.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "DeterministicInferenceNode.h"
#include "Distribution.h"
#include "Ellipsis.h"
#include "InferenceDagNode.h"
#include "Distribution.h"
#include "InferenceFunction.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "RlDistribution.h"
#include "ParserMove.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "StochasticInferenceNode.h"
#include "VariableInferenceNode.h"
#include "UserInterface.h"

#include <algorithm>
#include <sstream>


/** Default constructor for a Model object. */
Model::Model( void ) : MemberObject( getMemberRules() ) {
}



/** Copy constructor */
Model::Model( const Model& x ) : MemberObject( x ) {

    /* Make copy of DAG by pulling from first node in x */
    if ( x.sourceNodes.size() > 0 ) {
        for (std::set<RbPtr<const DAGNode> >::const_iterator it = x.sourceNodes.begin(); it != x.sourceNodes.end(); ++it) {
            createModelFromDagNode( *it );
            createLeanDag( nodesMap[*it] );
        }
    }    
}

/** Assignment operator */
Model& Model::operator=( const Model& x ) {

    if ( this != &x ) {

        /* Free old model */
        leanNodesMap.clear();
        leanDagNodes.clear();
        nodesMap.clear();
        dagNodes.clear();
        sourceNodes.clear();
        
        /* Make copy of DAG by pulling from first node in x */
        if ( x.sourceNodes.size() > 0 ) {
            for (std::set<RbPtr<const DAGNode> >::const_iterator it = x.sourceNodes.begin(); it != x.sourceNodes.end(); ++it) {
                createModelFromDagNode( *it );
                createLeanDag( nodesMap[*it] );
            }
        }
    }

    return (*this);
}


/*
 * Add a source node and rebuilt the model.
 */
void Model::addSourceNode( const RbPtr<const DAGNode> &sourceNode ) {
    
    
    createModelFromDagNode( sourceNode );
    createLeanDag( nodesMap[sourceNode] );
    
}


void Model::createLeanDag(const DAGNode *fatDagNode) {
    fatDagNode->createLeanDag( leanNodesMap );
    
    /* insert new nodes in dagNodes member frame and direct access vector */
    std::map<const DAGNode*, InferenceDagNode*>::iterator i = leanNodesMap.begin();
    
    while ( i != leanNodesMap.end() ) {
        
        InferenceDagNode* theNewNode = (*i).second;
        ++i;
            
        // insert in direct access vector
        leanDagNodes.push_back( theNewNode );
    }
    
}

/**
 * Create the model from a single source node.
 *
 * First we clone the entire DAG, then we clone the moves and the monitors and set the new nodes appropriately.
 */
void Model::createModelFromDagNode(const RbPtr<const DAGNode> &theSourceNode) {
    
    if (theSourceNode == NULL)
        throw RbException("Cannot instantiate a model with a NULL DAG node.");
    
    theSourceNode->cloneDAG(nodesMap);
    // add the source node to our set of sources
    sourceNodes.insert( (DAGNode*)nodesMap[theSourceNode] );
    
    /* insert new nodes in dagNodes member frame and direct access vector */
    std::map<const DAGNode*, RbPtr<DAGNode> >::iterator i = nodesMap.begin();
        
    std::vector<std::map<const DAGNode*, RbPtr<DAGNode> >::iterator> nodeNeedToBeRemoved;
    while ( i != nodesMap.end() ) {
        
        DAGNode* theNewNode = (*i).second;
        
        // do not add myself into the list of nodes
        if ( theNewNode->isTypeSpec( DeterministicNode::getClassTypeSpec() ) ) {
            DeterministicNode* theDetNode = dynamic_cast<DeterministicNode*>((DAGNode*)theNewNode);
            const RbFunction& theFunction = theDetNode->getFunction();
            if (theFunction.isTypeSpec(ConstructorFunction::getClassTypeSpec())) {
                const ConstructorFunction& theConstructorFunction = dynamic_cast<const ConstructorFunction&>( theFunction );
                if ( theConstructorFunction.getReturnType() == Model::getClassTypeSpec() ) {
                    // remove the dag node holding the model constructor function from the dag
                    const std::set<RbPtr<DAGNode> >& parents = theDetNode->getParents();

                    while ( parents.size() > 0 ) {
                        DAGNode* node = *parents.begin();
                        theDetNode->removeParentNode(node);
                        node->removeChildNode( theDetNode );
                    }
                    
                    // remove the dag node holding the model constructor function also from the nodes map
                    // we do the actual remove later so that we do not disturb the loop
                    nodeNeedToBeRemoved.push_back(i);
                    ++i;
                    
                }
                else {
                    // increment the iterator;
                    ++i;
                    
                    // insert in direct access vector
                    dagNodes.push_back( theNewNode );
                }
            }
            else {
                // increment the iterator;
                ++i;
                
                // insert in direct access vector
                dagNodes.push_back( theNewNode );
            }
        }
        else {
            // increment the iterator;
            ++i;
            
            // insert in direct access vector
            dagNodes.push_back( theNewNode );
        }
    }
    
    for (std::vector<std::map<const DAGNode*, RbPtr<DAGNode> >::iterator>::iterator i = nodeNeedToBeRemoved.begin(); i != nodeNeedToBeRemoved.end(); i++) {
        nodesMap.erase(*i);
    }
            
}


/** Clone the object */
Model* Model::clone(void) const {

    return new Model(*this);
}


/** Find the offset of the node p in the vector v. */
int Model::findIndexInVector(const std::vector<RbPtr<DAGNode> >& v, const DAGNode* p) const {
    
	int cnt = 0;
    for (std::vector<RbPtr<DAGNode> >::const_iterator i=v.begin(); i!=v.end(); i++) 
    {
		cnt++;
		if ( (*i) == p )
			return cnt;
    }
	return -1;
}


/** Find the offset of the node p in the vector v. */
int Model::findIndexInVector(const std::vector<InferenceDagNode*>& v, const InferenceDagNode* p) const {
    
	int cnt = 0;
    for (std::vector<InferenceDagNode*>::const_iterator i=v.begin(); i!=v.end(); i++) 
    {
		cnt++;
		if ( (*i) == p )
			return cnt;
    }
	return -1;
}


/** Get class name of object */
const std::string& Model::getClassName(void) { 
    
    static std::string rbClassName = "Model";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Model::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


const MemberRules& Model::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ArgumentRule( "sinknode", true, RbObject::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis( RbObject::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


const std::map<const DAGNode *, InferenceDagNode *>& Model::getNodesMap( void ) const {
    return leanNodesMap;
}


/* Get the source node to clone the model */
const std::set<RbPtr<const DAGNode> >& Model::getSourceNodes( void ) const {
    return sourceNodes;
}

/** Get type spec */
const TypeSpec& Model::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void Model::printValue(std::ostream& o) const {

	/* print the information on the model */
	std::ostringstream msg;
	RBOUT("\n");
    msg << "Model with " << dagNodes.size() << " vertices";
	RBOUT(msg.str());
	msg.str("");
	RBOUT("-------------------------------------");
	int cnt = 0;
    for (std::vector<RbPtr<DAGNode> >::const_iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {   	
		msg << "Vertex " << ++cnt;
		std::string nameStr = msg.str();
		size_t nameStrSize = nameStr.size();
		for (size_t j=0; j<18-nameStrSize; j++)
			nameStr += " ";
		nameStr += (*i)->getName() + " (" + long((const DAGNode*)*i) + ")";
		RBOUT(nameStr);
		msg.str("");

		if ( (*i)->isTypeSpec( ConstantNode::getClassTypeSpec() ) )
			msg << "   Type         = Constant";
		else if ( (*i)->isTypeSpec( StochasticNode::getClassTypeSpec() ) )
			msg << "   Type         = Stochastic";
		else 
			msg << "   Type         = Deterministic";
		RBOUT(msg.str());
		msg.str("");
		
        if ( (*i)->isTypeSpec( DeterministicNode::getClassTypeSpec() ) ) {
            DAGNode* dnode = *i;
            DeterministicNode* node = static_cast<DeterministicNode*>( dnode );
            msg << "   Function     = " << node->getFunction().getTypeSpec();
        } else if ( (*i)->isTypeSpec( StochasticNode::getClassTypeSpec() ) ) {
            DAGNode* dnode = *i;
            StochasticNode* node = static_cast<StochasticNode*>( dnode );
            msg << "   Distribution = " << node->getDistribution().getTypeSpec();
        }
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
       
        msg << "   Value        = ";
        (*i)->getValue().printValue(msg);
            
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
		
		msg << "   Parents      = ";
		const std::set<RbPtr<DAGNode> > &parents = (*i)->getParents();
		for (std::set<RbPtr<DAGNode> >::const_iterator j=parents.begin(); j != parents.end(); j++) {   	
			int idx = findIndexInVector( dagNodes, (*j) );
			msg << idx;
        }
		if (parents.size() == 0)
			msg << "No Parents";
		RBOUT(msg.str());
		msg.str("");
		
		msg << "   Children     = ";
        const std::set<VariableNode*> &children = (*i)->getChildren();
		for (std::set<VariableNode*>::const_iterator j=children.begin(); j != children.end(); j++) {   	
			int idx = findIndexInVector( dagNodes, *j );
			msg << idx << " (" << long(*j) << ") ";
        }
		if (children.size() == 0)
			msg << "No Children";
		RBOUT(msg.str());
		msg.str("");
    }
	
    RBOUT("-------------------------------------");
    
    printLeanValue(o);
}



/** Print value for user */
void Model::printLeanValue(std::ostream& o) const {
    
	/* print the information on the model */
	std::ostringstream msg;
	RBOUT("\n");
    msg << "Model with " << leanDagNodes.size() << " vertices";
	RBOUT(msg.str());
	msg.str("");
	RBOUT("-------------------------------------");
	int cnt = 0;
    for (std::vector<InferenceDagNode*>::const_iterator i=leanDagNodes.begin(); i!=leanDagNodes.end(); i++) {   	
		msg << "Vertex " << ++cnt;
		std::string nameStr = msg.str();
		size_t nameStrSize = nameStr.size();
		for (size_t j=0; j<18-nameStrSize; j++)
			nameStr += " ";
		nameStr += (*i)->getName() + " (" + long((const DAGNode*)*i) + ")";
		RBOUT(nameStr);
		msg.str("");
        
		msg << "   Type         = " << typeid(*(*i)).name();
        
		RBOUT(msg.str());
		msg.str("");
		
        if ( typeid(**i) == typeid(DeterministicInferenceNode) ) {
            InferenceDagNode* dnode = *i;
            DeterministicInferenceNode* node = static_cast<DeterministicInferenceNode*>( dnode );
            msg << "   Function     = ";
            std::string func = node->getFunction()->toString();
            msg << func;
        } else if ( typeid(**i) == typeid(StochasticInferenceNode) ) {
            InferenceDagNode* dnode = *i;
            StochasticInferenceNode* node = static_cast<StochasticInferenceNode*>( dnode );
            msg << "   Distribution     = ";
            std::string dist =  node->getDistribution().toString();
            msg << dist;
        }
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
        
        msg << "   Value        = " << long( (*i)->getValue().value );
        
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
		
		msg << "   Parents      = ";
		const std::set<InferenceDagNode*> &parents = (*i)->getParents();
		for (std::set<InferenceDagNode*>::const_iterator j=parents.begin(); j != parents.end(); j++) {   	
			int idx = findIndexInVector( leanDagNodes, (*j) );
			msg << idx << " (" << long(*j) << ") ";
        }
		if (parents.size() == 0)
			msg << "No Parents";
		RBOUT(msg.str());
		msg.str("");
		
		msg << "   Children     = ";
        const std::set<VariableInferenceNode*> &children = (*i)->getChildren();
		for (std::set<VariableInferenceNode*>::const_iterator j=children.begin(); j != children.end(); j++) {   	
			int idx = findIndexInVector( leanDagNodes, *j );
			msg << idx << " (" << long(*j) << ") ";
        }
		if (children.size() == 0)
			msg << "No Children";
		RBOUT(msg.str());
		msg.str("");
    }
	
    RBOUT("-------------------------------------");
}


/** Set a member variable */
void Model::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "sinknode" || name == "" ) {
        
        const DAGNode* theNode = var->getDagNode();
        addSourceNode( theNode );
        
    }
    else {
        MemberObject::setConstMemberVariable(name, var);
    }
}


