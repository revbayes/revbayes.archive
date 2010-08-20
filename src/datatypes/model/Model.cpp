/**
 * @file
 * This file contains the implementation of Model, which is used to hold
 * DAGs copied from the working environment.
 *
 * @brief Implementation of Model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#include "ContainerIterator.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DAGNodePlate.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "StochasticNode.h"
#include "Model.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"
#include "UserInterface.h"

#include <algorithm>
#include <sstream>


/** Default constructor for a Model object. */
Model::Model(void) : dagNodes(), maintainedHere() {
	
}

/** Constructor for the Model object that takes as an argument a vector containing at least one of the DAGNodes in the graph representing the model. */
Model::Model(const std::vector<DAGNode*>& sinkNodes) : RbComplex(), dagNodes(), maintainedHere() {

    /* Check to see that we have at least one DAG node */
    if (sinkNodes.empty())
        throw RbException("No sink nodes specified");

    /* Make copy of DAG: pulling from first sink node is sufficient */
    std::map<DAGNode*, DAGNode*> newNodes;
    sinkNodes.front()->cloneDAG(newNodes);

    /* Check that all sink nodes are included */
    for (std::vector<DAGNode*>::const_iterator i=sinkNodes.begin(); i!=sinkNodes.end(); i++) {
        if (newNodes.find(*i) == newNodes.end()) {
            for (std::map<DAGNode*, DAGNode*>::iterator j=newNodes.begin(); j!=newNodes.end(); j++)
                delete (*j).second;
            throw RbException("All sink nodes are not connected to the same DAG");
        }
    }

    /* Insert new nodes in dagNodes member variable */
    int count = 1;
    for (std::map<DAGNode*, DAGNode*>::iterator i=newNodes.begin(); i!=newNodes.end(); i++) {
        DAGNode* theNode = (*i).second;
        if (theNode->getName() == "") {
            maintainedHere.push_back(true);
            if ((*i).first->getName() != "")
                theNode->setName((*i).first->getName());
            else {
                std::ostringstream name;
                name << "Unnamed node " << count++;
                theNode->setName(name.str());
            }
        }
        else
            maintainedHere.push_back(false);
        dagNodes.push_back(theNode);
    }
}

/** Destructor */
Model::~Model(void) {

    std::vector<DAGNode*>::iterator i;
    std::vector<bool>::iterator j;
    for (i=dagNodes.begin(), j=maintainedHere.begin(); i!=dagNodes.end(); i++, j++) {
        if ((*j) == true) {
            (*i)->setName("");
            if ((*i)->numRefs() == 0)
                delete (*i);
        }
    }
}

/** Copy constructor */
Model::Model(const Model& x) : RbComplex(), dagNodes(), maintainedHere() {

    /* Make copy of DAG by pulling from first node in x */
    std::map<DAGNode*, DAGNode*> newNodes;
    if (x.dagNodes.size() > 0)
        x.dagNodes[0]->cloneDAG(newNodes);

    /* Insert new nodes in dagNodes member variable */
    int count = 1;
    for (std::map<DAGNode*, DAGNode*>::iterator i=newNodes.begin(); i!=newNodes.end(); i++) {
        DAGNode* theNode = (*i).second;
        if (theNode->getName() == "") {
            maintainedHere.push_back(true);
            if ((*i).first->getName() != "")
                theNode->setName((*i).first->getName());
            else {
                std::ostringstream name;
                name << "Unnamed node " << count++;
                theNode->setName(name.str());
            }
        }
        else
            maintainedHere.push_back(false);
        dagNodes.push_back(theNode);
    }
}

/** Assignment operator */
Model& Model::operator=(const Model& x) {

    if (this != &x) {

        /* Free old model */
        std::vector<DAGNode*>::iterator i;
        std::vector<bool>::iterator j;
        for (i=dagNodes.begin(), j=maintainedHere.begin(); i!=dagNodes.end(); i++, j++) {
            if ((*j) == true) {
                (*i)->setName("");
                if ((*i)->numRefs() == 0)
                    delete (*i);
            }
        }

        /* Make copy of DAG by pulling from first node in x */
        std::map<DAGNode*, DAGNode*> newNodes;
        if (x.dagNodes.size() > 0)
            x.dagNodes[0]->cloneDAG(newNodes);

        /* Insert new nodes in dagNodes member variable */
        int count = 1;
        for (std::map<DAGNode*, DAGNode*>::iterator i=newNodes.begin(); i!=newNodes.end(); i++) {
            DAGNode* theNode = (*i).second;
            if (theNode->getName() == "") {
                maintainedHere.push_back(true);
                if ((*i).first->getName() != "")
                    theNode->setName((*i).first->getName());
                else {
                    std::ostringstream name;
                    name << "Unnamed node " << count++;
                    theNode->setName(name.str());
                }
            }
            else
                maintainedHere.push_back(false);
            dagNodes.push_back(theNode);
        }
    }

    return (*this);
}

/** Clone the object */
Model* Model::clone(void) const {

    return new Model(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Model::convertTo(const std::string& type) const {

    throw (RbException("Conversion to " + type + " not supported"));
    return NULL;
}

/** Pointer-based equals comparison */
bool Model::equals(const RbObject* obj) const {

    return false;
}

/** Get class vector describing object */
const VectorString& Model::getClass(void) const {

    static VectorString rbClass = VectorString(Model_name) + RbComplex::getClass();
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
			if ( (*i)->getType() == DAGNodeContainer_name )
				{
				std::set<DAGNode*>& parentNodes = (*i)->getParents();
				for (std::set<DAGNode*>::iterator j=parentNodes.begin(); j != parentNodes.end(); j++) 
					excludedNodes.insert( (*j) );
				}
			}
		else 
			{
			if ( (*i)->getType() == DAGNodeContainer_name )
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
			if ( (*i)->getIsDagExposed() == true && nodeIsExcluded == false )
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

/** Convert to object of another class. The caller manages the object. */
bool Model::isConvertibleTo(const std::string& type) const {

    return false;
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

		if ( (*i)->getType() == ConstantNode_name )
			msg << "   Type         = Constant";
		else if ( (*i)->getType() == StochasticNode_name )
			msg << "   Type         = Stochastic";
		else if ( (*i)->getType() == DAGNodeContainer_name )
			msg << "   Type         = Plate";
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
       
		if ( (*i)->isType(ConstantNode_name) )
            msg << "   Value        = " << ((ConstantNode*)(*i))->getValue()->toString();
		else if ( (*i)->isType(StochasticNode_name) )
            msg << "   Value        = " << ((StochasticNode*)(*i))->getValue()->toString();
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
std::string Model::toString(void) const {

    std::ostringstream o;
    o << "Model:" << std::endl;
    printValue(o);
    
    return o.str();
}


