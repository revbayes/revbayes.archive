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
#include "DAGNode.h"
#include "ContainerNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "FunctionNode.h"
#include "StochasticNode.h"
#include "Model.h"
#include "RbException.h"
#include "RbNames.h"
#include "VariableSlot.h"
#include "VectorString.h"
#include "UserInterface.h"

#include <algorithm>
#include <sstream>


/** Default constructor for a Model object. */
Model::Model( void ) : MemberObject() {
}


/** Constructor for the Model object that takes as an argument a vector containing at least one of the DAGNodes in the graph representing the model. */
Model::Model( const std::vector<DAGNode*>& sinkNodes ) : MemberObject() {

    /* Check to see that we have at least one DAG node */
    if ( sinkNodes.empty() )
        throw RbException( "No sink nodes specified" );

    /* Make copy of DAG: pulling from first sink node is sufficient */
    std::map<const DAGNode*, DAGNode*> newNodes;
    sinkNodes.front()->cloneDAG( newNodes );

    /* Check that all sink nodes are included */
    for ( std::vector<DAGNode*>::const_iterator i = sinkNodes.begin(); i != sinkNodes.end(); i++ ) {
        if ( newNodes.find(*i) == newNodes.end() ) {
            for ( std::map<const DAGNode*, DAGNode*>::iterator j = newNodes.begin(); j != newNodes.end(); j++ )
                delete (*j).second;
            throw RbException( "All sink nodes are not connected to the same DAG" );
        }
    }

    /* Insert new nodes in dagNodes member frame and direct access vector */
    for ( std::map<const DAGNode*, DAGNode*>::iterator i = newNodes.begin(); i != newNodes.end(); i++ ) {

        const DAGNode* theOldNode = (*i).first;
        DAGNode*       theNewNode = (*i).second;

        // Insert named nodes into hidden variable frame
        if ( theOldNode->getSlot() != NULL ) {

            // Create new variable slot from old slot if old node was in a slot
            const VariableSlot* oldSlot = theOldNode->getSlot();
            dagNodeMembers.addVariable( oldSlot->getName(), oldSlot->getTypeSpec(), theNewNode );
        }

        // Insert in direct access vector
        dagNodes.push_back( theNewNode );
    }
}


/** Copy constructor */
Model::Model( const Model& x) : MemberObject() {

    /* Make copy of DAG by pulling from first node in x */
    std::map<const DAGNode*, DAGNode*> newNodes;
    if ( x.dagNodes.size() > 0 )
        x.dagNodes[0]->cloneDAG( newNodes );

    /* Insert new nodes in dagNodes member frame and direct access vector */
    int count = 1;
    for ( std::map<const DAGNode*, DAGNode*>::iterator i = newNodes.begin(); i != newNodes.end(); i++ ) {

        const DAGNode* theOldNode = (*i).first;
        DAGNode*       theNewNode = (*i).second;

        if ( theOldNode->getSlot() != NULL ) {

            // Create new variable slot from old slot if old node was in a slot
            const VariableSlot* oldSlot = theOldNode->getSlot();
            dagNodeMembers.addVariable( oldSlot->getName(), oldSlot->getTypeSpec(), theNewNode );
        }

        // Insert in direct access vector
        dagNodes.push_back( theNewNode );
    }
}

/** Assignment operator */
Model& Model::operator=( const Model& x ) {

    if ( this != &x ) {

        /* Free old model */
        dagNodeMembers.clear();
        dagNodes.clear();
        
        /* Make copy of DAG by pulling from first node in x */
        std::map<const DAGNode*, DAGNode*> newNodes;
        if ( x.dagNodes.size() > 0 )
            x.dagNodes[0]->cloneDAG( newNodes );

        /* Insert new nodes in dagNodes member frame and direct access vector */
        int count = 1;
        for ( std::map<const DAGNode*, DAGNode*>::iterator i = newNodes.begin(); i != newNodes.end(); i++ ) {

            const DAGNode* theOldNode = (*i).first;
            DAGNode*       theNewNode = (*i).second;

            if ( theOldNode->getSlot() != NULL ) {

                // Create new variable slot from old slot if old node was in a slot
                const VariableSlot* oldSlot = theOldNode->getSlot();
                dagNodeMembers.addVariable( oldSlot->getName(), oldSlot->getTypeSpec(), theNewNode );
            }

            // Insert in direct access vector
            dagNodes.push_back( theNewNode );
        }
    }

    return (*this);
}


