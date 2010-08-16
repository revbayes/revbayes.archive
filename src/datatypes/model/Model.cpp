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
#include "DAGNodeContainer.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "StochasticNode.h"
#include "Model.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"
#include "UserInterface.h"

#include <sstream>


/** Default constructor */
Model::Model(void)
    : dagNodes(), maintainedHere() {
	
}

/** Sinknodes constructor */
Model::Model(const std::vector<DAGNode*>& sinkNodes)
    : RbComplex(), dagNodes(), maintainedHere() {

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
Model::Model(const Model& x)
    : RbComplex(), dagNodes(), maintainedHere() {

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
const StringVector& Model::getClass(void) const {

    static StringVector rbClass = StringVector(Model_name) + RbComplex::getClass();
    return rbClass;
}

/** Convert to object of another class. The caller manages the object. */
bool Model::isConvertibleTo(const std::string& type) const {

    return false;
}

int Model::getIndexForVector(DAGNode* p) const {

	int cnt = 0;
    for (std::vector<DAGNode*>::const_iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
		{
		cnt++;
		if ( (*i) == p )
			return cnt;
		}
	return -1;
}

/** Print value for user */
void Model::printValue(std::ostream& o) const {

	std::ostringstream msg;

	RBOUT("\n");
    msg << "Model with " << dagNodes.size() << " vertices" << std::endl;
	RBOUT(msg.str());
	msg.str("");
	RBOUT("-----------------------------------------------");
	int cnt = 0;
    for (std::vector<DAGNode*>::const_iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
		{   	
		msg << "Vertex " << ++cnt;
		RBOUT(msg.str());
		msg.str("");
		
		msg << "   Name         = " << (*i)->getName();
		RBOUT(msg.str());
		msg.str("");

		msg << "   Type         = " << (*i)->getType();
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
		std::set<DAGNode*>& parentNodes = (*i)->getParents();
		for (std::set<DAGNode*>::iterator j=parentNodes.begin(); j!=parentNodes.end(); j++) 
			{   	
			int idx = getIndexForVector((*j));
			msg << idx << " ";
			}
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
		
		msg << "   Children     = ";
        std::set<VariableNode*>& childrenNodes = (*i)->getChildren();
		for (std::set<VariableNode*>::iterator j=childrenNodes.begin(); j!=childrenNodes.end(); j++) 
			{   	
			int idx = getIndexForVector((*j));
			msg << idx << " ";
			}
		if ( msg.str() != "" )
			RBOUT(msg.str());
		msg.str("");
		
		}
		
#	if 0
    o << std::endl;
    o << "Model with " << dagNodes.size() << " DAG nodes" << std::endl;

    for (std::vector<DAGNode*>::const_iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {   	
        o << std::endl;
        o << "'" << (*i)->getName() << "' [" << (*i) << "] of type " << (*i)->getType() << std::endl;
        if ((*i)->isType(DeterministicNode_name))
            o << "Function: " << ((DeterministicNode*)(*i))->getFunction()->getType() << std::endl;
        if ((*i)->isType(StochasticNode_name))
            o << "Distribution: " << ((StochasticNode*)(*i))->getDistribution()->getType() << std::endl;
        (*i)->printParents(o);
        (*i)->printChildren(o);
    }
#	endif
}

/** Complete info about object */
std::string Model::toString(void) const {

    std::ostringstream o;
    o << "Model:" << std::endl;
    printValue(o);
    
    return o.str();
}


