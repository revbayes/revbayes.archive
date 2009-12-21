
#include <iostream>
#include <assert.h>

#include "ConstantNode.h"
#include "DAGNode.h"
#include "RbDouble.h"
#include "RbFunction_sqrt.h"
#include "RbObject.h"

int main(int argc, char **argv) {

	// create a RbFunction_swrt object
	RbFunction_sqrt f;

	// set the argmuents
	RbDouble* x = new RbDouble(25);

	// execute the function
	ConstantNode* cn = new ConstantNode(x);
	Argument a = Argument("x", cn);
	std::vector<Argument> args;
	args.push_back( a );
	const RbObject* result = f.execute(args);

	// check the result
	assert ( result->equals(new RbDouble(5)) );

	return 0;

}
