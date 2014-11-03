#include "ArgumentRule.h"
#include "Func_writeFasta.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlDnaState.h"
#include "RlString.h"
#include "FastaWriter.h"



using namespace RevLanguage;


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Func_writeFasta* Func_writeFasta::clone( void ) const 
{
    
    return new Func_writeFasta( *this );
}


/** 
 * Execute the function. 
 * Here we will extract the character data object from the arguments and get the file name
 * into which we shall write the character data. Then we simply create a FastaWriter
 * instance and delegate the work
 *
 * \return NULL because the output is going into a file
 */
RevPtr<Variable> Func_writeFasta::execute( void ) 
{
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    const RevBayesCore::AbstractDiscreteCharacterData &data = static_cast< const AbstractDiscreteCharacterData & >( args[1].getVariable()->getRevObject() ).getValue();
    
    RevBayesCore::FastaWriter fw;
    fw.writeData(fn.getValue(), data);
    
    return NULL;
}


/** 
 * Get the argument rules for this function.
 *
 * The argument rules of the writeFasta function are:
 * (1) the filename which must be a string.
 * (2) the data object that must be some character matrix.
 *
 * \return The argument rules.
 */
const ArgumentRules& Func_writeFasta::getArgumentRules( void ) const 
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ArgumentRule( "filename", RlString::getClassTypeSpec()                     , ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "data"    , AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Func_writeFasta::getClassType(void) 
{ 
    
    static std::string revType = "Func_writeFasta";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Func_writeFasta::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Func_writeFasta::getTypeSpec( void ) const 
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** 
 * Get the return type of the function. 
 * This function does not return anything so the return type is NULL.
 *
 * \return NULL
 */
const TypeSpec& Func_writeFasta::getReturnType( void ) const 
{
    
    static TypeSpec returnTypeSpec = RevNullObject::getClassTypeSpec();
    return returnTypeSpec;
}




