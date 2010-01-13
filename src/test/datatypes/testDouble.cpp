
#include <iostream>

#include "RbDouble.h"
#include "RbObject.h"
#include <assert.h>

int main(int argc, char **argv) {
	// create a RbDouble object
	RbDouble d(0.0);
	//d.setName("testDouble");

	//print
	std::cout << "print double" << '\n';
	d.print(std::cout);

	//set a new value
	d.setValue(25.3);
	assert (d.getValue() == 25.3);

	RbObject* cp = d.clone();
	std::cout << '\n' << "print d" << '\n';
	d.print(std::cout);
	std::cout << '\n' << "print cp" << '\n';
	cp->print(std::cout);
	// check the return type
	assert (typeid(*cp) == typeid(RbDouble));
	// check if they are actually equal
	assert (d.equals(cp));
	// check that they are different instances
	assert (&d != cp);

	// change the value
	d.setValue(4.5);
	// check if they are still equal
	assert (!(d.equals(cp)));

	return 0;

}
