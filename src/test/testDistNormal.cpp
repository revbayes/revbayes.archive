#include <iostream>

#include "../distributions/Distribution.h"
#include "../distributions/DistNormal.h"
#include "../datatypes/RbDataType.h"
#include "../datatypes/primary/RbDouble.h"

int main(int argc, char **argv) {
	// create a DistNormal object
	DistNormal* dist = new DistNormal();

	RbDouble* sigma = new RbDouble(1.0);
	RbDouble* mu = new RbDouble(0.0);
	std::vector<RbDataType*> arguments(2);
	arguments[0] = mu;
	arguments[1] = sigma;


	RbDouble* x = new RbDouble(0.0);

	double pdf = dist->pdf(x, arguments);

	std::cout << "The pdf is " << pdf << '\n';
}
