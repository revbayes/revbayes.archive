#include "AminoAcidState.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BranchLengthTree.h"
#include "Clade.h"
#include "ContinuousCharacterData.h"
#include "DiscreteCharacterData.h"
#include "DnaState.h"
#include "Ellipsis.h"
#include "MatrixReal.h"
#include "PrecisionMatrix.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RnaState.h"
#include "Signals.h"
#include "StandardState.h"
#include "Taxon.h"
#include "TimeTree.h"
#include "Topology.h"
#include "TypedUserFunction.h"
#include "TypeSpec.h"
#include "UserFunction.h"
#include "UserFunctionArgs.h"
#include "UserFunctionCall.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
UserFunction::UserFunction( const ArgumentRules*        argRules,
                            const TypeSpec&             retType ,
                            std::list<SyntaxElement*>*  stmts   )   :
    Function(), argumentRules(argRules), returnType(retType), code(stmts) {

}


/** Copy constructor */
UserFunction::UserFunction(const UserFunction &x) :
    Function(x), argumentRules( new ArgumentRules(*x.argumentRules) ), returnType( x.returnType ), code( NULL ) {
    
    // create a new list for the code
    code = new std::list<SyntaxElement*>();
    for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++) {
        SyntaxElement* element = (*i)->clone();
        code->push_back(element);
    }
}


UserFunction& UserFunction::operator=(const UserFunction &f) {
    
    if ( this != &f ) {
        // call the base class assignment operator
        Function::operator=(f);

        delete argumentRules;
        argumentRules = new ArgumentRules(*f.argumentRules);
        
        for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++) {
            SyntaxElement* theSyntaxElement = *it;
            delete theSyntaxElement;
        }
        code->clear();
        
        // create a new list for the code
        for (std::list<SyntaxElement*>::const_iterator i=f.code->begin(); i!=f.code->end(); i++) {
            SyntaxElement* element = (*i)->clone();
            code->push_back(element);
        }
    }
    
    return *this;
}


/** Destructor */
UserFunction::~UserFunction() {
    
    delete argumentRules;
    
    for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++) {
        SyntaxElement* theSyntaxElement = *it;
        delete theSyntaxElement;
    }
    
    delete code;
    
}


/** Clone the object */
UserFunction* UserFunction::clone(void) const {
    
    return new UserFunction(*this);
}


