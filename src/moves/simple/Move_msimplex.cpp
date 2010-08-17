/**
 * @file
 * This file contains the implementation of Move_msimplex,
 * which changes a simplex.
 *
 * @brief Implementation of Move_msimplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "Distribution.h"
#include "Move_msimplex.h"
#include "PosReal.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "StringVector.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser */
Move_msimplex::Move_msimplex(void) : SimpleMove(getMemberRules()) {

}

/** Constructor for internal use */
Move_msimplex::Move_msimplex(StochasticNode* node, double tuning, int nc, double weight, RandomNumberGenerator* rng) : SimpleMove(getMemberRules()) {

    setValue("tuning", new PosReal(tuning));
    setValue("num_cats", new RbInt(nc));
    setValue("weight", new PosReal(weight));
    setValue("rng",    rng);

    setNodePtr(node);
}

/** Clone object */
RbObject* Move_msimplex::clone(void) const {

    return new Move_msimplex(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Move_msimplex::convertTo(const std::string& type) const {

    return NULL;
}

/** Get class vector describing type of object */
const StringVector& Move_msimplex::getClass() const {

    static StringVector rbClass = StringVector(Move_mslide_name) + Move::getClass();
    return rbClass;
}

/** Return member rules */
const MemberRules& Move_msimplex::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
		{
        memberRules.push_back(new ArgumentRule("tuning", PosReal_name));
        memberRules.push_back(new ArgumentRule("num_cats", RbInt_name));

        /* Inherit weight and rng from Move, put it at back */
        const MemberRules& inheritedRules = Move::getMemberRules();
        memberRules.insert(memberRules.end(), inheritedRules.begin(), inheritedRules.end()); 

        rulesSet = true;
		}

    return memberRules;
}

/** Convert to object of another class. The caller manages the object. */
bool Move_msimplex::isConvertibleTo(const std::string& type) const {

    return false;
}

/** Perform the move */
double Move_msimplex::perform(std::set<StochasticNode*>& affectedNodes) {

    Simplex*               valPtr  = (Simplex*)(nodePtr->getValuePtr(affectedNodes));
    RandomNumberGenerator* rng     = (RandomNumberGenerator*)(getValue("rng"));
    double                 alpha0  = ((RbDouble*)(getValue("tuning")))->getValue();
    int                    k       = ((RbInt*)(getValue("num_cats")))->getValue();
	int                    n       = valPtr->getValue().size();

	std::vector<double> curVal = valPtr->getValue();
	std::vector<double> newVal = curVal;

	if ( k > n )
		{
		throw (RbException("Attempting to update too many simplex variables"));
		}
	else if ( k < 1 )
		{
		throw (RbException("Attempting to update too few simplex variables"));
		}
	else if ( k < n )
		{
		
		}
	else 
		{
		std::vector<double> alpha(curVal.size());
		for (int i=0; i<curVal.size(); i++)
			alpha[i] = curVal[i] * alpha0;
		newVal = RbStatistics::Dirichlet::rv( alpha, rng );
		}
		
    valPtr->setValue(newVal);
	
    return 0.0;
}


