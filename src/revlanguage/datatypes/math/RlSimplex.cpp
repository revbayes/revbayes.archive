#include "RlSimplex.h"

#include "ConstantNode.h"
#include "Natural.h"
#include "Probability.h"
#include "RbException.h"
#include "RlUtils.h"
#include "StochasticNode.h"
#include "VectorIndexOperator.h"

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
Simplex::Simplex( const std::vector<double>& v ) :
    ModelVector<RealPos>()
{
    std::vector<double>* newVal = makeNormalizedValue( v );
    
    // Now set the constant value of the simplex
    RevBayesCore::ConstantNode< std::vector< double > >* newNode = new RevBayesCore::ConstantNode< std::vector< double > >( "", newVal );
    this->setDagNode( newNode );
}


/**
 * Make sure that a distribution or a function associated with
 * a dynamic node are of the right type
 */
Simplex::Simplex( RevBayesCore::TypedDagNode<std::vector<double> >* n ) :
    ModelVector<RealPos>()
{
    // Make sure we actually have a simplex
    RevBayesCore::DynamicNode< std::vector<double> >* dynNode = dynamic_cast<RevBayesCore::DynamicNode< std::vector<double> >* >( n );
    if ( dynNode != NULL )
    {
        // We just make sure the type is right before setting the dynamic node
        // TODO: Ask the DAG node for the Rev object type
//        if ( dynNode->getRevTypeOfValue() != getClassType() )
//            throw RbException( "Invalid attempt to initialize a simplex with a dynamic DAG node of Rev type '" + dynNode->getRevTypeOfValue() + "'" );

        this->setDagNode( n );
    }
    else
    {
        // We have a constant node, which does not know its language type. We normalize the vector to be on the safe side and
        // replace the node.
        const std::vector<double>& v = n->getValue();
        std::vector<double>* newVal = makeNormalizedValue( v );

        RevBayesCore::ConstantNode< std::vector< double > >* newNode = new RevBayesCore::ConstantNode< std::vector< double > >( "", newVal );
        
        this->setDagNode( newNode );
    }
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


/**
 * We override this function here to stop the parser
 * from assigning to the simplex through direct
 * element assignment.
 */
RevPtr<Variable> Simplex::findOrCreateElement(const std::vector<size_t> oneOffsetIndices)
{
    throw RbException( "Illegal attempt to assign to simplex element" );
}


/**
 * We override this function here to stop the parser
 * from creating a reference to one of our elements,
 * allowing the user to modify that element. Instead of
 * throwing an error, we simply create a copy of the
 * element, which leaves our element intact. This is
 * done by calling the getElementFromValue function,
 * which does exactly that.
 */
RevPtr<Variable> Simplex::getElement(const std::vector<size_t> oneOffsetIndices)
{
    return getElementFromValue( oneOffsetIndices );
}


/** Get class name of object */
const std::string& Simplex::getClassType(void) { 
    
    static std::string revType = "Simplex";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Simplex::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Container::getClassTypeSpec() ) );
    
	return revTypeSpec; 
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
std::vector<double>* Simplex::makeNormalizedValue( const std::vector<double>& v )
{
    // Check that we have real positive values
    for ( size_t i = 0; i < v.size(); ++i )
        if ( v[i] < 0.0 )
            throw RbException( "Attempt to set simplex element with negative number" );
    
    // Normalize the vector to be on the safe side
    std::vector<double>* newVal = new std::vector<double>( v );
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