/** Execute function. Here we create a deterministic node if applicable, otherwise we just execute the code */
RevObject* UserFunction::execute( void ) {
    
    // If we can match the internal type, we return an appropriate model/container/factor object with a
    // deterministic node inside it. Otherwise we return a "flat" RevObject without a dag node inside it.

    /* Get the internal value type of the return type */
    std::string internalValueType = Workspace::userWorkspace().getInternalValueType( returnType.getType() );

    /* Return value directly of objects that do not have a templated internal value node by simply executing the code */
        if ( internalValueType == "" )
    {
        UserFunctionCall fxnCall( this );
        return fxnCall.execute();
    }

    /* We have an internal value type so we want to return an object with a deterministic dag node inside it */
    RevObject* retValue = Workspace::userWorkspace().getNewTypeObject( returnType.getType() );
    
    /* Generate the required internal function object */
    UserFunctionCall*           userFunctionCall    = new UserFunctionCall( this );
    UserFunctionArgs*           userFunctionArgs    = new UserFunctionArgs( this );
    if ( internalValueType == "double" )
    {
        makeDeterministicVariable<double>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "int" )
    {
        makeDeterministicVariable<int>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "std::string" )
    {
        makeDeterministicVariable<typename std::string>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "bool" )
    {
        makeDeterministicVariable<bool>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "std::complex<double>" )
    {
        makeDeterministicVariable<typename std::complex<double> >( userFunctionCall, userFunctionArgs, retValue );
    }
#if 0   // Abstract (!)
    else if ( internalValueType == "RateMatrix" )
    {
        makeDeterministicVariable<typename RevBayesCore::RateMatrix>( userFunctionCall, userFunctionArgs, retValue );
    }
#endif
#if 0   // Missing default constructor
    else if ( internalValueType == "MatrixReal" )
    {
        makeDeterministicVariable<typename RevBayesCore::MatrixReal>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "PrecisionMatrix" )
    {
        makeDeterministicVariable<typename RevBayesCore::PrecisionMatrix>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "Taxon" )
    {
        makeDeterministicVariable<typename RevBayesCore::Taxon>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "Clade" )
    {
        makeDeterministicVariable<typename RevBayesCore::Clade>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "Topology" )
    {
        makeDeterministicVariable<typename RevBayesCore::Topology>( userFunctionCall, userFunctionArgs, retValue );
    }
#endif
    else if ( internalValueType == "BranchLengthTree" )
    {
        makeDeterministicVariable<typename RevBayesCore::BranchLengthTree>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "TimeTree" )
    {
        makeDeterministicVariable<typename RevBayesCore::TimeTree>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "AminoAcoidState" )
    {
        makeDeterministicVariable<typename RevBayesCore::AminoAcidState>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "DnaState" )
    {
        makeDeterministicVariable<typename RevBayesCore::DnaState>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "RnaState" )
    {
        makeDeterministicVariable<typename RevBayesCore::RnaState>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "StandardState" )
    {
        makeDeterministicVariable<typename RevBayesCore::StandardState>( userFunctionCall, userFunctionArgs, retValue );
    }
#if 0   // Missing printValue
    else if ( internalValueType == "ContinuousCharacterData" )
    {
        makeDeterministicVariable<typename RevBayesCore::ContinuousCharacterData>( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "DiscreteCharacterData<AminoAcidState>" )
    {
        makeDeterministicVariable<typename RevBayesCore::DiscreteCharacterData<RevBayesCore::AminoAcidState> >( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "DiscreteCharacterData<DnaState>" )
    {
        makeDeterministicVariable<typename RevBayesCore::DiscreteCharacterData<RevBayesCore::DnaState> >( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "DiscreteCharacterData<RnaState>" )
    {
        makeDeterministicVariable<typename RevBayesCore::DiscreteCharacterData<RevBayesCore::RnaState> >( userFunctionCall, userFunctionArgs, retValue );
    }
    else if ( internalValueType == "DiscreteCharacterData<StandardState>" )
    {
        makeDeterministicVariable<typename RevBayesCore::DiscreteCharacterData<RevBayesCore::StandardState> >( userFunctionCall, userFunctionArgs, retValue );
    }
#endif
    else
    {
        /* Unknown internal type  */
        delete retValue;
        delete userFunctionCall;
        delete userFunctionArgs;

        std::ostringstream o;
        o << "The internal value type '" << internalValueType << "' is not supported yet in user-defined functions." << std::endl;
        o << "This is a bug; please report it to the RevBayes issue tracker." << std::endl;
        throw RbException( o.str() );
    }

    return retValue;
    
}

    
/** Get class name of object */
const std::string& UserFunction::getClassName(void) { 
    
    static std::string rbClassName = "UserFunction";
    
	return rbClassName; 
}


/** Get a reference to the code for execution outside of this class */
const std::list<SyntaxElement*>& UserFunction::getCode(void) const {
    
    return *code;
}


/** Get class type spec describing type of object */
const TypeSpec& UserFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& UserFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& UserFunction::getArgumentRules(void) const {
    
    return *argumentRules;
}


/** Get return type */
const TypeSpec& UserFunction::getReturnType(void) const {
    
    return returnType;
}


/** Templated function to make a deterministic variable of the right type */
template<typename valueType>
void UserFunction::makeDeterministicVariable( UserFunctionCall* call, UserFunctionArgs* args, RevObject* retValue )
{
    TypedUserFunction<valueType>*  fxn      = new TypedUserFunction<valueType>( call );
    DeterministicNode<valueType>*  detNode  = new DeterministicNode<valueType>("", fxn, args );
    
    static_cast< ModelObject<valueType>* >( retValue )->setDagNode( detNode );
}

