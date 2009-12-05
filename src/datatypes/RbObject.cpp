#include "RbObject.h"
#include "StringVector.h"

const StringVector RbObject::rbClass = StringVector("object");

/** Get type (first entry in class vector) */
std::string RbObject::getType() const {

    return getClass()[0];
}

/** Is object of specified type? We need to check entire class vector in case it is derived from type. */
bool RbObject::isType(const std::string t) const {

    StringVector classVec = getClass();
    for (int i=0; i<classVec.size(); i++) {
        if (t == classVec[i])
            return true;
    }

	return false;
}

/** Print info about project simply by using toString function */
void RbObject::print(std::ostream& o) const {

    o << toString() << std::endl;
}

