/**
 * @file Serializer.h
 *
 *
 * @brief Declaration of Serializer. This class clones objects by either calling clone, if the
 * object is derived from Cloneable, or calling the copy constructor. Hence, the Serializer can be
 * used in templates when it is unknown if either the copy constructor (basic classes)
 * or the clone function (abstract classes) exists.
 *
 * (c) Copyright 2009-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Serializer_H
#define Serializer_H

#include <string>

#include "RbException.h"

namespace RevBayesCore {
    
    template <typename objType, int>
    // general case: T is not derived from Cloneable
    // calls copy constructor
    class Serializer {
        
    public:
        //!< Create a clone of the given object.
        static objType*                 ressurectFromString( const std::string &s ) { throw RbException("Could not serialize object."); }
    };
    
    template <typename objType>
    // T is derived from Cloneable
    // calls clone
    class Serializer<objType,1> {
        
    public:
        //!< Create a clone of the given object.
        static objType*                 ressurectFromString( const std::string &s ) { objType *val = new objType(); val->initFromString(s); return val; }
    };
    
    template <>
    // T is derived from Cloneable
    // calls clone
    class Serializer<double,0> {
        
    public:
        //!< Create a clone of the given object.
        static double*                  ressurectFromString( const std::string &s ) { double *val = new double(atof( s.c_str())); return val; }
    };
    
    template <>
    // T is derived from Cloneable
    // calls clone
    class Serializer<int,0> {
        
    public:
        //!< Create a clone of the given object.
        static int*                  ressurectFromString( const std::string &s ) { int *val = new int(atoi( s.c_str())); return val; }
    };
    
}



#endif

