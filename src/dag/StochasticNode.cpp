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
StochasticNode::StochasticNode(Distribution* dist)
    : DAGNode(), distribution(dist) {

    value = distribution->rv();
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
    : DAGNode(s), distribution(s.distribution) {
}


/**
 * @brief Destructor
 *
 * The stochastic node manages the distribution; the object is
 * managed and destroyed by the DAGNode base class.
 *
 */
StochasticNode::~StochasticNode() {

	if ( distribution != NULL )
		delete distribution;
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

	if (value != NULL)
		delete value;

    value = observedVal;
}


/**
 * @brief Clone the stochastic node
 *
 * This function clones the stochastic node.
 *
 */
StochasticNode* StochasticNode::clone() const {

	return new StochasticNode(*this);
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
void StochasticNode::lnProb() {

	value->print();
	storedValue->print();
}

