
#include <iostream>

#include "../../datatypes/primitive/RbBool.h"
#include "../../main/RbObject.h"

int main(int argc, char **argv) {
	// create a RbBool object
	RbBool b(true);
	b.setName("testBool");

	//print
	std::cout << "print bool" << '\n';
	b.print(std::cout);

	//set a new value
	b.setValue(false);
	assert (b.getValue() == false);

	RbObject* cp = b.clone();
	std::cout << '\n' << "print b" << '\n';
	b.print(std::cout);
	std::cout << '\n' << "print cp" << '\n';
	cp->print(std::cout);
	// check the return type
	assert (typeid(*cp) == typeid(RbBool));
	// check if they are actually equal
	assert (b == (*cp));
	// check that they are different instances
	assert (&b != cp);

	// change the value
	b.setValue(true);
	// check if they are still equal
	assert (!(b == *cp));

	return 0;

}
