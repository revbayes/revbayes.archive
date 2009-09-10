/**
 * @file
 * This file contains the declaration of StochasticNode, which is a derived
 * class from DAGNode. StochasticNodes are DAG nodes with an distribution assigned to its value.
 *
 * @brief Declaration of StochasticNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#ifndef StochasticNode_H
#define StochasticNode_H

#include <string>
#include <vector>
#include "DAGNode.h"

class Distribution;
class RbDataType;
class StochasticNode : public DAGNode {

public:
	StochasticNode(void);                                       //!< Default constructor TODO maybe we should not have a default constructor
	StochasticNode(RbDataType* dat, Distribution *dist);        //!< Constructor with variable and the distribution
	StochasticNode(StochasticNode &d);                          //!< Copy constructor
	~StochasticNode(void);                                      //!<

	Distribution* getDistribution(void) { return distribution; }
    bool isChanged(void) { return true; }     //TODO not sure if that is meaningful
	void print(void);                         //TODO same as for print in DAGNode

private:
	Distribution* distribution;
};

#endif
