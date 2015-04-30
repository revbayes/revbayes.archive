#include "Argument.h"
#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_concatenate.h"
#include "RbException.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "Workspace.h"

using namespace RevLanguage;

/** Default constructor */
Func_concatenate::Func_concatenate( void ) : Procedure()
{
    
}


/** Clone object */
Func_concatenate* Func_concatenate::clone( void ) const
{
    
    return new Func_concatenate( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_concatenate::execute( void )
{
    
    const AbstractDiscreteCharacterData& a = static_cast<const AbstractDiscreteCharacterData &>( args[0].getVariable()->getRevObject() );
    const AbstractDiscreteCharacterData& b = static_cast<const AbstractDiscreteCharacterData &>( args[1].getVariable()->getRevObject() );
    
    AbstractDiscreteCharacterData* d = a.concatenate( b );
    for (size_t i = 2; i < args.size(); ++i)
    {
        const AbstractDiscreteCharacterData& c = static_cast<const AbstractDiscreteCharacterData &>( args[i].getVariable()->getRevObject() );
        
        AbstractDiscreteCharacterData* tmp = d->concatenate( c );
        delete d;
        
        d = tmp;
    }
    
    return new RevVariable( d );
}


/** Get argument rules */
const ArgumentRules& Func_concatenate::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "a", AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "v", AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new Ellipsis( AbstractDiscreteCharacterData::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_concatenate::getClassType(void) {
    
    static std::string revType = "Func_concatenate";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_concatenate::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_concatenate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_concatenate::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}

