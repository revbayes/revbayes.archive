#include "ArgumentRule.h"
#include "DiscretizeDistributionFunction.h"
#include "Ellipsis.h"
#include "Func_discretizeDistribution.h"
#include "Integer.h"
#include "ModelVector.h"
#include "RbUtil.h"
#include "Real.h"
#include "RlBoolean.h"
#include "RlContinuousDistribution.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Func_discretizeDistribution::Func_discretizeDistribution() : TypedFunction< ModelVector< Real > >()
{
    
}

/* Clone object */
Func_discretizeDistribution* Func_discretizeDistribution::clone( void ) const
{
    
    return new Func_discretizeDistribution( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_discretizeDistribution::createFunction( void ) const
{
    
    const ContinuousDistribution& rlDistribution = static_cast<const ContinuousDistribution &>( this->args[0].getVariable()->getRevObject() );
    RevBayesCore::ContinuousDistribution* g0 = static_cast<RevBayesCore::ContinuousDistribution* >( rlDistribution.createDistribution() );
    RevBayesCore::TypedDagNode<int>* numCats = static_cast<const Integer &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::DiscretizeDistributionFunction *func = new RevBayesCore::DiscretizeDistributionFunction( g0, numCats );
    
    return func;
}


/** Get argument rules */
const ArgumentRules& Func_discretizeDistribution::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "G0"      , ContinuousDistribution::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "num_cats", Integer::getClassTypeSpec()               , ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_discretizeDistribution::getClassName(void)
{
    
    static std::string rbClassName = "Func_discretizeDistribution";
    
    return rbClassName;
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func_discretizeDistribution::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rbClass;
}


/** Get type spec */
const TypeSpec& Func_discretizeDistribution::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
