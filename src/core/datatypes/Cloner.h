/**
 * @file Cloner.h
 *
 *
 * @brief Declaration of Cloner. This class clones objects by either calling clone, if the
 * object is derived from Cloneable, or calling the copy constructor. Hence, the cloner can be
 * used in templates when it is unknown if either the copy constructor (basic classes) 
 * or the clone function (abstract classes) exists.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-03-07 15:57:47 +0100 (Wed, 07 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-05-09, version 1.0
 *
 * $Id: RbPtr.h 1325 2012-03-07 14:57:47Z hoehna $
 */

#ifndef Cloner_H
#define Cloner_H

#include <string>

namespace RevBayesCore {
    
    template <typename objType, int>
    // general case: T is not derived from Cloneable
    // calls copy constructor
    class Cloner {
        
    public:
        //!< Create a clone of the given object.
        static objType*                 createClone( const objType &o ) { return new objType( o ); } 
    };
    
    template <typename objType>
    // T is derived from Cloneable
    // calls clone
    class Cloner<objType,1> {
        
    public:
        //!< Create a clone of the given object.
        static objType*                 createClone( const objType &o ) { return o.clone(); } 
    };
    
}



#endif

