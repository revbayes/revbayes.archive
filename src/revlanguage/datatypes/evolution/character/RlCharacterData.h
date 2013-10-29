/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of Alignment
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlCharacterData_H
#define RlCharacterData_H

#include "CharacterData.h"
#include "RlAbstractCharacterData.h"
#include "RlModelVariableWrapper.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    

    template <class rlCharType>
    class CharacterData : public AbstractCharacterData {
    
    public:
        CharacterData(void);                                                                                                                    //!< Constructor requires character type
        CharacterData(RevBayesCore::CharacterData<typename rlCharType::valueType> *v);                                                          //!< Constructor requires character type
        CharacterData(RevBayesCore::TypedDagNode< RevBayesCore::AbstractCharacterData >*d);                              //!< Constructor requires character type
        CharacterData(const CharacterData& d);                                                                                                  //!< Constructor requires character type
    
        typedef RevBayesCore::CharacterData<typename rlCharType::valueType> valueType;
    
        // Basic utility functions
        CharacterData*                      clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
         
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RbLanguageObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
            
    };
    
}


#include "ArgumentRule.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTaxonData.h"
#include "Vector.h"


template <class rlCharType>
RevLanguage::CharacterData<rlCharType>::CharacterData(void) : AbstractCharacterData() {
    
}


template <class rlCharType>
RevLanguage::CharacterData<rlCharType>::CharacterData( RevBayesCore::CharacterData<typename rlCharType::valueType> *v) : AbstractCharacterData( v ) {
    
}


template <class rlCharType>
RevLanguage::CharacterData<rlCharType>::CharacterData( RevBayesCore::TypedDagNode< RevBayesCore::AbstractCharacterData > *d) : AbstractCharacterData( d ) {

}


template <class rlCharType>
RevLanguage::CharacterData<rlCharType>::CharacterData( const CharacterData<rlCharType> &v) : AbstractCharacterData( v ) {
    
}



template <typename charType>
RevLanguage::CharacterData<charType>* RevLanguage::CharacterData<charType>::clone() const {
    
    return new CharacterData<charType>( *this );
}


/* Map calls to member methods */
template <typename charType>
RevLanguage::RbLanguageObject* RevLanguage::CharacterData<charType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "[]") 
    {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getValue() );
            
        if (this->value->getValue().getNumberOfTaxa() < (size_t)(index.getValue()) ) 
        {
            throw RbException("Index out of bounds in []");
        }
            
        const RevBayesCore::TaxonData<typename charType::valueType>& element = static_cast< RevBayesCore::CharacterData<typename charType::valueType>& >( this->value->getValue() ).getTaxonData(index.getValue() - 1);
    
        return new TaxonData<charType>( new RevBayesCore::TaxonData<typename charType::valueType>( element ) );
    }
    
    return AbstractCharacterData::executeMethod( name, args );
}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::CharacterData<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "CharacterData";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::CharacterData<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( AbstractCharacterData::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename rlType>
const RevLanguage::MethodTable& RevLanguage::CharacterData<rlType>::getMethods(void) const {
    
    static MethodTable    myMethods                   = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) 
    {
                
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        myMethods.addFunction("[]",  new MemberFunction( TaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );
        
                
        // necessary call for proper inheritance
        myMethods.setParentTable( &AbstractCharacterData::getMethods() );
        methodsSet = true;
    }
    
    
    return myMethods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::CharacterData<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



#endif

