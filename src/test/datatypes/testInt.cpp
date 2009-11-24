
#include <iostream>

#include "RbInt.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "RbException.h"
#include <assert.h>

int main(int argc, char **argv) {
	// create a RbDouble object
	RbInt i(0);

	//print
	std::cout << "print int" << '\n';
	i.print(std::cout);

	//set a new value
	i.setValue(25);
	assert (i.getValue() == 25);

	RbObject* cp = i.clone();
	std::cout << '\n' << "print i" << '\n';
	i.print(std::cout);
	std::cout << '\n' << "print cp" << '\n';
	cp->print(std::cout);
	// check the return type
	assert (typeid(*cp) == typeid(RbInt));
	// check if they are actually equal
	assert (i.equals(cp));
	// check that they are different instances
	assert (&i != cp);

	// change the value
	i.setValue(42);
	// check if they are still equal
	assert (!(i.equals(cp)));


	RbDouble d(42);
	//should be possible to compare these two
	assert (!(typeid(d) == typeid(RbInt)));
	assert (i.equals(&d));
	d.print(std::cout);

	RbInt a(2);
	RbInt b(3);
	RbInt c(5);
	RbInt r = a + b;
	assert(c == r);

	std::cout << '\n' << "done" << '\n';
}
