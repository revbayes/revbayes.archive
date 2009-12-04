/**
 * @file
 * This file contains the partial implementation of RbComplex,
 * which is the RevBayes abstract base class for complex objects.
 *
 * @brief Partial implementation of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#include <map>
#include <string>
#include <ostream>
#include <iostream>
#include <vector>

#include "RbComplex.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbObject.h"
#include "StringVector.h"



const StringVector RbComplex::rbClass = StringVector("Complex") + RbObject::rbClass;

/**
 * @brief Constructor
 *
 * The constructor uses the global workspace (the base environment)
 * to initialize the member variables and retrieve a reference to the
 * function table.
 *
 */
RbComplex::RbComplex() : RbObject() {

}


/**
 * @brief Destructor
 *
 * The destructor deletes the objects that may be stored
 * in the object slots of the members map.
 *
 */
RbComplex::~RbComplex() {

    members.clear();
    type.clear();
//    methodTable.clear();
}


/**
 * @brief Get method
 *
 * This function gets the named method. It simply asks the global
 * workspace for the method.
 *
 * @param name  The name of the method
 * @returns     A pointer to the method
 *
 */
//const RbFunction* RbComplex::getMethod(const std::string& name) const {
//
//	std::map<std::string, RbFunction*>::const_iterator it = methods.find( name );
//	if ( it != methods.end() )
//		return it->second;
//	return NULL;
//}

/**
 * @brief Print object info
 *
 * Print object info to ostream, including type, member variables
 * and their values, and member functions.
 *
 * @param o The ostream for printing
 *
 */
void RbComplex::print(std::ostream& o) const {

    for (std::map<std::string, RbObject*>::const_iterator i=members.begin(); i!=members.end(); i++) {
        o << "." << i->first << " = ";
        o << getMember(i->first)->briefInfo();
    }

//    std::map<std::string, RbFunction*> methods = getMethods();
//
//    for (std::map<std::string, RbFunction*>::iterator i=methods.begin(); i!=methods.end(); i++) {
//    	i->second->print(o);
//    }
}


/**
 * @brief Print object value
 *
 * Print object value to ostream. This includes object class and
 * values of member variables.
 *
 * @param o The ostream for printing
 *
 */
void RbComplex::printValue(std::ostream& o) const {

	o << "Complex object of type " << getClass()[0] << std::endl;

    for (std::map<std::string, RbObject*>::const_iterator i=members.begin(); i!=members.end(); i++) {
        o << "." << i->first << " = ";
        o << getMember(i->first)->briefInfo();
        o << std::endl;
    }
}


/**
 * @brief Set member variable
 *
 * This function sets the value of a member variable.
 *
 * @param name  The name of the variable
 * @param val   The new value to be used for the variable
 * @throws      Throws exception if member variable not found
 *
 */
void RbComplex::setMember(const std::string& name, const std::string& t, RbObject* val) {

	std::map<std::string, RbObject*>::iterator it = members.find( name );
	if ( it != members.end() )
		{
		members.insert( std::make_pair(name,val) );
        type.insert( std::make_pair(name,t) );
		}
	else 
		{
	    std::map<std::string, std::string>::iterator t = type.find( name );
	    if (val->isType(t->second)) {
	        RbObject* temp = it->second;
	        delete temp;
	        it->second = val;
	    }
	    else {
	        RbException e("Cannot set " + name + " because type " + t->second + " was expected but got " + val->getClass().toString());
	        throw e;
	    }

		}
}

bool RbComplex::addMember(const std::string& name, const std::string& t, RbObject* v) {

	std::map<std::string, RbObject*>::iterator it = members.find( name );
	if ( it != members.end() )
		{
		members.insert( std::make_pair(name,v) );
        type.insert( std::make_pair(name,t) );
		}
	else 
		{
		RbException e("Error in adding member. Member \"" + name + "\" already exists.");
		throw(e);
		}
}

void RbComplex::deleteMember(const std::string& name) {

	members.erase(name);
	type.erase(name);
}

const RbObject* RbComplex::getMember(const std::string& name) const {

	std::map<std::string, RbObject*>::const_iterator it = members.find( name );
	if ( it != members.end() )
		return it->second;
	return NULL;
}

