
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "EssMax.h"
#include "GelmanRubinStoppingRule.h"
#include "OptionRule.h"
#include "RlGelmanRubinStoppingRule.h"
#include "RealPos.h"
#include "RbException.h"
#include "RlString.h"
#include "SemMin.h"
#include "TypeSpec.h"


using namespace RevLanguage;

/**
 * Default constructor.
 * Create the default instance.
 */
GelmanRubinStoppingRule::GelmanRubinStoppingRule(void) : StoppingRule()
{
    
}


/**
 * Clone object
 */
GelmanRubinStoppingRule* GelmanRubinStoppingRule::clone(void) const
{
    
    return new GelmanRubinStoppingRule(*this);
}


void GelmanRubinStoppingRule::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new stopping rule
    double r = static_cast<const RealPos &>( R->getRevObject() ).getValue();
    int fq = static_cast<const Natural &>( frequency->getRevObject() ).getValue();
    const std::string &fn = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string &bm = static_cast<const RlString &>( burninMethod->getRevObject() ).getValue();
    
    RevBayesCore::BurninEstimatorContinuous *burninEst = NULL;
    
    if ( bm == "ESS" )
    {
        burninEst = new RevBayesCore::EssMax();
    }
    else if ( bm == "SEM" )
    {
        burninEst = new RevBayesCore::SemMin();
    }
    else
    {
        throw RbException("Unknown burnin estimation method");
    }
    
    value = new RevBayesCore::GelmanRubinStoppingRule(r, fn, size_t(fq), burninEst);
}


/** Get Rev type of object */
const std::string& GelmanRubinStoppingRule::getClassType(void)
{
    
    static std::string revType = "GelmanRubinStoppingRule";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& GelmanRubinStoppingRule::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( StoppingRule::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules */
const MemberRules& GelmanRubinStoppingRule::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "R"        , RealPos::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule( "filename" , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule( "frequency", Natural::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10000) ) );
        
        std::vector<std::string> bMethods;
        bMethods.push_back( "ESS" );
        bMethods.push_back( "SEM" );
        //        optionsUnits.push_back( "fixed" );
        memberRules.push_back( new OptionRule( "burninMethod", new RlString("ESS"), bMethods ) );
        
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& GelmanRubinStoppingRule::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void GelmanRubinStoppingRule::printValue(std::ostream &o) const
{
    
    o << "GelmanRubinStoppingRule";
}


/** Set a member variable */
void GelmanRubinStoppingRule::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "burninMethod" )
    {
        burninMethod = var;
    }
    else if ( name == "filename" )
    {
        filename = var;
    }
    else if ( name == "frequency" )
    {
        frequency = var;
    }
    else if ( name == "R" )
    {
        R = var;
    }
    else
    {
        StoppingRule::setConstParameter(name, var);
    }
    
}
