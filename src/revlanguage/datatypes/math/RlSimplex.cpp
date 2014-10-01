#include "RlSimplex.h"

#include "ConstantNode.h"
#include "Natural.h"
#include "Probability.h"
#include "RbException.h"
#include "RlUtils.h"

using namespace RevLanguage;


/**
 * Construct an empty simplex.
 * We simply rely on the base class.
 */
Simplex::Simplex( void ) :
    ModelVector<RealPos>()
{
}


/**
 * Construct simplex from a vector of real values (doubles). We
 * rescale the values here and make sure they are all positive.
 * Just in case.
 */
Simplex::Simplex( const RevBayesCore::RbVector<double>& v ) :
    ModelVector<RealPos>()
{
    RevBayesCore::RbVector<double>* newVal = makeNormalizedValue( v );
    
    // Now set the constant value of the simplex
    RevBayesCore::ConstantNode< RevBayesCore::RbVector< double > >* newNode = new RevBayesCore::ConstantNode< RevBayesCore::RbVector< double > >( "", newVal );
    this->setDagNode( newNode );
}


/**
 * Make sure that a distribution or a function associated with
 * a dynamic node are of the right type.
 *
 * @todo Make sure we actually have a simplex stored in n (or an
 *       NA value)
 */
Simplex::Simplex( RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* n ) :
    ModelVector<RealPos>()
{
        this->setDagNode( n );
}


/**
 * Virtual destructor to ensure proper handling
 * of deletion from base class pointers or
 * references in case of future derived classes.
 */
Simplex::~Simplex()
{
}


/** Generate a type-safe clone */
Simplex* Simplex::clone( void ) const
{
    return new Simplex( *this );
}


/** Get Rev type of object */
const std::string& Simplex::getClassType(void) { 
    
    static std::string revType = "Simplex";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Simplex::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &ModelVector<RealPos>::getClassTypeSpec() );
    
	return revTypeSpec; 
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& Simplex::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get the type spec of this class */
const TypeSpec& Simplex::getTypeSpec(void) const
{
    return getClassTypeSpec();
}


/**
 * Generate a normalized vector. This is a Simplex help functions used
 * by the constructors.
 */
RevBayesCore::RbVector<double>* Simplex::makeNormalizedValue( const RevBayesCore::RbVector<double>& v )
{
    // Check that we have real positive values
    for ( size_t i = 0; i < v.size(); ++i )
    {
        if ( v[i] < 0.0 )
        {
            throw RbException( "Attempt to set simplex element with negative number" );
        }
    }
    
    // Normalize the vector to be on the safe side
    RevBayesCore::RbVector<double>* newVal = new RevBayesCore::RbVector<double>( v );
    double sum = 0.0;
    for ( size_t i = 0; i < (*newVal).size(); ++i )
        sum += (*newVal)[i];
    for ( size_t i = 0; i < (*newVal).size(); ++i )
        (*newVal)[i] /= sum;
    
    // Return the normalized value (the simplex)
    return newVal;
}


/** Throw an error if someone attempts to sort us */
void Simplex::sort( void )
{
    throw RbException( "Illegal attempt to sort a Simplex variable" );
}


/** Throw an error if someone attempts to apply unique */
void Simplex::unique( void )
{
    throw RbException( "Illegal attempt to apply unique to a Simplex variable" );
}
