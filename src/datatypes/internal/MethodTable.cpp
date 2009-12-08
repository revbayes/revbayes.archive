/**
 * @file
 * This file contains the implementation of MethodTable, which is
 * used to hold descriptions of member functions (methods) of
 * complex language objects. It is not a true function table,
 * it just holds the function descriptions for the parser to use
 * in printing descriptions of complex language objects and in
 * providing info about member methods.
 *
 * @brief Implementation of MethodTable
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: MethodTable.h 63 2009-11-18 00:05:57Z ronquist $
 */


#include "FunctionTable.h"
#include "MethodDescr.h"
#include "MethodTable.h"
#include "RbException.h"

#include <sstream>


/** Add method description to table */
void MethodTable::addMethodDescr(const std::string name, const std::string& retType, const ArgumentRule** argRules) {

    const MethodDescr theDescr = MethodDescr(retType, argRules);

    std::pair<std::multimap<const std::string, const MethodDescr>::const_iterator,
              std::multimap<const std::string, const MethodDescr>::const_iterator> retVal;
    retVal = methodTable.equal_range(name);

    for (std::multimap<const std::string, const MethodDescr>::const_iterator i=retVal.first; i!=retVal.second; i++) {
        if (!FunctionTable::isDistinctFormal((*i).second.getArgRules(), argRules)) {
            std::ostringstream  msg;
            (*i).second.printValue(msg);
            msg << " cannot overload ";
            theDescr.printValue(msg);
            throw (RbException(msg.str()));
        }
    }

    methodTable.insert(std::pair<const std::string, const MethodDescr>(name, theDescr));
}


/** Print description of named member function(s) */
void MethodTable::printValue(std::ostream& o, const std::string& name) const {

    std::pair<std::multimap<const std::string, const MethodDescr>::const_iterator,
              std::multimap<const std::string, const MethodDescr>::const_iterator> retVal;
    retVal = methodTable.equal_range(name);

    for (std::multimap<const std::string, const MethodDescr>::const_iterator i=retVal.first; i!=retVal.second; i++) {
        (*i).second.printValue(o);
        o << std::endl;
   }
}


/** Print description of member functions */
void MethodTable::printValue(std::ostream& o) const {

    for (std::multimap<const std::string, const MethodDescr>::const_iterator i=methodTable.begin(); i!=methodTable.end(); i++) {
        o << (*i).first << " = ";
        (*i).second.printValue(o);
        o << std::endl;
   }
}


/** Complete info about object */
std::string MethodTable::toString() const {

    std::ostringstream o;
    if (methodTable.size() == 0)
        o << "MethodTable:" << std::endl << "<empty>";
    else {
        o << "MethodTable:" << std::endl;
        printValue(o);
    }

    return o.str();
}


