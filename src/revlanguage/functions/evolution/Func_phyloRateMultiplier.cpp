#include "Func_phyloRateMultiplier.h"
#include "RateMultiplierPhyloFunction.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlTimeTree.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
Func_phyloRateMultiplier::Func_phyloRateMultiplier( void ) : Function( ) {
    
}


/** Clone object */
Func_phyloRateMultiplier* Func_phyloRateMultiplier::clone( void ) const 
{
    
    return new Func_phyloRateMultiplier( *this );
}


RevPtr<Variable> Func_phyloRateMultiplier::execute()
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tree = static_cast<const TimeTree &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< std::vector<double> >* rates = static_cast<const Vector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* baseRate = static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::RateMultiplierPhyloFunction* f = new RevBayesCore::RateMultiplierPhyloFunction( tree, rates, baseRate );
    
    DeterministicNode< std::vector<double> > *detNode = new DeterministicNode< std::vector<double> >("", f, this->clone());
    
    Vector<RealPos>* value = new Vector<RealPos>( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_phyloRateMultiplier::getArgumentRules( void ) const 
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


const std::string& Func_phyloRateMultiplier::getClassName(void) { 
    
    static std::string rbClassName = "Func_phyloRateMultiplier";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_phyloRateMultiplier::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_phyloRateMultiplier::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Vector<RealPos>::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_phyloRateMultiplier::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
