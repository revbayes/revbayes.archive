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

#ifndef RlTaxonData_H
#define RlTaxonData_H

#include "TaxonData.h"
#include "RlModelVariableWrapper.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    template <class rlCharType>
    class TaxonData : public RlModelVariableWrapper<RevBayesCore::TaxonData<typename rlCharType::valueType> > {
        
    public:
        TaxonData(void);                                                                                                                    //!< Constructor requires character type
        TaxonData(RevBayesCore::TaxonData<typename rlCharType::valueType> *v);                                                          //!< Constructor requires character type
        TaxonData(const TaxonData& d);                                                                                                  //!< Constructor requires character type
        
        typedef RevBayesCore::TaxonData<typename rlCharType::valueType> valueType;
        
        // Basic utility functions
        TaxonData*                          clone(void) const;                                                                                  //!< Clone object
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
#include "Vector.h"


template <class rlCharType>
RevLanguage::TaxonData<rlCharType>::TaxonData(void) : RlModelVariableWrapper<RevBayesCore::TaxonData<typename rlCharType::valueType> >() {
    
}


template <class rlCharType>
RevLanguage::TaxonData<rlCharType>::TaxonData( RevBayesCore::TaxonData<typename rlCharType::valueType> *v) : RlModelVariableWrapper<RevBayesCore::TaxonData<typename rlCharType::valueType> >( v ) {
    
}


template <class rlCharType>
RevLanguage::TaxonData<rlCharType>::TaxonData( const TaxonData<rlCharType> &v) : RlModelVariableWrapper<RevBayesCore::TaxonData<typename rlCharType::valueType> >( v ) {
    
}



template <typename charType>
RevLanguage::TaxonData<charType>* RevLanguage::TaxonData<charType>::clone() const {
    return new TaxonData<charType>( *this );
}


/* Map calls to member methods */
template <typename charType>
RevLanguage::RbLanguageObject* RevLanguage::TaxonData<charType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if ( name == "[]") {
        // get the member with give index
        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getValue() );
            
        if (this->value->getValue().size() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in []");
        }
            
        RbLanguageObject* element = new charType( this->value->getValue().getElement(index.getValue() - 1) );
        return element;
    } 
    
    return RlModelVariableWrapper<RevBayesCore::TaxonData<typename charType::valueType> >::executeMethod( name, args );
}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::TaxonData<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "TaxonData";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::TaxonData<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename rlType>
const RevLanguage::MethodTable& RevLanguage::TaxonData<rlType>::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
        
        
        // necessary call for proper inheritance
        methods.setParentTable( &RbLanguageObject::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::TaxonData<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



#endif

