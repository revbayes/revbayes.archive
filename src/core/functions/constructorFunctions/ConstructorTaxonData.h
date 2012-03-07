/**
 * @file
 * This file contains the declaration of ConstructorTaxonData, which is 
 * the constructor function for taxon data from a vector of characters and a name.
 *
 * @brief Declaration of ConstructorTaxonData
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: $
 */

#ifndef ConstructorTaxonData_H
#define ConstructorTaxonData_H

#include "RbFunction.h"
#include "TaxonData.h"


class ConstructorTaxonData :  public RbFunction {
    
public:

    ConstructorTaxonData(void);                                                                                             //!< Object constructor
//    ConstructorTaxonData( const ConstructorTaxonData& c );                                                                  //!< Object constructor
    virtual                                 ~ConstructorTaxonData();
    
    // overloaded operators
    
    // Basic utility functions
    ConstructorTaxonData*                   clone(void) const;                                                              //!< Clone the object
    static const std::string&               getClassName(void);                                                             //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
    const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
    
protected:
    const RbLanguageObject&                 executeFunction(void);                                                          //!< Execute function
    void                                    setArgumentVariable(const std::string& name, const Variable* var);
    
    RbConstVariablePtr                      name;
    RbConstVariablePtr                      chars;
    
    // member function return object
    TaxonData                               retVal;

};

#endif

