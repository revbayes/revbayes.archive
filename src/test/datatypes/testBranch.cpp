
#include <iostream>

#include "../../datatypes/model/Branch.h"
#include "../../datatypes/primitive/RbDouble.h"

int main(int argc, char **argv) {
	// create a Branch object
	Branch b;
	std::string s="testBranch";
	b.setName(s);

	//print branch
	cout << "print branch" << '\n';
	b.print(std::cout);

	//create a length parameter for the branch
	RbDouble brlen(1.0);
	brlen.setName("length");

	//add the branch length to the branch
	assert(b.getNumberOfParameter()==0);
	b.addParameter(&brlen);
	assert(b.getNumberOfParameter()==1);

	//print branch
	cout << "print branch with branch length parameter" << '\n';
	b.print(std::cout);

	//remove the branch length parameter
	assert(b.getNumberOfParameter()==1);
	b.removeParameter(&brlen);
	assert(b.getNumberOfParameter()==0);

	//print branch
	cout << "print branch with removed branch length parameter" << '\n';
	b.print(std::cout);

}
