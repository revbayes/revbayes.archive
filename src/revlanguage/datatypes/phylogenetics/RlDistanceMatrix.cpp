#include "ArgumentRule.h"
#include "DistanceMatrix.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RbVector.h"
#include "RealPos.h"
#include "RlDistanceMatrix.h"
#include "RlBoolean.h"
#include "RlMatrixReal.h"
#include "RlString.h"
#include "RlSimplex.h"
#include "RevVariable.h"
#include <vector>

using namespace RevLanguage;

RlDistanceMatrix::RlDistanceMatrix(void) : ModelObject<RevBayesCore::DistanceMatrix>( )
{

    ArgumentRules* namesArgRules               = new ArgumentRules();
    ArgumentRules* matrixArgRules              = new ArgumentRules();
	ArgumentRules* elementArgRules             = new ArgumentRules();
	elementArgRules->push_back( new ArgumentRule( "i" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
	elementArgRules->push_back( new ArgumentRule( "j" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );

    methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
	methods.addFunction("matrix",              new MemberProcedure(MatrixReal::getClassTypeSpec(),            matrixArgRules          ) );
	methods.addFunction("getElement",          new MemberProcedure(Real::getClassTypeSpec(),                  elementArgRules         ) );

	
}


RlDistanceMatrix::RlDistanceMatrix( RevBayesCore::DistanceMatrix *v) : ModelObject<RevBayesCore::DistanceMatrix>( v ),
distanceMatrix(v)
{

    ArgumentRules* namesArgRules               = new ArgumentRules();
    ArgumentRules* matrixArgRules              = new ArgumentRules();
	ArgumentRules* elementArgRules             = new ArgumentRules();
	elementArgRules->push_back( new ArgumentRule( "i" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
	elementArgRules->push_back( new ArgumentRule( "j" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );

	methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
	methods.addFunction("matrix",              new MemberProcedure(MatrixReal::getClassTypeSpec(),            matrixArgRules          ) );
	methods.addFunction("getElement",          new MemberProcedure(Real::getClassTypeSpec(),                  elementArgRules         ) );

}


RlDistanceMatrix::RlDistanceMatrix( RevBayesCore::TypedDagNode<RevBayesCore::DistanceMatrix> *m) : ModelObject<RevBayesCore::DistanceMatrix>( m ),
distanceMatrix(&m->getValue())
{

	ArgumentRules* namesArgRules               = new ArgumentRules();
	ArgumentRules* matrixArgRules              = new ArgumentRules();
	ArgumentRules* elementArgRules             = new ArgumentRules();
	elementArgRules->push_back( new ArgumentRule( "i" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
	elementArgRules->push_back( new ArgumentRule( "j" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
	
	methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
	methods.addFunction("matrix",              new MemberProcedure(MatrixReal::getClassTypeSpec(),            matrixArgRules          ) );
	methods.addFunction("getElement",          new MemberProcedure(Real::getClassTypeSpec(),                  elementArgRules         ) );

}


RlDistanceMatrix* RlDistanceMatrix::clone() const {
    return new RlDistanceMatrix( *this );
}


/* Map calls to member methods */
RevPtr<RevVariable> RlDistanceMatrix::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "matrix")
    {
        found = true;

        return new RevVariable(new MatrixReal((RevBayesCore::MatrixReal) (this->dagNode->getValue().getMatrix() ) ) ) ;
    }
    else if (name == "names")
    {
        found = true;
		
		std::vector<std::string> names = this->dagNode->getValue().getNames();
        ModelVector<RlString> *n = new ModelVector<RlString>();
        for (size_t i = 0; i < names.size(); ++i)
        {
            n->push_back( names[i] );
        }
        return new RevVariable( n );
    }
	else if ( name == "size" )
	{
		found = true;
		
		// return a new RevVariable with the size of this container
		return RevPtr<RevVariable>( new RevVariable( new Natural( size() ), "" ) );
	}
	else if ( name == "getElement" )
	{
		
		if ( args.size() > 1 && args[0].getVariable()->getRevObject().isType( Natural::getClassTypeSpec() ) && args[1].getVariable()->getRevObject().isType( Natural::getClassTypeSpec() ) )
		{
			found = true;
			// get the member with given indices
			const Natural& i = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
			const Natural& j = static_cast<const Natural&>( args[1].getVariable()->getRevObject() );

			if ( size() < (size_t)(i.getValue()) )
			{
				throw RbException("Index i out of bounds in getElement");
			}
			if ( size() < (size_t)(j.getValue()) )
			{
				throw RbException("Index j out of bounds in getElement");
			}

			double element = static_cast< RevBayesCore::DistanceMatrix& >( this->dagNode->getValue() ).getElement(size_t(i.getValue()) - 1, size_t(j.getValue()) - 1);
			
			return new RevVariable( new Real( element ) );

		}

	}

	
    return ModelObject<RevBayesCore::DistanceMatrix>::executeMethod( name, args, found );
}





const Real* RlDistanceMatrix::getElement(size_t idx, size_t idy) const
{
	double element = static_cast< RevBayesCore::DistanceMatrix& >( this->dagNode->getValue() ).getElement(idx - 1, idy - 1);
	
	return new Real( element ) ;

}



/*
template <typename rlType>
rlType* ModelVector<rlType>::getElement(size_t idx) const
{
	return new rlType( this->getValue()[ idx ] );
}



 RevObject*   RlDistanceMatrix::getElement(size_t idx) const {
	
	return new RevVariable( this->getValue()[ idx ] );
}*/

/**
 * Add an element to the end of the vector.
 */

/*void RlDistanceMatrix::push_back(const RevObject &x)
{
	//We do nothing, in principle we could implement a push_back...
}
*/

/**
 * Size of the matrix.
 */
size_t RlDistanceMatrix::size( void ) const
{
	return this->dagNode->getValue().size();
}




/* Get Rev type of object */
const std::string& RlDistanceMatrix::getClassType(void) {
    
    static std::string revType = "RlDistanceMatrix";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RlDistanceMatrix::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RlDistanceMatrix::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}

