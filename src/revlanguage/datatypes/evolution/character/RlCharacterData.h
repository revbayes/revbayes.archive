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
#include "RlModelVariableWrapper.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    

    template <class rlCharType>
    class CharacterData : public RlModelVariableWrapper<RevBayesCore::CharacterData<typename rlCharType::valueType> > {
    
    public:
        CharacterData(void);                                                                                                                    //!< Constructor requires character type
        CharacterData(RevBayesCore::CharacterData<typename rlCharType::valueType> *v);                                                          //!< Constructor requires character type
        CharacterData(RevBayesCore::TypedDagNode<RevBayesCore::CharacterData<typename rlCharType::valueType> >*d);                              //!< Constructor requires character type
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
RevLanguage::CharacterData<rlCharType>::CharacterData(void) : RlModelVariableWrapper<RevBayesCore::CharacterData<typename rlCharType::valueType> >() {
    
}


template <class rlCharType>
RevLanguage::CharacterData<rlCharType>::CharacterData( RevBayesCore::CharacterData<typename rlCharType::valueType> *v) : RlModelVariableWrapper<RevBayesCore::CharacterData<typename rlCharType::valueType> >( v ) {
    
}


template <class rlCharType>
RevLanguage::CharacterData<rlCharType>::CharacterData( RevBayesCore::TypedDagNode<RevBayesCore::CharacterData<typename rlCharType::valueType> > *d) : RlModelVariableWrapper<RevBayesCore::CharacterData<typename rlCharType::valueType> >( d ) {
    
}


template <class rlCharType>
RevLanguage::CharacterData<rlCharType>::CharacterData( const CharacterData<rlCharType> &v) : RlModelVariableWrapper<RevBayesCore::CharacterData<typename rlCharType::valueType> >( v ) {
    
}



template <typename charType>
RevLanguage::CharacterData<charType>* RevLanguage::CharacterData<charType>::clone() const {
    return new CharacterData<charType>( *this );
}


/* Map calls to member methods */
template <typename charType>
RevLanguage::RbLanguageObject* RevLanguage::CharacterData<charType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "names") {
        Vector<RlString> *n = new Vector<RlString>();
        for (size_t i = 0; i < this->value->getValue().getNumberOfTaxa(); ++i) {
            n->push_back( this->value->getValue().getTaxonNameWithIndex( i ) );
        }
        return n;
    }
    else if (name == "[]") {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getValue() );
        
        if (this->value->getValue().getNumberOfTaxa() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in []");
        }
        
        const RevBayesCore::TaxonData<typename charType::valueType>& element = this->value->getValue().getTaxonData(index.getValue() - 1);
        return new TaxonData<charType>( new RevBayesCore::TaxonData<typename charType::valueType>( element ) );
    }
    else if (name == "size") {
        int n = (int)this->value->getValue().getNumberOfTaxa();
        return new Natural(n);
    }
    else if (name == "ntaxa") {
        int n = (int)this->value->getValue().getNumberOfTaxa();
        return new Natural(n);
    }
    else if (name == "nchar") {
        
        Vector<Natural> *numChar = new Vector<Natural>();
        for (size_t i=0; i<this->value->getValue().getNumberOfTaxa(); i++) {
            if ( this->value->getValue().isTaxonExcluded(i) == false ) {
                if (this->value->getValue().getIsHomologyEstablished() == true)
                    numChar->push_back( Natural( this->value->getValue().getNumberOfCharacters() ) );
                else
                    numChar->push_back( Natural( this->value->getValue().getTaxonData(i).getNumberOfCharacters() ) );
            }
        }
        return numChar;
    }
