/**
 * @file
 * This file contains the declaration for tree function.
 * The tree function creates a tree out of a topology, which contains the set of nodes,
 * the branches, the parameters associated to the nodes and the parameters associated to the branches.
 *
 * @brief Declaration of the class FunctionTree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 * @interface RbFunction
 * @extends RbFunction
 *
 * $Id$
 */

#ifndef RBFUNCTION_TREE_H_
#define RBFUNCTION_TREE_H_

#include <set>
#include <vector>
#include "../AbstractFunction.h"
#include "../../main/RbParameter.h"
#include "../../datatypes/model/Tree.h"

class RbFunction_tree : AbstractFunction{
public:
	RbFunction_tree();
	virtual ~RbFunction_tree();

	//Tree* createTree(RbParameter* tau, std::set<RbParameter*> b, std::vector<std::vector<RbParameter*>> nodeParam, std::vector<std::vector<RbParameter*>> branchParam);
	RbDataType* execute();
};

#endif /* RBFUNCTION_TREE_H_ */
