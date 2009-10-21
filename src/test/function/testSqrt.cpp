
#include <iostream>

#include "RbDouble.h"
#include "RbFunction_sqrt.h"
#include "RbObject.h"

int main(int argc, char **argv) {
	// create a RbFunction_swrt object
	RbFunction_sqrt f;

	// set the argmuents
	RbDouble x(25);

	// execute the function
	RbDataType* result = f.execute();

	// check the result
	assert (*result == RbDouble(5));

	return 0;

}
