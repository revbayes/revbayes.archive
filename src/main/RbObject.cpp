
#include <string>
#include <ostream>
#include "RbObject.h"


RbObject::RbObject(void) {

}  //!< default constructor

RbObject::~RbObject() {

}  //! Virtual destructor because of virtual functions


std::string RbObject::getName(void) {
	return name;
}

void RbObject::setName(std::string n) {
	name = n;
}

std::string RbObject::toString(void){
	return name;
}

void RbObject::print(std::ostream &c) {
	c<<toString();
} //!< Print this object
