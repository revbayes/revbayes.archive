#include "ArgumentRule.h"
#include "Func_TaxonReader.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlUserInterface.h"
#include "TaxonReader.h"

#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_TaxonReader* Func_TaxonReader::clone( void ) const
{
    
    return new Func_TaxonReader( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_TaxonReader::execute( void )
{
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    char del = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue()[0];
    
    RevBayesCore::TaxonReader tr = RevBayesCore::TaxonReader(fn.getValue(), del);
    const std::vector<RevBayesCore::Taxon>& taxa = tr.getTaxa();
    
    return new RevVariable( new ModelVector<Taxon>( taxa ) );
}





/** Get argument rules */
const ArgumentRules& Func_TaxonReader::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        argumentRules.push_back( new ArgumentRule( "filename", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "delimiter", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_TaxonReader::getClassType(void)
{
    
    static std::string revType = "Func_TaxonReader";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_TaxonReader::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_TaxonReader::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_TaxonReader::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = ModelVector<Taxon>::getClassTypeSpec();
    return returnTypeSpec;
}




