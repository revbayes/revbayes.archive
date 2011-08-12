/**
 * @file
 * This file contains the implementation of Model, which is used to hold
 * DAGs copied from the working environment.
 *
 * @brief Implementation of Model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#include "ContainerIterator.h"
#include "ConstantNode.h"
#include "ConstructorFunction.h"
#include "DAGNode.h"
#include "DagNodeContainer.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "StochasticNode.h"
#include "Model.h"
#include "RbException.h"
#include "RbNames.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "UserInterface.h"

#include <algorithm>
#include <sstream>


/** Default constructor for a Model object. */
Model::Model( void ) : ConstantMemberObject(getMemberRules()) {
}


/** Constructor for the Model object that takes as an argument a vector containing at least one of the DAGNodes in the graph representing the model. */
Model::Model( const std::vector<DAGNode*>& sinkNodes ) : ConstantMemberObject(getMemberRules()) {

    /* Check to see that we have at least one DAG node */
    if ( sinkNodes.empty() )
        throw RbException( "No sink nodes specified" );

    /* Make copy of DAG: pulling from first sink node is sufficient */
    sinkNodes.front()->cloneDAG( nodesMap );

    /* Check that all sink nodes are included */
    for ( std::vector<DAGNode*>::const_iterator i = sinkNodes.begin(); i != sinkNodes.end(); i++ ) {
        if ( nodesMap.find(*i) == nodesMap.end() ) {
            for ( std::map<const DAGNode*, DAGNode*>::iterator j = nodesMap.begin(); j != nodesMap.end(); j++ )
                delete (*j).second;
            throw RbException( "Not all sink nodes are connected to the same DAG" );
        }
    }

}


/** Copy constructor */
Model::Model( const Model& x ) : ConstantMemberObject( x ) {

    /* Make copy of DAG by pulling from first node in x */
    std::map<const DAGNode*, DAGNode*> newNodes;
    if ( x.dagNodes.size() > 0 )
        x.dagNodes[0]->cloneDAG( newNodes );
   

    /* Insert new nodes in dagNodes member frame and direct access vector */
    for ( std::map<const DAGNode*, DAGNode*>::const_iterator i = x.nodesMap.begin(); i != x.nodesMap.end(); i++ ) {

        const DAGNode *theOrgNode = (*i).first;
        DAGNode       *theOldNode = (*i).second;
        
        // find the new node in our temporary map
        DAGNode       *theNewNode = newNodes.find(theOldNode)->second;

        // Insert in direct access vector
        dagNodes.push_back( theNewNode );
        
        // add the pair into the map of original nodes
        nodesMap[theOrgNode] = theNewNode;
    }
}

/** Assignment operator */
Model& Model::operator=( const Model& x ) {

    if ( this != &x ) {

        /* Free old model */
//        dagNodeMembers.clear();
        dagNodes.clear();
        nodesMap.clear();
        
        /* Make copy of DAG by pulling from first node in x */
        std::map<const DAGNode*, DAGNode*> newNodes;
        if ( x.dagNodes.size() > 0 )
            x.dagNodes[0]->cloneDAG( newNodes );

        /* Insert new nodes in dagNodes member frame and direct access vector */
        for ( std::map<const DAGNode*, DAGNode*>::const_iterator i = x.nodesMap.begin(); i != x.nodesMap.end(); i++ ) {
            
            const DAGNode *theOrgNode = (*i).first;
            DAGNode       *theOldNode = (*i).second;
            
            // find the new node in our temporary map
            DAGNode       *theNewNode = newNodes.find(theOldNode)->second;
            
            // Insert in direct access vector
            dagNodes.push_back( theNewNode );
            
            // add the pair into the map of original nodes
            nodesMap[theOrgNode] = theNewNode;
        }
    }

    return (*this);
}


/** Clone the object */
Model* Model::clone(void) const {

    return new Model(*this);
}


/** Find the offset of the node p in the vector v. */
int Model::findIndexInVector(const std::vector<DAGNode*>& v, const DAGNode* p) const {
    
	int cnt = 0;
    for (std::vector<DAGNode*>::const_iterator i=v.begin(); i!=v.end(); i++) 
    {
		cnt++;
		if ( (*i) == p )
			return cnt;
    }
	return -1;
}



/** Get class vector describing object */
const VectorString& Model::getClass(void) const {

    static VectorString rbClass = VectorString(Model_name) + MemberObject::getClass();
    return rbClass;
}


