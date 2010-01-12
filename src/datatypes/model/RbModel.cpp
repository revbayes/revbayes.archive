/*
 * RbModel.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include "DAGNode.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbModel.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "StringVector.h"
#include "VariableNode.h"

RbModel::RbModel(std::vector<DAGNode*>& s, RandomNumberGenerator* r) {

	rng = r;
    sinkDags = s;
    extractNodes(sinkDags);
    initializeUpdateInfo();
    initializeDAGs();
}

RbModel::RbModel(const RbModel& m) {

    sinkDags = m.sinkDags;
    dagNodes = m.dagNodes;
    changeableDagUpdateProbs = m.changeableDagUpdateProbs;
    changeableDags = m.changeableDags;
}

RbModel::~RbModel() {
    // TODO Auto-generated destructor stub
}

/**
 * @brief Pointer-based equal comparison
 *
 * Compares equality of this object to another RbObject.
 *
 * @param obj   The object of the comparison
 * @returns     Result of comparison
 *
 */
bool RbModel::equals(const RbObject* obj) const {

    return false;
}

void RbModel::initializeUpdateInfo(void) {

	double sumWeights = 0.0;
    for (std::set<DAGNode*>::iterator it=dagNodes.begin(); it!= dagNodes.end(); it++)
		{
        if ((*it)->isType(VariableNode_name)) {
            VariableNode* n = (VariableNode*)(*it);
            if (n->hasAttachedMove() == true) {
                double x = n->getUpdateWeight();
                sumWeights += x;
                changeableDags.push_back(*it);
                changeableDagUpdateProbs.push_back(x);
            }
        }
    }
	for (int i=0; i<changeableDagUpdateProbs.size(); i++)
		changeableDagUpdateProbs[i] /= sumWeights;
}

void RbModel::initializeDAGs(void) {

    for (std::set<DAGNode*>::iterator it=dagNodes.begin(); it!= dagNodes.end(); it++)
		{
        if ((*it)->isType(StochasticNode_name)) {
            StochasticNode* n = (StochasticNode*)(*it);
            n->getLnLikelihoodRatio();
            n->getLnPriorRatio();
        }
    }
}

DAGNode* RbModel::getDagToUpdate(void) {

    double u = rng->nextDouble();
    double sum = 0.0;
    for (int i=0; i<changeableDagUpdateProbs.size(); i++)
		{
		sum += changeableDagUpdateProbs[i];
		if ( u < sum )
			return changeableDags[i];
		}
    return NULL;
}

/** Get string showing inheritance */
const StringVector& RbModel::getClass(void) const {

    static StringVector rbClass = StringVector(RbNames::Model::name) + RbComplex::getClass();
    return rbClass;
}

void RbModel::monitor(int i) {
    for (std::set<DAGNode*>::iterator it=dagNodes.begin(); it!= dagNodes.end(); it++){

        if ((*it)->isType(VariableNode_name)) {
            VariableNode* n = (VariableNode*)(*it);
            n->monitor(i);
        }
    }
}

void	RbModel::extractNodes(std::vector<DAGNode*>& dn) {

	dagNodes.clear();
    for (std::vector<DAGNode*>::iterator it=dn.begin(); it!= dn.end(); it++) 
    	{
    	extractNodes(*it);
    	}
    	
}

void	RbModel::extractNodes(DAGNode* dn) {

    std::set<DAGNode*>& p = dn->getParentNodes(); 
    for (std::set<DAGNode*>::iterator itP=p.begin(); itP!= p.end(); itP++) 
    	{
    	extractNodes(*itP);
    	}
    	
    dagNodes.insert(dn);
}

RbObject& RbModel::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbModel& x = dynamic_cast<const RbModel&> (obj);

        RbModel& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbModel& x = dynamic_cast<const RbModel&> (*(obj.convertTo(RbNames::Model::name)));

            RbModel& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::Model::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbModel& RbModel::operator=(const RbModel& obj) {

    sinkDags.clear();
    for (std::vector<DAGNode*>::const_iterator i=obj.sinkDags.begin(); i!=obj.sinkDags.end(); i++){
        sinkDags.push_back((DAGNode*)(*i)->clone());
    }
    (*rng) = (*obj.rng);
    return (*this);
}


void RbModel::print(std::ostream& o) const {

    o << "Model:" << std::endl;
    for (int i=0; i<sinkDags.size(); i++)
    	sinkDags[i]->printValue(o);
}

RbObject* RbModel::clone(void) const {

    RbModel* x = new RbModel(*this);
    return (RbObject*)x;
}

void RbModel::printValue(std::ostream& o) const {

    for (int i=0; i<sinkDags.size(); i++)
    	sinkDags[i]->printValue(o);
}

std::string RbModel::toString(void) const {

    std::string tempStr = "Model\n";
	//tmpStr += sink->toString();
    return tempStr;
}

