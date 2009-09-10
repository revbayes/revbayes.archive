/**
 * @file
 * This file contains the declaration of DeterministicNode, which is a derived
 * class from DAGNode. DeterministicNode are DAG nodes with an expression assigned to its value.
 *
 * @brief Declaration of DeterministicNode
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

#ifndef DeterministicNode_H
#define DeterministicNode_H

#include <string>
#include <vector>
#include "DAGNode.h"

class RbDataType;
class SyntaxElement;
class DeterministicNode : public DAGNode {

public:
	DeterministicNode();                                        //!< Default constructor TODO maybe we should not have a default constructor
	DeterministicNode(RbDataType* dt, SyntaxElement* e);        //!< Constructor with variable and root of syntax tree
	DeterministicNode(DeterministicNode &d);                    //!< Copy constructor
    ~DeterministicNode(void);                                   //!<

    bool isChanged(void) { return true; }     //TODO not sure if that is meaningful
	void print(void);                         //TODO same as for print in DAGNode

private:
	SyntaxElement* rootSyntaxTree;
};

#endif