/** Clone the object */
Model* Model::clone(void) const {

    return new Model(*this);
}


/** Get class vector describing object */
const VectorString& Model::getClass(void) const {

    static VectorString rbClass = VectorString(Model_name) + MemberObject::getClass();
    return rbClass;
}


/** Get a list of the nodes in dagNodes that are exposed. */
void Model::getExposedDagNodes(std::vector<DAGNode*>& exposedDagNodes, bool exposeEverybody, bool usePlates) const {

	// initialize a set of DAGNodes that are excluded from being printed in the model
	std::set<DAGNode*> excludedNodes;
    for (std::vector<DAGNode*>::const_iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
		{
		if (usePlates == true)
			{
			if ( (*i)->getDAGType() == ContainerNode_name )
				{
				std::set<DAGNode*>& parentNodes = (*i)->getParents();
				for (std::set<DAGNode*>::iterator j=parentNodes.begin(); j != parentNodes.end(); j++) 
					excludedNodes.insert( (*j) );
				}
			}
		else 
			{
			if ( (*i)->getDAGType() == ContainerNode_name )
				excludedNodes.insert( (*i) );
			}
		}
	
	// initialize a vector of DAGNodes to be included in the printed model
	exposedDagNodes.clear();
    for (std::vector<DAGNode*>::const_iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
		{
		bool nodeIsExcluded = false;
		std::set<DAGNode*>::iterator it = excludedNodes.find( (*i) );
		if (it != excludedNodes.end())
			nodeIsExcluded = true;
		if (exposeEverybody == true)
			{
			exposedDagNodes.push_back( (*i) );
			}
		else 
			{
			if ( !((*i)->isDAGType(LookupNode_name) || (*i)->isDAGType(ConverterNode_name)) )
                exposedDagNodes.push_back( (*i) );
			}
		}
}


/** Get a list of the DAG nodes that are children of p and in the list nodeList. */
void Model::getExposedChildren(DAGNode* p, std::set<VariableNode*>& ec, std::vector<DAGNode*>& nodeList) const {

	std::set<VariableNode*>& childNodes = p->getChildren();
	for (std::set<VariableNode*>::iterator i=childNodes.begin(); i != childNodes.end(); i++) 
		{
		DAGNode* q = dynamic_cast<DAGNode*>((*i));
		if (q)
			{
			bool isQinList = false;
			std::vector<DAGNode*>::iterator it = std::find ( nodeList.begin(), nodeList.end(), q );
			if (it != nodeList.end())	
				isQinList = true;
			
			if ( isQinList == true )
				ec.insert( (*i) );
			else 
				getExposedChildren( q, ec, nodeList );
			}
		else 
			{
			throw (RbException("Cannot cast VariableNode to be a DAGNode"));
			}
		}
}


/** Get a list of the DAG nodes that are parents of p and in the list nodeList. */
void Model::getExposedParents(DAGNode* p, std::set<DAGNode*>& ep, std::vector<DAGNode*>& nodeList) const {

	std::set<DAGNode*>& parentNodes = p->getParents();
	for (std::set<DAGNode*>::iterator i=parentNodes.begin(); i != parentNodes.end(); i++) 
		{
		bool isIinList = false;
		std::vector<DAGNode*>::iterator it = std::find ( nodeList.begin(), nodeList.end(), (*i) );
		if (it != nodeList.end())	
			isIinList = true;
		
		if ( isIinList == true )
			ep.insert( (*i) );
		else 
			getExposedParents( (*i), ep, nodeList );
		}
}


/** Find the offset of the node p in the vector v. */
int Model::getIndexForVector(const std::vector<DAGNode*>& v, const DAGNode* p) const {

	int cnt = 0;
    for (std::vector<DAGNode*>::const_iterator i=v.begin(); i!=v.end(); i++) 
		{
		cnt++;
		if ( (*i) == p )
			return cnt;
		}
	return -1;
}


/** Print value for user */
void Model::printValue(std::ostream& o) const {

	/* set flags for writing out the model */
	bool showHiddenDagNodes = false;
	bool usePlates          = true;

	/* initialize a list of the DAGNodes to be printed as part of the model */
	std::vector<DAGNode*> exposedDagNodes;
	getExposedDagNodes( exposedDagNodes, showHiddenDagNodes, usePlates );

	/* print the information on the model */
	std::ostringstream msg;
	RBOUT("\n");
    msg << "Model with " << exposedDagNodes.size() << " vertices";
	RBOUT(msg.str());
	msg.str("");
	RBOUT("-------------------------------------");
	int cnt = 0;
    for (std::vector<DAGNode*>::const_iterator i=exposedDagNodes.begin(); i!=exposedDagNodes.end(); i++) 
		{   	
		msg << "Vertex " << ++cnt;
		std::string nameStr = msg.str();
		int nameStrSize = nameStr.size();
		for (int j=0; j<18-nameStrSize; j++)
			nameStr += " ";
		nameStr += (*i)->getName();
		RBOUT(nameStr);
		msg.str("");

		if ( (*i)->getDAGType() == ConstantNode_name )
			msg << "   Type         = Constant";
		else if ( (*i)->getDAGType() == StochasticNode_name )
			msg << "   Type         = Stochastic";
		else if ( (*i)->getDAGType() == ContainerNode_name )
			msg << "   Type         = Plate";
		else if ( (*i)->getDAGType() == MemberNode_name )
			msg << "   Type         = Member object";
		else 
			msg << "   Type         = Deterministic";
		RBOUT(msg.str());
		msg.str("");
		
        if ((*i)->isDAGType(FunctionNode_name))
            msg << "   Function     = " << ((FunctionNode*)(*i))->getFunction()->getType();
        else if ((*i)->isDAGType(StochasticNode_name))
            msg << "   Distribution = " << ((StochasticNode*)(*i))->getDistribution()->getType();
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
       
		if ( (*i)->isDAGType(ConstantNode_name) )
            msg << "   Value        = " << ((ConstantNode*)(*i))->getValue()->richInfo();
		else if ( (*i)->isDAGType(StochasticNode_name) )
            msg << "   Value        = " << ((StochasticNode*)(*i))->getValue()->richInfo();
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
		
		msg << "   Parents      = ";
		std::set<DAGNode*> exposedParents;
		getExposedParents( (*i), exposedParents, exposedDagNodes );
		for (std::set<DAGNode*>::const_iterator j=exposedParents.begin(); j != exposedParents.end(); j++) 
			{   	
			int idx = getIndexForVector( exposedDagNodes, (*j) );
			msg << idx << " ";
			}
		if (exposedParents.size() == 0)
			msg << "No Parents";
		RBOUT(msg.str());
		msg.str("");
		
		msg << "   Children     = ";
		std::set<VariableNode*> exposedChildren;
		getExposedChildren( (*i), exposedChildren, exposedDagNodes );
		for (std::set<VariableNode*>::const_iterator j=exposedChildren.begin(); j != exposedChildren.end(); j++) 
			{   	
			int idx = getIndexForVector( exposedDagNodes, (*j) );
			msg << idx << " ";
			}
		if (exposedChildren.size() == 0)
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


