/*
 * MCMC.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include "RbMcmc.h"

RbMcmc::RbMcmc(RbModel* mp) {

	modelPtr = mp;
	members.insert(std::make_pair("burnin", new RbInt(10000)));
	members.insert(std::make_pair("chainLength", new RbInt(1000000)));
}

RbMcmc::RbMcmc(RbModel* mp, RbInt* b, RbInt* cl) {

	modelPtr = mp;
	members.insert(std::make_pair("burnin",b));
	members.insert(std::make_pair("chainLength",cl));
}

RbMcmc::RbMcmc(RbMcmc& m) {

	// TODO: Deep copy of model needs to be performed
}

RbMcmc::~RbMcmc(void) {

}

void RbMcmc::runChain(void) {

	int chainLength = 0;
	int burnIn = 0;
	for (std::map<std::string,RbObject*>::iterator m=members.begin(); m != members.end(); m++)
		{
		if ( m->first == "burnin" )
			burnIn = ((RbInt*)m->second)->getValue();
		else if ( m->first == "chainLength" )
			chainLength = ((RbInt*)m->second)->getValue();
		}
		
	for (int i=0; i<(burnIn+chainLength); i++) 
		{
		// get the DAG-Node
		DAGNode* node = getDagToChange();
		
		// update the dag-node
		double lnHastingsRatio = update(node);
		
		// get prior
		double lnPriorRatio = getLnPriorRatio(node);
		
		// get likelihood
		double lnLikelihoodRatio = getLnLikelihoodRatio(node);
		
		// calc acceptance
		double r = calculateAcceptanceProb( lnLikelihoodRatio + lnPriorRatio + lnHastingsRatio );
		
		// accept/reject the move
		if ( uniformRVfromSomewhere < r )
			accept(node);
		else 
			reject(node);
			
		// monitor
		if (i > burnIn) 
			{
			modelPtr->monitor(i-burnIn);
			}
		}
			
}

double RbMcmc::calculateAcceptanceProb(double lnR) {

	if (lnR > 0.0)
		return 1.0;
	else if (lnR < -300.0)
		return 0.0;
	else
		return exp(lnR);
}

RbObject* RbMcmc::clone() const {
	RbMcmc* x = new RbMcmc(*this);
	return (RbObject*)x;
}

bool RbMcmc::equals(const RbObject* obj) const {
	return (this == obj);
}

const StringVector& RbMcmc::getClass() const { 
	return rbClass; 
}            //!< Get class

RbObject& RbMcmc::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbMcmc& x = dynamic_cast<const RbMcmc&> (obj);

        RbMcmc& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbMcmc& x = dynamic_cast<const RbMcmc&> (*(obj.convertTo(RbNames::MCMC::name)));

            RbMcmc& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::MCMC::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbMcmc& RbMcmc::operator=(const RbMcmc& obj) {

    (*modelPtr) = (*obj.modelPtr);
    return (*this);
}

void RbMcmc::print(std::ostream& o) const {
	o << "MCMC on model: " << modelPtr->toString() << std::endl;
}

void RbMcmc::printValue(std::ostream& o) const {
	o << "MCMC on model: " << modelPtr->toString() << std::endl;
}

std::string RbMcmc::toString(void) const {
	std::string tmp = "MCMC on model: " + modelPtr->toString();
	return tmp;
}

DAGNode* RbMcmc::getDagToUpdate() {
	modelPtr->getDagToUpdate();
}

double RbMcmc::getLnPriorRatio(DAGNode* d) {
	return d->getLnPriorRatio();
}

double RbMcmc::update(DAGNode* d) {
	return d->performMove();
}

double RbMcmc::getLnLikelihoodRatio(DAGNode* d) {
	return d->getLnLikelihoodRatio();
}

void RbMcmc::accept(DAGNode* d) {
	d->accept();
}

void RbMcmc::reject(DAGNode* d) {
	d->reject();
}


