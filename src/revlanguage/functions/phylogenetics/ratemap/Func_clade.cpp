#include "Func_clade.h"
#include "ModelVector.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"

using namespace RevLanguage;

/** default constructor */
Func_clade::Func_clade( void ) : Procedure( )
{
    
}


/** Clone object */
Func_clade* Func_clade::clone( void ) const
{
    
    return new Func_clade( *this );
}


RevPtr<RevVariable> Func_clade::execute()
{
    
    // now allocate a new Clade
    const std::vector<std::string>& n = static_cast<const ModelVector<RlString> &>( args[0].getVariable()->getRevObject() ).getValue();
    
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < n.size(); ++i)
    {
        taxa.push_back( RevBayesCore::Taxon( n[i] ) );
    }
    
    double a = static_cast<const RealPos &>( args[1].getVariable()->getRevObject() ).getValue();
    RevBayesCore::Clade *c = new RevBayesCore::Clade(taxa, a);
    
    return new RevVariable( new Clade(c) );
}


/* Get argument rules */
const ArgumentRules& Func_clade::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "taxa"   , ModelVector<RlString>::getClassTypeSpec(), "A vector a taxa that is contained in this clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "age"    , RealPos::getClassTypeSpec(), "The age of the clade (optional).", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_clade::getClassType(void)
{
    
    static std::string revType = "Func_clade";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_clade::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_clade::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "clade";
    
    return f_name;
}


/* Get return type */
const TypeSpec& Func_clade::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = Clade::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_clade::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
