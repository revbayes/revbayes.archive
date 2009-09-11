
#include <iostream>

#include "../datatypes/model/Branch.h"
#include "../datatypes/primary/RbDouble.h"

int main(int argc, char **argv) {
	// create a Branch object
	Branch* b = new Branch();
	b->setName("testBranch");

	//print branch
	cout << "print branch" << '\n';
	b->print(std::cout);

	//create a length parameter for the branch
	RbDouble* brlen = new RbDouble(1.0);
	brlen->setName("length");

	//add the branch length to the branch
	b->addParameter(brlen);

	//print branch
	cout << "print branch with branch length parameter" << '\n';
	b->print(std::cout);

	//remove the branch length parameter
	b->removeParameter(brlen);

	//print branch
	cout << "print branch with removed branch length parameter" << '\n';
	b->print(std::cout);
}
