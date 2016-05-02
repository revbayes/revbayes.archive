#include "AbstractHomologousDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "RelativeNodeAgeWeightedConstraints.h"
#include "RelativeNodeAgeWeightedConstraintsReader.h"
#include "Ellipsis.h"
#include "Func_readRelativeNodeAgeWeightedConstraints.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "Real.h"
#include "RlRelativeNodeAgeWeightedConstraints.h"
#include "RlHomologousDiscreteCharacterData.h"
#include "RlString.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readRelativeNodeAgeWeightedConstraints* Func_readRelativeNodeAgeWeightedConstraints::clone( void ) const
{
    
    return new Func_readRelativeNodeAgeWeightedConstraints( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readRelativeNodeAgeWeightedConstraints::execute( void )
{
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    double th = static_cast<const Real&>( args[1].getVariable()->getRevObject() ).getValue();
    RevBayesCore::RelativeNodeAgeWeightedConstraintsReader* dmr = new RevBayesCore::RelativeNodeAgeWeightedConstraintsReader( fn.getValue(), '\t', 0, th );
    RevBayesCore::RelativeNodeAgeWeightedConstraints* dm = new RevBayesCore::RelativeNodeAgeWeightedConstraints(dmr);
    
    return new RevVariable( new RlRelativeNodeAgeWeightedConstraints(dm) );
}


/** Get argument rules */
const ArgumentRules& Func_readRelativeNodeAgeWeightedConstraints::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), "Relative or absolute name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "threshold", Real::getClassTypeSpec(), "weight threshold below which constraints are ignored.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
        
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readRelativeNodeAgeWeightedConstraints::getClassType(void)
{
    
    static std::string revType = "Func_readRelativeNodeAgeWeightedConstraints";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readRelativeNodeAgeWeightedConstraints::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readRelativeNodeAgeWeightedConstraints::getFunctionName( void ) const
{
    // create a name variable that is the same for all instances of this class
    std::string f_name = "readRelativeNodeAgeWeightedConstraints";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readRelativeNodeAgeWeightedConstraints::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readRelativeNodeAgeWeightedConstraints::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlRelativeNodeAgeWeightedConstraints::getClassTypeSpec();
    return returnTypeSpec;
}

