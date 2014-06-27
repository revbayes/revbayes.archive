#include "DeterministicNode.h"
#include "RateMultiplierPhyloFunction.h"
#include "RlRateMultiplierPhyloFunction.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
RateMultiplierPhyloFunction::RateMultiplierPhyloFunction( void ) : Function( ) {
    
}


/** Clone object */
RateMultiplierPhyloFunction* RateMultiplierPhyloFunction::clone( void ) const 
{
    
    return new RateMultiplierPhyloFunction( *this );
}


RevObject* RateMultiplierPhyloFunction::execute() 
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tree = static_cast<const TimeTree &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< std::vector<double> >* rates = static_cast<const Vector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* baseRate = static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::RateMultiplierPhyloFunction* f = new RevBayesCore::RateMultiplierPhyloFunction( tree, rates, baseRate );
    RevBayesCore::DeterministicNode< std::vector<double> > *detNode = new RevBayesCore::DeterministicNode< std::vector<double> >("", f);
    
    Vector<RealPos>* value = new Vector<RealPos>( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& RateMultiplierPhyloFunction::getArgumentRules( void ) const 
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        
        argumentRules.push_back( new ArgumentRule( "tree",      true, TimeTree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "rates",     true, Vector<RealPos>::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "baseRate",  true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& RateMultiplierPhyloFunction::getClassName(void) { 
    
    static std::string rbClassName = "RateMultiplierPhyloFunction";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& RateMultiplierPhyloFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& RateMultiplierPhyloFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Vector<RealPos>::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& RateMultiplierPhyloFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
