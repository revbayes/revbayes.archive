/**
 * @file
 * This file contains the implementation of StochasticNode, which is derived
 * from DAGNode. StochasticNode is used for DAG nodes holding stochastic
 * variables.
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
 * @brief Calculate probability of node
 *
 * This function calculates the probability of the current value given
 * the current values of any parent nodes in the DAG, that is, the probability
 * density of the value of the node given the current values of the parameters
 * of the probability distribution associated with the node.
 *
 * @returns         Ln probability
 *
 */
void StochasticNode::lnProb() {

	value->print();
	storedValue->print();
}

