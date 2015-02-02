#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_discretizeGamma.h"
#include "Integer.h"
#include "NormalizeVectorFunction.h"
#include "ModelVector.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

#include "DiscretizeGammaFunction.h"


using namespace RevLanguage;

Func_discretizeGamma::Func_discretizeGamma() : TypedFunction< ModelVector< RealPos > >() {
    
}

/* Clone object */
Func_discretizeGamma* Func_discretizeGamma::clone( void ) const
{
    
    return new Func_discretizeGamma( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_discretizeGamma::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* shape = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* rate = static_cast<const Real &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int>* numCats = static_cast<const Integer &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    bool median = static_cast<const RlBoolean &>( args[3].getVariable()->getRevObject() ).getValue();

    RevBayesCore::DiscretizeGammaFunction *func = new RevBayesCore::DiscretizeGammaFunction( shape, rate, numCats, median );
    
    return func;
}


/** Get argument rules */
const ArgumentRules& Func_discretizeGamma::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "shape"   , RealPos::getClassTypeSpec()  , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "rate"    , RealPos::getClassTypeSpec()  , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "num_cats", Integer::getClassTypeSpec()  , ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "median"  , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_discretizeGamma::getClassName(void) { 
    
    static std::string rbClassName = "Func_discretizeGamma";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func_discretizeGamma::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func_discretizeGamma::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
