
/**
 * @file
 * This file contains the implementation of ConstructorCtmcModel, which is used
 * for functions that construct member objects.
 *
 * @brief Implementation of ConstructorCtmcModel
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-01 14:55:05 +0200 (Fri, 01 Jun 2012) $
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id: ConstructorFunction.cpp 1603 2012-06-01 12:55:05Z hoehna $
 */

#include "ArgumentRule.h"
#include "ConstructorCtmcModel.h"
#include "OptionRule.h"
#include "RbLanguageObject.h"
#include "RlAminoAcidState.h"
#include "RlCharacterStateEvolutionAlongTree.h"
#include "RlBranchLengthTree.h"
#include "RlDnaState.h"
#include "RlRnaState.h"
#include "RlTree.h"
#include "RlTimeTree.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Constructor */
ConstructorCtmcModel::ConstructorCtmcModel( void ) : Function() {
    
}


/** Clone the object */
ConstructorCtmcModel* ConstructorCtmcModel::clone(void) const {
    
    return new ConstructorCtmcModel(*this);
}


/** Execute function: we reset our template object here and give out a copy */
RbLanguageObject* ConstructorCtmcModel::execute( void ) {
    
    const RateMatrix& q = static_cast<const RateMatrix &>( args[1].getVariable()->getValue() );
    const RealPos& rate = static_cast<const RealPos &>( args[2].getVariable()->getValue() );
    int nSites = static_cast<const Natural &>( args[3].getVariable()->getValue() ).getValue();
    const std::string& type = static_cast<const RlString &>( args[4].getVariable()->getValue() ).getValue();
    
    Distribution *d = NULL;
    
    if ( type == "DNA" ) 
    {
        d = new CharacterStateEvolutionAlongTree<DnaState, TimeTree>();
    } 
    else if ( type == "RNA" )
    {
        d = new CharacterStateEvolutionAlongTree<RnaState, TimeTree>();
    }
    else if ( type == "AA" || type == "Protein" )
    {
        d = new CharacterStateEvolutionAlongTree<AminoAcidState, TimeTree>();
    }
    
    return d;
}


/** Get argument rules */
const ArgumentRules& ConstructorCtmcModel::getArgumentRules(void) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {

//        argumentRules.push_back( new ArgumentRule( "tree",   true, Tree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "Q",      true, RateMatrix::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "rate",   true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        argumentRules.push_back( new ArgumentRule( "nSites", true, Natural::getClassTypeSpec() ) );
        
        Vector<RlString> options;
        options.push_back( RlString("DNA") );
        options.push_back( RlString("RNA") );
        options.push_back( RlString("AA") );
        options.push_back( RlString("Protein") );
        argumentRules.push_back( new OptionRule( "type", new RlString("DNA"), options ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& ConstructorCtmcModel::getClassName(void) { 
    
    static std::string rbClassName = "CtmcModel Constructor function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ConstructorCtmcModel::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ConstructorCtmcModel::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& ConstructorCtmcModel::getReturnType(void) const {
    
    return CharacterStateEvolutionAlongTree<DnaState,TimeTree>::getClassTypeSpec();
}



