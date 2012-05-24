/**
 * @file
 * This file contains the implementation of ConstructorTaxonData, which is 
 * the constructor function for TaxonData from a name and vector of characters.
 *
 * @brief Implementation of ConstructorTaxonData
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id: $
 */

#include "ArgumentRule.h"
#include "CharacterState.h"
#include "ConstArgumentRule.h"
#include "ConstructorTaxonData.h"
#include "DAGNode.h"
#include "DagNodeContainer.h"
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
ConstructorTaxonData::ConstructorTaxonData() : RbFunction() {
    
}


/** Clone the object */
ConstructorTaxonData* ConstructorTaxonData::clone(void) const {
    
    return new ConstructorTaxonData(*this);
}


/** Execute function: we reset our template object here and give out a copy */
RbPtr<RbLanguageObject> ConstructorTaxonData::executeFunction(const std::vector<const RbObject *> &args) {
    
    // \TODO: Maybe we want to have specialized taxondata vectors?!
    TaxonData retVal;
    
    // the name of the taxon
    const std::string& n = static_cast<const RbString *>( args[0] )->getValue();
    retVal.setTaxonName( n );
    
    // set the vector of characters
    const RlVector<CharacterState, RlCharacterState>& v = static_cast<const RlVector<CharacterState, RlCharacterState> &>( *args[1] );
    for (size_t i = 0; i < v.size(); i++) {
        const CharacterState& c = v[i];
        retVal.addCharacter( c );
    }
    
    return RbPtr<RbLanguageObject>( new RlTaxonData( retVal ) );
}


/** Get argument rules */
const ArgumentRules& ConstructorTaxonData::getArgumentRules(void) const {
        
   static ArgumentRules argRules = MemberRules();
   static bool        rulesSet = false;
   
   if (!rulesSet) {
       
       argRules.push_back( new ConstArgumentRule( "name", RbString::getClassTypeSpec() ) );
       // \TODO: We should specificly expect elements of type character and not DAG node containers for which we cannot guarantee what is inside.
       argRules.push_back( new ConstArgumentRule( "x"   , TypeSpec( RlVector<CharacterState, RlCharacterState>::getClassTypeSpec(), new TypeSpec( RlCharacterState::getClassTypeSpec() ) ) ) );
//       argRules.push_back( new ConstArgumentRule( "x"   , RbObject::getClassTypeSpec() ) );
       
       rulesSet = true;
   }
   
   return argRules;
    
}


/** Get class name of object */
const std::string& ConstructorTaxonData::getClassName(void) { 
    
    static std::string rbClassName = "Constructor function for taxon data";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ConstructorTaxonData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ConstructorTaxonData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& ConstructorTaxonData::getReturnType(void) const {
    
    return RlTaxonData::getClassTypeSpec();
}



