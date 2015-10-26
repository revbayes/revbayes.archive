#include "Func_maximumTree.h"
#include "ModelVector.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlTimeTree.h"
#include "ModelVector.h"
#include "MaximumTreeFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_maximumTree::Func_maximumTree( void ) : TypedFunction<TimeTree>( )
{
    
}


/** Clone object */
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
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "geneTrees", ModelVector< TimeTree >::getClassTypeSpec() , "The vector of trees from which to pick the maximum.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_maximumTree::getClassType(void)
{
    
    static std::string revType = "Func_maximumTree";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_maximumTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_maximumTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
