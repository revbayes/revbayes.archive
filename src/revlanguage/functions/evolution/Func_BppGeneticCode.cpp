//
//  Func_BppGeneticCode.cpp
//  RevBayesCore
//
//  Created by Laurent Guéguen vendredi 11 juillet 2014, à 11h 41
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_BppGeneticCode.h"
#include "BppGeneticCode.h"
#include "RlBppGeneticCode.h"
#include "RlBppAlphabet.h"
#include <iostream>
#include <sstream>

#include "RlString.h"

#include <Bpp/Seq/App/SequenceApplicationTools.h>


using namespace std;
using namespace RevLanguage;

/** default constructor */
Func_BppGeneticCode::Func_BppGeneticCode( void ) : Function( ) {
    
}


/** Clone object */
Func_BppGeneticCode* Func_BppGeneticCode::clone( void ) const {
    
    return new Func_BppGeneticCode( *this );
}


RevObject* Func_BppGeneticCode::execute() {

  RevBayesCore::TypedDagNode<RevBayesCore::BppAlphabet>* dalph= static_cast<const BppAlphabet&>(args[0].getVariable()->getRevObject()).getDagNode();

  const RlString& fn = static_cast<const RlString&>( args[1].getVariable()->getRevObject() );

  ostringstream o;
  fn.printValue(o);

  bpp::GeneticCode* gc=bpp::SequenceApplicationTools::getGeneticCode(static_cast<const bpp::CodonAlphabet*>(dalph->getValue().getAlphabet())->getNucleicAlphabet(),o.str());

  RevBayesCore::BppGeneticCode* rbgc= new RevBayesCore::BppGeneticCode(gc);
  
  return new BppGeneticCode(rbgc);
}


/* Get argument rules */
const ArgumentRules& Func_BppGeneticCode::getArgumentRules( void ) const {

  
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "alphabet", true, BppAlphabet::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "desc", true, RlString::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_BppGeneticCode::getClassName(void) { 
    
    static std::string rbClassName = "Func_BppGeneticCode";
    
    return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_BppGeneticCode::getClassTypeSpec(void) { 
    
  static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
  
  return rbClass; 
}


/* Get return type */
const TypeSpec& Func_BppGeneticCode::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = BppGeneticCode::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_BppGeneticCode::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
