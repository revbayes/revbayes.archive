#include "Func_treeAssembly.h"
#include "ModelVector.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RlTree.h"
#include "RlDeterministicNode.h"
#include "TreeAssemblyFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_treeAssembly::Func_treeAssembly( void ) : TypedFunction<Tree>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_treeAssembly* Func_treeAssembly::clone( void ) const
{
    
    return new Func_treeAssembly( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::Tree>* Func_treeAssembly::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* brlens = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::RbVector<double> tmp = brlens->getValue();

    bool m = static_cast<const RlBoolean &>( this->args[2].getVariable()->getRevObject() ).getValue();

    std::string label = this->args[2].getLabel();

    RevBayesCore::TreeAssemblyFunction* f;
    if( label == "multiply" )
    {
        f = new RevBayesCore::TreeAssemblyFunction( tau, brlens, false, m );
    }
    else
    {
        f = new RevBayesCore::TreeAssemblyFunction( tau, brlens, m, false );
    }
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_treeAssembly::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "topology", Tree::getClassTypeSpec(), "The tree topology variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "brlens",   ModelVector<RealPos>::getClassTypeSpec(), "The vector of branch lengths.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        std::vector<std::string> labels;
        labels.push_back("multiply");
        labels.push_back("divide");
        argumentRules.push_back( new ArgumentRule( labels , RlBoolean::getClassTypeSpec() , "Multiply/divide topology branch lengths by input branch lengths?", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(false) ) );

        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_treeAssembly::getClassType(void)
{
    
    static std::string rev_type = "Func_treeAssembly";
    
	return rev_type; 
}


/* Get class type spec describing type of object */
const TypeSpec& Func_treeAssembly::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Func_treeAssembly::getFunctionNameAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "treeAssembly" );
    
    return a_names;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_treeAssembly::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnTreeAssembly";
    
    return f_name;
}


const TypeSpec& Func_treeAssembly::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
