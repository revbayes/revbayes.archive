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
	
    methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
	methods.addFunction("matrix",              new MemberProcedure(MatrixReal::getClassTypeSpec(),            matrixArgRules          ) );
	
}


RlDistanceMatrix::RlDistanceMatrix( RevBayesCore::DistanceMatrix *v) : ModelObject<RevBayesCore::DistanceMatrix>( v ),
distanceMatrix(v)
{

    ArgumentRules* namesArgRules               = new ArgumentRules();
    ArgumentRules* matrixArgRules              = new ArgumentRules();
	
	methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
	methods.addFunction("matrix",              new MemberProcedure(MatrixReal::getClassTypeSpec(),            matrixArgRules          ) );
	
}


RlDistanceMatrix::RlDistanceMatrix( RevBayesCore::TypedDagNode<RevBayesCore::DistanceMatrix> *m) : ModelObject<RevBayesCore::DistanceMatrix>( m ),
distanceMatrix(&m->getValue())
{

	ArgumentRules* namesArgRules               = new ArgumentRules();
	ArgumentRules* matrixArgRules              = new ArgumentRules();
	
	methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
	methods.addFunction("matrix",              new MemberProcedure(MatrixReal::getClassTypeSpec(),            matrixArgRules          ) );

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
	
    return ModelObject<RevBayesCore::DistanceMatrix>::executeMethod( name, args, found );
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

