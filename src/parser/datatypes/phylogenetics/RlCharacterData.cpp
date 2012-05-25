/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Implementation of Alignment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package datatypes
 *
 * $Id$
 */

#include "CharacterState.h"
#include "CharacterData.h"
#include "ConstArgumentRule.h"
#include "DnaState.h"
#include "Ellipsis.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RlCharacterData.h"
#include "RlTaxonData.h"
#include "SimpleMemberFunction.h"
#include "StochasticNode.h"
#include "TaxonData.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/** Constructor requires character type; passes member rules to base class */
RlCharacterData::RlCharacterData( const std::string& charType ) : MemberObject( getMemberRules() ), 
typeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ), new TypeSpec(charType) ), data( charType )
{
    
}


/** Constructor requires character type; passes member rules to base class */
RlCharacterData::RlCharacterData( const CharacterData& d ) : MemberObject( getMemberRules() ), 
typeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ), new TypeSpec( d.getDataType() ) ), data( d )
{
    
}


/** Clone object */
RlCharacterData* RlCharacterData::clone(void) const {
    
    return new RlCharacterData(*this);
}



/** Map calls to member methods */
RbPtr<RbLanguageObject> RlCharacterData::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
    if (name == "names") 
    {
        RlVector<RbString> *n = new RlVector<RbString>();
        for (size_t i = 0; i < data.getNumberOfTaxa(); ++i) {
            n->push_back( data.getTaxonNameWithIndex( i ) );
        }
        return RbPtr<RbLanguageObject>( n );
    }
    else if (name == "[]") 
    {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( *args[0] );
        
        if (data.getNumberOfTaxa() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in []");
        }
        
        const TaxonData& element = data.getTaxonData(index.getValue() - 1);
        return RbPtr<RbLanguageObject>( new RlTaxonData( element ) );
    }
    else if (name == "size") 
    {
        int n = (int)data.getNumberOfTaxa();
        return RbPtr<RbLanguageObject>( new Natural(n) );
    }
    else if (name == "ntaxa") 
    {
        int n = (int)data.getNumberOfTaxa();
        return RbPtr<RbLanguageObject>( new Natural(n) );
    }
    else if (name == "nchar")
    {
        
        RlVector<Natural> *numChar = new RlVector<Natural>();
        for (size_t i=0; i<data.getNumberOfTaxa(); i++)
        {
            if ( data.isTaxonExcluded(i) == false )
            {
                if (data.getIsHomologyEstablished() == true)
                    numChar->push_back( new Natural( data.getNumberOfCharacters() ) );
                else
                    numChar->push_back( new Natural( data.getTaxonData(i).getNumberOfCharacters() ) );
            }
        }
        return numChar;
    }
//    else if (name == "chartype")
//    {
//        return characterType.clone();
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
    else if (name == "show")
    {
        int nt = (int)data.getNumberOfTaxa();
        for (int i=0; i<nt; i++)
        {
            const TaxonData& taxonData = data.getTaxonData(i);
            std::string taxonName = data.getTaxonNameWithIndex(i);
            int nc = (int)taxonData.getNumberOfCharacters();
            std::cout << "   " << taxonName << std::endl;
            std::cout << "   ";
            for (int j=0; j<nc; j++)
            {
                const CharacterState& o = taxonData[j];
                std::string s = o.getStringValue();
                
                std::cout << s;
                if ( (j+1) % 100 == 0 && (j+1) != nc )
                    std::cout << std::endl << "   ";
            }
            std::cout << std::endl;
        }
        return NULL;
    }
//    else if (name == "ishomologous")
//    {
//        bool ih = getIsHomologyEstablished();
//        return RbPtr<RbLanguageObject>( new RbBoolean(ih) );
//    }
    
    return MemberObject::executeSimpleMethod( name, args );
}



