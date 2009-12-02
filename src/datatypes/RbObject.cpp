#include "RbObject.h"
#include "StringVector.h"

const StringVector RbObject::rbClass = StringVector("object");

bool RbObject::isType(const std::string t) const {

	std::string sv = getClass().get(0);
	if (sv == t)
		return true;
	return false;
}

void RbObject::print(std::ostream& o) const {
    o << toString() << std::endl;
}
