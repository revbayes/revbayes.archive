/*
 * RbFunction_tree.cpp
 *
 *  Created on: 17 sep 2009
 *      Author: Sebastian
 */

#include "RbFunction_tree.h"
#include "../../datatypes/RbDataType.h"
#include "../../datatypes/model/Topology.h"
#include "../../datatypes/model/Node.h"
#include "../../datatypes/model/Branch.h"
#include "../../datatypes/complex/RbVector.h"


/** Define the argument rules */
const ArgumentRule RbFunction_tree::argRules[] = {

    ArgumentRule("topology", Topology::dataType),
    ArgumentRule("branches", Branch::dataType),            // should be a vector of branches
    ArgumentRule("nodeParameter", RbVector::dataType),     // should be a vector of a vector of RbDataType
    ArgumentRule("branchParameter", RbVector::dataType),   // should be a vector of a vector of RbDataType
    ArgumentRule()
};

/** Add to symbol table */
static bool function_tree = SymbolTable::globalTable().add("tree", new RbFunction_tree());

RbFunction_tree::RbFunction_tree() {
	// TODO Auto-generated constructor stub

}

RbFunction_tree::~RbFunction_tree() {
	// TODO Auto-generated destructor stub
}

void RbFunction_tree::execute() {

}
