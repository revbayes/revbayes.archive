#include "Func_maximumTree.h"

#include "ModelVector.h"
#include "RlDeterministicNode.h"
#include "RlTimeTree.h"
#include "MaximumTreeFunction.h"
#include "TypedDagNode.h"
#include "Argument.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ModelObject.h"
#include "RbHelpReference.h"
#include "RbVector.h"
#include "RevVariable.h"
#include "RlFunction.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** default constructor */
Func_maximumTree::Func_maximumTree( void ) : TypedFunction<TimeTree>( )
{

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_maximumTree* Func_maximumTree::clone( void ) const
{

    return new Func_maximumTree( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::Tree >* Func_maximumTree::createFunction( void ) const
{

    RevBayesCore::TypedDagNode< RevBayesCore::RbVector< RevBayesCore::Tree > >* gTrees = static_cast<const ModelVector< TimeTree > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::MaximumTreeFunction* f = new RevBayesCore::MaximumTreeFunction( gTrees );

    return f;
}


/* Get argument rules */
const ArgumentRules& Func_maximumTree::getArgumentRules( void ) const
{

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;

    if ( !rules_set )
    {

        argumentRules.push_back( new ArgumentRule( "geneTrees", ModelVector< TimeTree >::getClassTypeSpec() , "The vector of trees from which to pick the maximum.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rules_set = true;
    }

    return argumentRules;
}


const std::string& Func_maximumTree::getClassType(void)
{

    static std::string rev_type = "Func_maximumTree";

	return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_maximumTree::getClassTypeSpec(void)
{

    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );

	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_maximumTree::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "maximumTree";

    return f_name;
}


const TypeSpec& Func_maximumTree::getTypeSpec( void ) const
{

    static TypeSpec type_spec = getClassTypeSpec();

    return type_spec;
}
