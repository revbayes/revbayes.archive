
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "EssMax.h"
#include "MinEssStoppingRule.h"
#include "OptionRule.h"
#include "RlMinEssStoppingRule.h"
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
MinEssStoppingRule::MinEssStoppingRule(void) : StoppingRule()
{
    
}


/**
 * Clone object
 */
MinEssStoppingRule* MinEssStoppingRule::clone(void) const
{
    
    return new MinEssStoppingRule(*this);
}


void MinEssStoppingRule::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new stopping rule
    double min = static_cast<const RealPos &>( minEss->getRevObject() ).getValue();
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
    
    value = new RevBayesCore::MinEssStoppingRule(min, fn, size_t(fq), burninEst);
}


/** Get Rev type of object */
const std::string& MinEssStoppingRule::getClassType(void)
{
    
    static std::string revType = "MinEssStoppingRule";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& MinEssStoppingRule::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( StoppingRule::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules */
const MemberRules& MinEssStoppingRule::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "minEss"   , RealPos::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
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
const TypeSpec& MinEssStoppingRule::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void MinEssStoppingRule::printValue(std::ostream &o) const
{
    
    o << "MinEssStoppingRule";
}


/** Set a member variable */
void MinEssStoppingRule::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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
    else if ( name == "minEss" )
    {
        minEss = var;
    }
    else
    {
        StoppingRule::setConstParameter(name, var);
    }
    
}
