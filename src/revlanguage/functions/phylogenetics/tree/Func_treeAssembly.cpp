#include "Func_treeAssembly.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBranchLengthTree.h"
#include "RlDeterministicNode.h"
#include "TreeAssemblyFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_treeAssembly::Func_treeAssembly( void ) : TypedFunction<BranchLengthTree>( )
{
    
}


/** Clone object */
Func_treeAssembly* Func_treeAssembly::clone( void ) const
{
    
    return new Func_treeAssembly( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::Tree>* Func_treeAssembly::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* brlens = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TreeAssemblyFunction* f = new RevBayesCore::TreeAssemblyFunction( tau, brlens );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_treeAssembly::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "topology", Tree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "brlens",   ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_treeAssembly::getClassType(void)
{
    
    static std::string revType = "Func_treeAssembly";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_treeAssembly::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


const TypeSpec& Func_treeAssembly::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
