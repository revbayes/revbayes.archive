#include "DistributionFunctionCdf.h"

#include "ArgumentRule.h"
#include "CummulativeDistributionFunction.h"
#include "DeterministicNode.h"
#include "DistributionFunctionPdf.h"
#include "Probability.h"
#include "ProbabilityDensityFunction.h"
#include "Real.h"
#include "RevObject.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
DistributionFunctionCdf::DistributionFunctionCdf( ContinuousDistribution *d ) : TypedFunction<Probability>(),
    templateObject( d ),
    templateObjectPositive( NULL )
{
    
    argRules.push_back( new ArgumentRule("x", Real::getClassTypeSpec(), "The value for which to compute the probability.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    const ArgumentRules &memberRules = templateObject->getParameterRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it)
    {
        argRules.push_back( (*it)->clone() );
    }
    
}

/** Constructor */
DistributionFunctionCdf::DistributionFunctionCdf( PositiveContinuousDistribution *d ) : TypedFunction<Probability>(),
    templateObject( NULL ),
    templateObjectPositive( d )
{
    
    argRules.push_back( new ArgumentRule("x", Real::getClassTypeSpec(), "The value for which to compute the probability.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    const ArgumentRules &memberRules = templateObjectPositive->getParameterRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it)
    {
        argRules.push_back( (*it)->clone() );
    }
    
}


/** Constructor */
DistributionFunctionCdf::DistributionFunctionCdf(const DistributionFunctionCdf& obj) : TypedFunction<Probability>(obj), argRules( obj.argRules )  {
    
    if ( obj.templateObject != NULL )
    {
        templateObject = obj.templateObject->clone();
    }
    else
    {
        templateObject = NULL;
    }
    
    if ( obj.templateObjectPositive != NULL )
    {
        templateObjectPositive = obj.templateObjectPositive->clone();
    }
    else
    {
        templateObjectPositive = NULL;
    }
    
}


DistributionFunctionCdf& DistributionFunctionCdf::operator=(const DistributionFunctionCdf &c) {
    
    if (this != &c)
    {
        Function::operator=(c);
        
        if ( c.templateObject != NULL )
        {
            templateObject = c.templateObject->clone();
        }
        else
        {
            templateObject = NULL;
        }
        
        if ( c.templateObjectPositive != NULL )
        {
            templateObjectPositive = c.templateObjectPositive->clone();
        }
        else
        {
            templateObjectPositive = NULL;
        }
        
        argRules = c.argRules;
    }
    
    return *this;
}


/** Clone the object */
DistributionFunctionCdf* DistributionFunctionCdf::clone(void) const
{
    
    return new DistributionFunctionCdf(*this);
}


RevBayesCore::TypedFunction<double>* DistributionFunctionCdf::createFunction( void ) const
{
    
    RevBayesCore::ContinuousDistribution *d = NULL;
    
    if ( templateObject != NULL )
    {
    
        ContinuousDistribution* copyObject = templateObject->clone();
    
        for ( size_t i = 1; i < args.size(); i++ )
        {
        
            if ( args[i].isConstant() )
            {
                copyObject->setConstParameter( args[i].getLabel(), RevPtr<const RevVariable>( (const RevVariable*) args[i].getVariable() ) );
            }
            else
            {
                copyObject->setParameter( args[i].getLabel(), args[i].getReferenceVariable() );
            }
        }
        
        d = copyObject->createDistribution();
        
    }
    else
    {
        
        
        PositiveContinuousDistribution* copyObject = templateObjectPositive->clone();
        
        for ( size_t i = 1; i < args.size(); i++ )
        {
            
            if ( args[i].isConstant() )
            {
                copyObject->setConstParameter( args[i].getLabel(), RevPtr<const RevVariable>( (const RevVariable*) args[i].getVariable() ) );
            }
            else
            {
                copyObject->setParameter( args[i].getLabel(), args[i].getReferenceVariable() );
            }
        }
        
        d = copyObject->createDistribution();
        
    }
    
    
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Probability &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::CummulativeDistributionFunction* f = new RevBayesCore::CummulativeDistributionFunction( arg, d );
    
    return f;
}


/** Get argument rules */
const ArgumentRules& DistributionFunctionCdf::getArgumentRules(void) const
{
    
    return argRules;
}


/** Get Rev type of object */
const std::string& DistributionFunctionCdf::getClassType(void)
{
    
    static std::string revType = "DistributionFunctionCdf";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunctionCdf::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get the help entry for this class */
RevBayesCore::RbHelpFunction* DistributionFunctionCdf::getHelpEntry( void ) const
{
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpFunction *help = NULL;

//    throw RbException("There is no help for a CDF");
    
    return help;
    
}


/** Get type spec */
const TypeSpec& DistributionFunctionCdf::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