/** Get class name of object */
const std::string& RlCharacterData::getClassName(void) { 
    
    static std::string rbClassName = "Character data";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlCharacterData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Get member rules */
const MemberRules& RlCharacterData::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        memberRules.push_back( new ConstArgumentRule("type", RbString::getClassTypeSpec() ) );
        memberRules.push_back( new ConstArgumentRule("x", RlTaxonData::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis( RlTaxonData::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    
    return memberRules;
}


/** Get methods */
const MethodTable& RlCharacterData::getMethods(void) const {
    
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
    
    if ( methodsSet == false ) 
    {
        
        excludecharArgRules->push_back(        new ConstArgumentRule(     "", Natural::getClassTypeSpec()       ) );
        excludecharArgRules2->push_back(       new ConstArgumentRule(     "", TypeSpec(RlVector<int, Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ) ) );
        
        methods.addFunction("names",               new SimpleMemberFunction(TypeSpec(RlVector<std::string, RbString>::getClassTypeSpec(), new TypeSpec( RbString::getClassTypeSpec() ) ),  namesArgRules              ) );
        methods.addFunction("nchar",               new SimpleMemberFunction(TypeSpec(RlVector<int, Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), ncharArgRules              ) );
        methods.addFunction("ntaxa",               new SimpleMemberFunction(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
        methods.addFunction("chartype",            new SimpleMemberFunction(RbString::getClassTypeSpec(),      chartypeArgRules           ) );
        methods.addFunction("nexcludedtaxa",       new SimpleMemberFunction(Natural::getClassTypeSpec(),       nexcludedtaxaArgRules      ) );
        methods.addFunction("nexcludedchars",      new SimpleMemberFunction(Natural::getClassTypeSpec(),       nexcludedcharsArgRules     ) );
        methods.addFunction("nincludedtaxa",       new SimpleMemberFunction(Natural::getClassTypeSpec(),       nincludedtaxaArgRules      ) );
        methods.addFunction("nincludedchars",      new SimpleMemberFunction(Natural::getClassTypeSpec(),       nincludedcharsArgRules     ) );
        methods.addFunction("excludedtaxa",        new SimpleMemberFunction(TypeSpec(RlVector<int, Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedtaxaArgRules       ) );
        methods.addFunction("excludedchars",       new SimpleMemberFunction(TypeSpec(RlVector<int, Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedcharsArgRules      ) );
        methods.addFunction("includedtaxa",        new SimpleMemberFunction(TypeSpec(RlVector<int, Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedtaxaArgRules       ) );
        methods.addFunction("includedchars",       new SimpleMemberFunction(TypeSpec(RlVector<int, Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedcharsArgRules      ) );
        methods.addFunction("nconstantpatterns",   new SimpleMemberFunction(Natural::getClassTypeSpec(),       nconstantpatternsArgRules  ) );
        methods.addFunction("ncharswithambiguity", new SimpleMemberFunction(Natural::getClassTypeSpec(),       ncharswithambiguityArgRules) );
        methods.addFunction("excludechar",         new SimpleMemberFunction(RbVoid_name,        excludecharArgRules        ) );
        methods.addFunction("excludechar",         new SimpleMemberFunction(RbVoid_name,        excludecharArgRules2       ) );
        methods.addFunction("show",                new SimpleMemberFunction(RbVoid_name,        showdataArgRules           ) );
        methods.addFunction("ishomologous",        new SimpleMemberFunction(RbBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ConstArgumentRule( "index" , Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new SimpleMemberFunction( RlTaxonData::getClassTypeSpec(), squareBracketArgRules) );
        
        
        // add method for call "x[]" as a function
        ArgumentRules* sizeArgRules = new ArgumentRules();
        methods.addFunction("size",  new SimpleMemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RlCharacterData::getTypeSpec(void) const {
    
    return typeSpec;
}


/** Print value for user */
void RlCharacterData::printValue(std::ostream& o) const {
    
    o << "Origination:          " << data.getFileName() << std::endl;
    o << "Data type:            " << data.getDataType() << std::endl;
    o << "Number of taxa:       " << data.getNumberOfTaxa() << std::endl;
    o << "Number of characters: " << data.getNumberOfCharacters() << std::endl;
}


/** Catch setting of the mixture variable */
void RlCharacterData::setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var) {
    
    if ( name == "type" ) {
        //        elementType = TypeSpec( static_cast<RbString&>( var->getValue() ).getValue() );
    }
    else if ( name == "x" || name == "" ) {
        const TaxonData& obs = static_cast<const RlTaxonData *>( (const RbLanguageObject *) var )->getTaxonData();
        data.addTaxonData( obs );        
    }
    else {
        MemberObject::setSimpleMemberValue(name, var);
    }
}



