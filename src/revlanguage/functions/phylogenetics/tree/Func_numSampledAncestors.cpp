#include "Clade.h"
#include "Func_numSampledAncestors.h"
#include "ModelVector.h"
#include "RlBoolean.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "Natural.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "NumberSampledAncestorsStatistic.h"

using namespace RevLanguage;

/** default constructor */
Func_numSampledAncestors::Func_numSampledAncestors( void ) : TypedFunction<Natural>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_numSampledAncestors* Func_numSampledAncestors::clone( void ) const {
    
    return new Func_numSampledAncestors( *this );
}


RevBayesCore::TypedFunction<int>* Func_numSampledAncestors::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::NumberSampledAncestorsStatistic* f = new RevBayesCore::NumberSampledAncestorsStatistic( tau );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_numSampledAncestors::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), "The tree variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_numSampledAncestors::getClassType(void)
{
    
    static std::string revType = "Func_numSampledAncestors";
    
	return revType; 
}


/* Get class type spec describing type of object */
const TypeSpec& Func_numSampledAncestors::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_numSampledAncestors::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnNumSampledAncestors";
    
    return f_name;
}


const TypeSpec& Func_numSampledAncestors::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
