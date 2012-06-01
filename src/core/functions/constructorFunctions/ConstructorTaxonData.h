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


template <typename valueType>
class ConstructorTaxonData :  public RbFunction {
    
public:

    ConstructorTaxonData(void);                                                                                             //!< Object constructor
    
    // Basic utility functions
    ConstructorTaxonData*                   clone(void) const;                                                              //!< Clone the object
    static const std::string&               getClassName(void);                                                             //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
    const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
    
protected:
    RbPtr<RbLanguageObject>                 executeFunction(const std::vector<const RbObject*>& args);                      //!< Execute the function. This is the function one has to overwrite for single return values.
    
};


#include "ArgumentRule.h"
#include "CharacterState.h"
#include "ConstructorTaxonData.h"
#include "DAGNode.h"
#include "MemberObject.h"
#include "RbFunction.h"
#include "RbString.h"
#include "RbUtil.h"
#include "RlCharacterState.h"
#include "RlTaxonData.h"
#include "TaxonData.h"
#include "TypeSpec.h"

#include <sstream>


/** Constructor */
template <typename valueType>
ConstructorTaxonData<valueType>::ConstructorTaxonData() : RbFunction() {
    
}


/** Clone the object */
template <typename valueType>
ConstructorTaxonData<valueType>* ConstructorTaxonData<valueType>::clone(void) const {
    
    return new ConstructorTaxonData(*this);
}


/** Execute function: we reset our template object here and give out a copy */
template <typename valueType>
RbPtr<RbLanguageObject> ConstructorTaxonData<valueType>::executeFunction(const std::vector<const RbObject *> &args) {
    
    // \TODO: Maybe we want to have specialized taxondata vectors?!
    TaxonData retVal = TaxonData( valueType::getClassName() );
    
    // the name of the taxon
    const std::string& n = static_cast<const RbString *>( args[0] )->getValue();
    retVal.setTaxonName( n );
    
    // set the vector of characters
    const RlVector<valueType>& v = static_cast<const RlVector<valueType> &>( *args[1] );
    for (size_t i = 0; i < v.size(); i++) {
        const CharacterState& c = v[i];
        retVal.addCharacter( c.clone() );
    }
    
    return RbPtr<RbLanguageObject>( new RlTaxonData( retVal ) );
}


/** Get argument rules */
template <typename valueType>
const ArgumentRules& ConstructorTaxonData<valueType>::getArgumentRules(void) const {
    
    static ArgumentRules argRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        argRules.push_back( new ArgumentRule( "name", true, RbString::getClassTypeSpec() ) );
        // \TODO: We should specificly expect elements of type character and not DAG node containers for which we cannot guarantee what is inside.
        argRules.push_back( new ArgumentRule( "x"   , true, TypeSpec( RlVector<valueType>::getClassTypeSpec(), new TypeSpec( RlCharacterState::getClassTypeSpec() ) ) ) );
        //       argRules.push_back( new ConstArgumentRule( "x"   , RbObject::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argRules;
    
}


/** Get class name of object */
template <typename valueType>
const std::string& ConstructorTaxonData<valueType>::getClassName(void) { 
    
    static std::string rbClassName = "Constructor function for taxon data(" + valueType::getClassName() + ")";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <typename valueType>
const TypeSpec& ConstructorTaxonData<valueType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
template <typename valueType>
const TypeSpec& ConstructorTaxonData<valueType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valueType>
const TypeSpec& ConstructorTaxonData<valueType>::getReturnType(void) const {
    
    return RlTaxonData::getClassTypeSpec();
}


#endif

