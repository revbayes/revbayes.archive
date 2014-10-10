#include "Func_averageRateOnBranch.h"
#include "ModelVector.h"
#include "RateOnBranchAve.h"
#include "RealPos.h"
#include "Integer.h"
#include "RlDeterministicNode.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

/** default constructor */
Func_averageRateOnBranch::Func_averageRateOnBranch( void ) : Function( ) {
    
}


/** Clone object */
Func_averageRateOnBranch* Func_averageRateOnBranch::clone( void ) const 
{
    
    return new Func_averageRateOnBranch( *this );
}


RevPtr<Variable> Func_averageRateOnBranch::execute()
{

    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* nodeRates = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tree = static_cast<const TimeTree &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
	RevBayesCore::TypedDagNode< double >* rootRate = static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
	RevBayesCore::TypedDagNode< double >* scaleValue = static_cast<const RealPos &>( this->args[3].getVariable()->getRevObject() ).getDagNode();
	size_t myID = static_cast<const Integer &>( args[4].getVariable()->getRevObject() ).getValue();
	RevBayesCore::RateOnBranchAve* f = new RevBayesCore::RateOnBranchAve( nodeRates, tree, rootRate, scaleValue, myID-1 );
    
    DeterministicNode< double > *detNode = new DeterministicNode< double >("", f, this->clone());
    
    RealPos* value = new RealPos( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_averageRateOnBranch::getArgumentRules( void ) const 
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        
        argumentRules.push_back( new ArgumentRule( "nodeRates" , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "tree"      , TimeTree::getClassTypeSpec()            , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "rootRate"  , RealPos::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "scaleValue", RealPos::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        argumentRules.push_back( new ArgumentRule( "index"     , Integer::getClassTypeSpec()             , ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_averageRateOnBranch::getClassType(void) { 
    
    static std::string revType = "Func_averageRateOnBranch";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_averageRateOnBranch::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_averageRateOnBranch::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_averageRateOnBranch::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
