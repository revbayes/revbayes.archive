
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MaxIterationStoppingRule.h"
#include "RlMaxIterationStoppingRule.h"
#include "RealPos.h"
#include "RbException.h"
#include "OptionRule.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

/**
 * Default constructor.
 * Create the default instance.
 */
MaxIterationStoppingRule::MaxIterationStoppingRule(void) : StoppingRule()
{
    
}


/**
 * Clone object
 */
MaxIterationStoppingRule* MaxIterationStoppingRule::clone(void) const
{
    
    return new MaxIterationStoppingRule(*this);
}


void MaxIterationStoppingRule::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new stopping rule
    double m = static_cast<const Natural &>( maxIteration->getRevObject() ).getValue();
    
    value = new RevBayesCore::MaxIterationStoppingRule(m);
}


/** Get Rev type of object */
const std::string& MaxIterationStoppingRule::getClassType(void)
{
    
    static std::string revType = "MaxIterationStoppingRule";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& MaxIterationStoppingRule::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( StoppingRule::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string MaxIterationStoppingRule::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "srMaxIteration";
    
    return c_name;
}


/** Return member rules */
const MemberRules& MaxIterationStoppingRule::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("maxIteration", Natural::getClassTypeSpec() , "The maximum number of iterations to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& MaxIterationStoppingRule::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void MaxIterationStoppingRule::printValue(std::ostream &o) const
{
    
    o << "MaxIterationStoppingRule";
}


/** Set a member variable */
void MaxIterationStoppingRule::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "maxIteration" )
    {
        maxIteration = var;
    }
    else
    {
        StoppingRule::setConstParameter(name, var);
    }
    
}
