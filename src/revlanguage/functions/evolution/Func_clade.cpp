#include "Func_clade.h"
#include "ModelVector.h"
#include "RlClade.h"
#include "RlString.h"

using namespace RevLanguage;

/** default constructor */
Func_clade::Func_clade( void ) : Procedure( ) {
    
}


/** Clone object */
Func_clade* Func_clade::clone( void ) const {
    
    return new Func_clade( *this );
}


RevPtr<Variable> Func_clade::execute() {
    
    // now allocate a new Clade
    const std::vector<std::string>& n = static_cast<const ModelVector<RlString> &>( args[0].getVariable()->getRevObject() ).getValue();
    RevBayesCore::Clade *c = new RevBayesCore::Clade(n,0.0);
    
    return new Variable( new Clade(c) );
}


/* Get argument rules */
const ArgumentRules& Func_clade::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "taxa", ModelVector<RlString>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_clade::getClassType(void) { 
    
    static std::string revType = "Func_clade";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_clade::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_clade::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Clade::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_clade::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
