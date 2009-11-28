/**
 * @file
 * This file contains the implementation of StochasticNode, which is derived
 * from DAGNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated probability density function.
 *
 * @brief Implementation of StochasticNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#include "Distribution.h"
#include "RbException.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"
#include "StochasticNode.h"


/**
 * @brief Constructor from distribution
 *
 * The constructor sets the distribution and then draws a
 * random value, which is used as the starting value.
 *
 * @param dist      The distribution
 *
 */
StochasticNode::StochasticNode(void)
    : DAGNode(), clamped(false) {

}   


/**
 * @brief Copy constructor
 *
 * Copy constructor for stochastic nodes.
 *
 * @param s     The stochastic node to be copied
 *
 */
StochasticNode::StochasticNode(const StochasticNode& s)
    : DAGNode(s), clamped(s.clamped), distribution(s.distribution) {
}


/**
 * @brief Destructor
 *
 * The stochastic node manages the distribution; the value is
 * managed and destroyed by the DAGNode base class.
 *
 */
StochasticNode::~StochasticNode() {

	if ( distribution != NULL )
		delete distribution;
}

RbObject& StochasticNode::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const StochasticNode& x = dynamic_cast<const StochasticNode&> (obj);

        StochasticNode& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const StochasticNode& x = dynamic_cast<const StochasticNode&> (*(obj.convertTo("const_node")));

            StochasticNode& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to const_node");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

StochasticNode& StochasticNode::operator=(const StochasticNode& obj) {

    changed = obj.changed;
    children = obj.children;
    (*lastMove) = (*obj.lastMove);
    members = obj.members;
    methods = obj.methods;
    monitors = obj.monitors;
    (*moves) = (*obj.moves);
    parents = obj.parents;
    (*storedValue) = (*obj.storedValue);
    touched = obj.touched;
    (*value) = (*obj.value);

    return (*this);
}

/**
 * @brief Clamp the node to a value
 *
 * This function sets the value of the stochastic node to some
 * observed value (data). Then the clamped flag is set to make
 * sure that the node is marked as clamped and no change is made
 * to the observed value.
 *
 * @param observedVal   The observed value
 */
void StochasticNode::clamp(RbObject* observedVal) {

	if (storedValue != NULL)
        delete storedValue;
	if (value != NULL)
        delete value;
    storedValue = NULL;
    value = observedVal;
    clamped = true;
}

double StochasticNode::getLnProbabilityRatio(void) {

	double lnNumerator = distribution->lnPdf(this->value);
	double lnDenominator = distribution->lnPdf(this->storedValue);
	return lnNumerator - lnDenominator;
}

void StochasticNode::assignDistribution(Distribution* d) {

	distribution = d;
}

void StochasticNode::initializeValue(RbObject* v) {

	value = v;
}

/**
 * @brief Calculate probability of node
 *
 * This function calculates the probability of the current value given
 * the values of any parent nodes in the DAG, that is, the probability
 * density of the value of the node given the current values of the
 * parameters of the probability distribution associated with the node.
 *
 * @returns         Ln probability
 *
 */
double StochasticNode::lnProb() const {

	return distribution->lnPdf(value);
}


/**
 * @brief Thouch affected nodes
 *
 * This function touches all affected DAG nodes, i.e. marks them as changed.
 * Unlike deterministc nodes, stochastic nodes do not need to 
 *
 */
void StochasticNode::touchAffected() {

    // TODO: This is not quite right; we need to go from current stochasticNode and down
    //       to next stochasticNode but this requires slight modification of this method
    //       in the DAGNode base class as well as here.
    if (!touched) {
        touch();
        for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++)
            (*i)->touchAffected();
    }
}
