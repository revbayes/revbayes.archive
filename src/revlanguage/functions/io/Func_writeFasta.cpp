/**
 * @file
 * This file contains the implementation of Func_readAlignment.
 *
 * @brief Implementation of Func_readAlingment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-22 11:45:25 +0200 (Wed, 22 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_readCharacterData.cpp 1765 2012-08-22 09:45:25Z hoehna $
 */

#include "ArgumentRule.h"
#include "Func_writeFasta.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RlCharacterData.h"
#include "RlDnaState.h"
#include "RlString.h"
#include "FastaWriter.h"



using namespace RevLanguage;

/** Clone object */
Func_writeFasta* Func_writeFasta::clone( void ) const {
    
    return new Func_writeFasta( *this );
}


/** Execute function */
RbLanguageObject* Func_writeFasta::execute( void ) {
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getValue() );
    const RevBayesCore::AbstractCharacterData &data = static_cast< const CharacterData<DnaState> & >( args[1].getVariable()->getValue() ).getValue();
    
    RevBayesCore::FastaWriter fw;
    fw.writeData(fn.getValue(), data);
    
    return NULL;
}




/** Get argument rules */
const ArgumentRules& Func_writeFasta::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) {
        argumentRules.push_back( new ArgumentRule( "filename", true, RlString::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "data", true, CharacterData<DnaState>::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_writeFasta::getClassName(void) { 
    
    static std::string rbClassName = "Write fasta function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_writeFasta::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_writeFasta::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_writeFasta::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbNullObject::getClassTypeSpec();
    return returnTypeSpec;
}




