/**
 * @file
 * This file contains the declaration of ConstructorSet, which is 
 * the constructor function for taxon data from a vector of characters and a name.
 *
 * @brief Declaration of ConstructorSet
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: $
 */

#ifndef ConstructorSet_H
#define ConstructorSet_H

#include "RbFunction.h"
#include "Set.h"


class ConstructorSet :  public RbFunction {
    
public:
    
    ConstructorSet(void);                                                                                             //!< Object constructor
    //    ConstructorSet( const ConstructorSet& c );                                                                  //!< Object constructor
    virtual                                 ~ConstructorSet();
    
    // overloaded operators
    
    // Basic utility functions
    ConstructorSet*                         clone(void) const;                                                              //!< Clone the object
    static const std::string&               getClassName(void);                                                             //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
    const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
    
protected:
    const RbLanguageObject&                 executeFunction(void);                                                          //!< Execute function
    void                                    setArgumentVariable(const std::string& name, const Variable* var);
    
    RbConstVariablePtr                      v;
    
    // member function return object
//    Set<>                               retVal;
    
};

#endif

