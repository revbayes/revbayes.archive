//
//  Func_BppModel.cpp
//  RevBayesCore
//
//  Created by Laurent Guéguen vendredi 11 juillet 2014, à 13h 19
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "BppModelRateMatrixFunction.h"
#include "Func_BppModel.h"
#include "BppRateMatrix.h"

#include "RlBppAlphabet.h"
#include "RlBppGeneticCode.h"
#include "RlRateMatrix.h"
#include "RlString.h"
#include "TypedDagNode.h"

#include <Bpp/Phyl/Io/BppOSubstitutionModelFormat.h>

using namespace RevLanguage;
using namespace std;

/** default constructor */
Func_BppModel::Func_BppModel( void ) : Function( ) {
    
}


/** Clone object */
Func_BppModel* Func_BppModel::clone( void ) const {
    
    return new Func_BppModel( *this );
}


RevObject* Func_BppModel::execute() {
  const bpp::Alphabet* alphabet= static_cast<const BppAlphabet&>(args[0].getVariable()->getRevObject()).getDagNode()->getValue().getAlphabet();

  const bpp::GeneticCode* gc= static_cast<const BppGeneticCode&>(args[1].getVariable()->getRevObject()).getDagNode()->getValue().getGeneticCode();

  const RlString& fn = static_cast<const RlString&>( args[2].getVariable()->getRevObject() );
  ostringstream o;
  fn.printValue(o);

  bpp::BppOSubstitutionModelFormat bIO(bpp::BppOSubstitutionModelFormat::ALL, true, true, true, false, 0);
  
  if (dynamic_cast<const bpp::CodonAlphabet*>(alphabet))
    bIO.setGeneticCode(gc);
  
  bpp::SubstitutionModel* model = bIO.read(alphabet, o.str(), 0, true);
  
  map<string, const RevBayesCore::TypedDagNode<double >* > sTDN;
  
  map<string, string> Unp=bIO.getUnparsedArguments();

  map<string, string>::iterator it;
  
  for (it=Unp.begin();it!=Unp.end();it++)
  {
    if (env->existsVariable(it->second))
      sTDN[it->first]=static_cast<RevBayesCore::TypedDagNode<double> * >((*env)[it->second].getRevObject().getDagNode());
  }
  
  RevBayesCore::BppModelRateMatrixFunction* f = new RevBayesCore::BppModelRateMatrixFunction( sTDN, model );
  
  RevBayesCore::DeterministicNode<RevBayesCore::RateMatrix> *detNode = new RevBayesCore::DeterministicNode<RevBayesCore::RateMatrix>("", f);
    
  RateMatrix* value = new RateMatrix( detNode );
    
  return value;
}


/* Get argument rules */
const ArgumentRules& Func_BppModel::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
      argumentRules.push_back( new ArgumentRule( "alphabet", true, BppAlphabet::getClassTypeSpec()));
      argumentRules.push_back( new ArgumentRule( "genetic_code", true, BppGeneticCode::getClassTypeSpec()));//, new RevBayesCore::BppGeneticCode(new bpp::GeneticCode());
      argumentRules.push_back( new ArgumentRule( "desc", true, RlString::getClassTypeSpec()));
        
      rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_BppModel::getClassName(void) { 
    
    static std::string rbClassName = "Func_BppModel";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_BppModel::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_BppModel::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_BppModel::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