//    else if (name == "chartype") {
//        return new RlString( this->value->getValue().getDataType() );
//    }
    //    else if (name == "nexcludedtaxa")
    //    {
    //        int n = (int)deletedTaxa.size();
    //        return RbPtr<RbLanguageObject>( new Natural(n) );
    //    }
    //    else if (name == "nexcludedchars")
    //    {
    //        int n = (int)deletedCharacters.size();
    //        return RbPtr<RbLanguageObject>( new Natural(n) );
    //    }
    //    else if (name == "nincludedtaxa")
    //    {
    //        int n = (int)(data.getNumberOfTaxa() - deletedTaxa.size());
    //        return RbPtr<RbLanguageObject>( new Natural(n) );
    //    }
    //    else if (name == "nincludedchars")
    //    {
    //        int n = (int)(data.getNumberOfCharacters() - deletedCharacters.size());
    //        return RbPtr<RbLanguageObject>( new Natural(n) );
    //    }
    //    else if (name == "excludedtaxa")
    //    {
    //        RlVector<std::string, RbString> *excludedTaxa = new RlVector<std::string, RbString>();
    //        for (std::set<size_t>::iterator it = data.deletedTaxa.begin(); it != deletedTaxa.end(); it++)
    //        {
    //            const std::string& tn = getTaxonNameWithIndex(*it);
    //            excludedTaxa->push_back( new RbString( tn ) );
    //        }
    //        return excludedTaxa;
    //    }
    //    else if (name == "excludedchars")
    //    {
    //        RlVector *excludedChars = new RlVector(Natural::getClassTypeSpec());
    //        for (std::set<size_t>::iterator it = deletedCharacters.begin(); it != deletedCharacters.end(); it++)
    //            excludedChars->push_back( new Natural(*it) );
    //        return excludedChars;
    //    }
    //    else if (name == "includedtaxa")
    //    {
    //        RlVector *includedTaxa = new RlVector(RbString::getClassTypeSpec());
    //        for (size_t i=0; i<getNumberOfTaxa(); i++)
    //        {
    //            if ( isTaxonExcluded(i) == false )
    //                includedTaxa->push_back( new RbString( getTaxonNameWithIndex(i) ) );
    //        }
    //        return includedTaxa;
    //    }
    //    else if (name == "includedchars")
    //    {
    //        RlVector *includedChars = new RlVector(Natural::getClassTypeSpec());
    //        for (size_t i=0; i<getNumberOfCharacters(); i++)
    //        {
    //            if ( isCharacterExcluded(i) == false )
    //                includedChars->push_back( new Natural(i+1) );
    //        }
    //        return includedChars;
    //    }
    //    else if (name == "nconstantpatterns")
    //    {
    //        int n = (int)numConstantPatterns();
    //        return RbPtr<RbLanguageObject>( new Natural(n) );
    //    }
    //    else if (name == "ncharswithambiguity")
    //    {
    //        int n = (int)numMissAmbig();
    //        return RbPtr<RbLanguageObject>( new Natural(n) );
    //    }
    //    else if (name == "excludechar")
    //    {
    //        const RbObject& argument = *args[0];
    //        if ( argument.isTypeSpec( Natural::getClassTypeSpec() ) ) 
    //        {
    //            int n = static_cast<const Natural&>( argument ).getValue();
    //            deletedCharacters.insert( n );
    //        }
    //        else if ( argument.isTypeSpec( RlVector::getClassTypeSpec() ) ) 
    //        {
    //            const RlVector& x = static_cast<const RlVector&>( argument );
    //            for ( size_t i=0; i<x.size(); i++ )
    //                deletedCharacters.insert( static_cast<const Natural &>( x[i] ).getValue() );
    //        }
    //        return NULL;
    //    }
    else if (name == "show") {
        int nt = (int)this->value->getValue().getNumberOfTaxa();
        for (int i=0; i<nt; i++) {
            const RevBayesCore::TaxonData<typename charType::valueType>& taxonData = this->value->getValue().getTaxonData(i);
            std::string taxonName = this->value->getValue().getTaxonNameWithIndex(i);
            int nc = (int)taxonData.getNumberOfCharacters();
            std::cout << "   " << taxonName << std::endl;
            std::cout << "   ";
            for (int j=0; j<nc; j++) {
                const RevBayesCore::CharacterState& o = taxonData[j];
                std::string s = o.getStringValue();
                
                std::cout << s;
                if ( (j+1) % 100 == 0 && (j+1) != nc )
                    std::cout << std::endl << "   ";
            }
            std::cout << std::endl;
        }
        return NULL;
    }
    else if (name == "ishomologous") {
        bool ih = this->value->getValue().getIsHomologyEstablished();
        return new RlBoolean(ih);
    } 
    
    return RlModelVariableWrapper<RevBayesCore::CharacterData<typename charType::valueType> >::executeMethod( name, args );
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
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename rlType>
const RevLanguage::MethodTable& RevLanguage::CharacterData<rlType>::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static ArgumentRules* ncharArgRules               = new ArgumentRules();
    static ArgumentRules* namesArgRules               = new ArgumentRules();
    static ArgumentRules* ntaxaArgRules               = new ArgumentRules();
    static ArgumentRules* chartypeArgRules            = new ArgumentRules();    
    static ArgumentRules* nexcludedtaxaArgRules       = new ArgumentRules();    
    static ArgumentRules* nexcludedcharsArgRules      = new ArgumentRules();    
    static ArgumentRules* nincludedtaxaArgRules       = new ArgumentRules();    
    static ArgumentRules* nincludedcharsArgRules      = new ArgumentRules();    
    static ArgumentRules* excludedtaxaArgRules        = new ArgumentRules();    
    static ArgumentRules* excludedcharsArgRules       = new ArgumentRules();    
    static ArgumentRules* includedtaxaArgRules        = new ArgumentRules();    
    static ArgumentRules* includedcharsArgRules       = new ArgumentRules();    
    static ArgumentRules* nconstantpatternsArgRules   = new ArgumentRules();    
    static ArgumentRules* ncharswithambiguityArgRules = new ArgumentRules();
    static ArgumentRules* excludecharArgRules         = new ArgumentRules();
    static ArgumentRules* excludecharArgRules2        = new ArgumentRules();
    static ArgumentRules* showdataArgRules            = new ArgumentRules();
    static ArgumentRules* ishomologousArgRules        = new ArgumentRules();    
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        excludecharArgRules->push_back(        new ArgumentRule(     "", true, Natural::getClassTypeSpec()       ) );
        excludecharArgRules2->push_back(       new ArgumentRule(     "", true, TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ) ) );
        
        methods.addFunction("names",               new MemberFunction(TypeSpec(Vector<RlString>::getClassTypeSpec(), new TypeSpec( RlString::getClassTypeSpec() ) ),  namesArgRules              ) );
        methods.addFunction("nchar",               new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), ncharArgRules              ) );
        methods.addFunction("ntaxa",               new MemberFunction(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
        methods.addFunction("chartype",            new MemberFunction(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
        methods.addFunction("nexcludedtaxa",       new MemberFunction(Natural::getClassTypeSpec(),       nexcludedtaxaArgRules      ) );
        methods.addFunction("nexcludedchars",      new MemberFunction(Natural::getClassTypeSpec(),       nexcludedcharsArgRules     ) );
        methods.addFunction("nincludedtaxa",       new MemberFunction(Natural::getClassTypeSpec(),       nincludedtaxaArgRules      ) );
        methods.addFunction("nincludedchars",      new MemberFunction(Natural::getClassTypeSpec(),       nincludedcharsArgRules     ) );
        methods.addFunction("excludedtaxa",        new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedtaxaArgRules       ) );
        methods.addFunction("excludedchars",       new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedcharsArgRules      ) );
        methods.addFunction("includedtaxa",        new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedtaxaArgRules       ) );
        methods.addFunction("includedchars",       new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedcharsArgRules      ) );
        methods.addFunction("nconstantpatterns",   new MemberFunction(Natural::getClassTypeSpec(),       nconstantpatternsArgRules  ) );
        methods.addFunction("ncharswithambiguity", new MemberFunction(Natural::getClassTypeSpec(),       ncharswithambiguityArgRules) );
        methods.addFunction("excludechar",         new MemberFunction(RbVoid_name,        excludecharArgRules        ) );
        methods.addFunction("excludechar",         new MemberFunction(RbVoid_name,        excludecharArgRules2       ) );
        methods.addFunction("show",                new MemberFunction(RbVoid_name,        showdataArgRules           ) );
        methods.addFunction("ishomologous",        new MemberFunction(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( TaxonData<rlType>::getClassTypeSpec(), squareBracketArgRules) );
        
        
        // add method for call "x[]" as a function
        ArgumentRules* sizeArgRules = new ArgumentRules();
        methods.addFunction("size",  new MemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RlModelVariableWrapper<RevBayesCore::CharacterData<typename rlType::valueType> >::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::CharacterData<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



#endif

