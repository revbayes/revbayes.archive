//
//  Func_BppAlphabet.cpp
//  RevBayesCore
//
//  Created by Laurent Guéguen dimanche 6 juillet 2014, à 23h 25
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_BppAlphabet.h"
#include "BppAlphabet.h"
#include "RlBppAlphabet.h"
#include <iostream>
#include <sstream>

#include "RlString.h"

#include <Bpp/Seq/App/SequenceApplicationTools.h>


using namespace std;
using namespace RevLanguage;

/** default constructor */
Func_BppAlphabet::Func_BppAlphabet( void ) : Function( ) {
    
}


/** Clone object */
Func_BppAlphabet* Func_BppAlphabet::clone( void ) const {
    
    return new Func_BppAlphabet( *this );
}


RevObject* Func_BppAlphabet::execute() {

  const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
  
  ostringstream o;
  fn.printValue(o);

  map<string , string> mdesc;
  mdesc["alphabet"]=o.str();

  bpp::Alphabet* alph=bpp::SequenceApplicationTools::getAlphabet(mdesc);

  RevBayesCore::BppAlphabet* rbAlph= new RevBayesCore::BppAlphabet(alph);
  
  return new BppAlphabet(rbAlph);
}


/* Get argument rules */
const ArgumentRules& Func_BppAlphabet::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "BppO alphabet description", true, RlString::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_BppAlphabet::getClassName(void) { 
    
    static std::string rbClassName = "Func_BppAlphabet";
    
    return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_BppAlphabet::getClassTypeSpec(void) { 
    
  static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
  
  return rbClass; 
}


/* Get return type */
const TypeSpec& Func_BppAlphabet::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = BppAlphabet::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_BppAlphabet::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