std::vector<VariableNode*> Model::getClonedDagNodes(std::vector<VariableNode*> &orgNodes) const {

    // create a vector for the cloned nodes
    std::vector<VariableNode*> clones;
    
    // find each original node and add it to the map
    for (std::vector<VariableNode*>::iterator it=orgNodes.begin(); it!=orgNodes.end(); it++) {
        std::map<const DAGNode*, DAGNode*>::const_iterator orgClonePair = nodesMap.find(*it);
        if (orgClonePair != nodesMap.end()) {
            clones.push_back(dynamic_cast<VariableNode*>(orgClonePair->second));
        }
        else {
            throw RbException("Could not find original DAG node in Model.");
        }
    }
    
    // return the clones
    return clones;
}


const MemberRules& Model::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ValueRule( "sinknode"  , RbObject_name ) );
        
        rulesSet = true;
    }
    
    return memberRules;
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
    for (std::vector<DAGNode*>::const_iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
		{   	
		msg << "Vertex " << ++cnt;
		std::string nameStr = msg.str();
		size_t nameStrSize = nameStr.size();
		for (size_t j=0; j<18-nameStrSize; j++)
			nameStr += " ";
		nameStr += (*i)->getName();
		RBOUT(nameStr);
		msg.str("");

		if ( (*i)->getType() == ConstantNode_name )
			msg << "   Type         = Constant";
		else if ( (*i)->getType() == StochasticNode_name )
			msg << "   Type         = Stochastic";
		else 
			msg << "   Type         = Deterministic";
		RBOUT(msg.str());
		msg.str("");
		
        if ((*i)->isType(DeterministicNode_name))
            msg << "   Function     = " << ((DeterministicNode*)(*i))->getFunction()->getType();
        else if ((*i)->isType(StochasticNode_name))
            msg << "   Distribution = " << ((StochasticNode*)(*i))->getDistribution()->getType();
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
       
        msg << "   Value        = " << (*i)->getValue()->richInfo();
            
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
		
		msg << "   Parents      = ";
		std::set<DAGNode*> &parents = (*i)->getParents();
		for (std::set<DAGNode*>::const_iterator j=parents.begin(); j != parents.end(); j++) 
			{   	
			int idx = findIndexInVector( dagNodes, (*j) );
			msg << idx << " ";
			}
		if (parents.size() == 0)
			msg << "No Parents";
		RBOUT(msg.str());
		msg.str("");
		
		msg << "   Children     = ";
            std::set<VariableNode*> &children = (*i)->getChildren();
		for (std::set<VariableNode*>::const_iterator j=children.begin(); j != children.end(); j++) 
			{   	
			int idx = findIndexInVector( dagNodes, (*j) );
			msg << idx << " ";
			}
		if (children.size() == 0)
			msg << "No Children";
		RBOUT(msg.str());
		msg.str("");
		}
	RBOUT("-------------------------------------");
}


/** Complete info about object */
std::string Model::richInfo(void) const {

    std::ostringstream o;
    o << "Model:" << std::endl;
    printValue(o);
    
    return o.str();
}

/** Set a member variable */
void Model::setMemberVariable(const std::string& name, Variable* var) {
    DAGNode *theNode = var->getDagNodePtr();
    if (name == "sinknode") {
        // test whether var is a DagNodeContainer
        while (theNode->getValue()->isType(DagNodeContainer_name)) {
            DagNodeContainer* container = dynamic_cast<DagNodeContainer*>(theNode->getValuePtr());
            theNode = container->getElement(0)->getDagNodePtr();
        }
        
        
        // if the var is not NULL we pull the DAG from it
        nodesMap.clear();
        if (theNode != NULL)
            theNode->cloneDAG(nodesMap);
        
        /* Insert new nodes in dagNodes member frame and direct access vector */
        for ( std::map<const DAGNode*, DAGNode*>::iterator i = nodesMap.begin(); i != nodesMap.end(); i++ ) {
            
            DAGNode*       theNewNode = (*i).second;
            
            // do not add myself into the list of nodes
            if (theNewNode->isType(DeterministicNode_name)) {
                DeterministicNode *theDetNode = dynamic_cast<DeterministicNode*>(theNewNode);
                const RbFunction *theFunction = theDetNode->getFunction();
                if (theFunction->isType(ConstructorFunction_name)) {
                    const ConstructorFunction *theConstructorFunction = dynamic_cast<const ConstructorFunction*>(theFunction);
                    if (theConstructorFunction->getTemplateObjectType() == Model_name) {
                        // remove the dag node holding the model constructor function from the dag
                        std::set<DAGNode*> parents = theDetNode->getParents();
                        for (std::set<DAGNode*>::iterator it=parents.begin(); it!=parents.end(); it++) {
                            theDetNode->removeParentNode(*it);
                            (*it)->removeChildNode(theDetNode);
                        }
                        // skip over adding the model node
                        continue;
                    }
                }
            }
            // Insert in direct access vector
            dagNodes.push_back( theNewNode );
        }
    }
    
    ConstantMemberObject::setMemberVariable(name, var);
}


