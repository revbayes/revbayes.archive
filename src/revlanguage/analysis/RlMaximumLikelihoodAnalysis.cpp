#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "MaxIterationStoppingRule.h"
#include "MaximumLikelihoodAnalysis.h"
#include "Model.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlMaximumLikelihoodAnalysis.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlStoppingRule.h"
#include "RlString.h"
#include "StoppingRule.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

MaximumLikelihoodAnalysis::MaximumLikelihoodAnalysis(void) : WorkspaceToCoreWrapperObject<RevBayesCore::MaximumLikelihoodAnalysis>( )
{
    
    initializeMethods();
    
}

MaximumLikelihoodAnalysis::MaximumLikelihoodAnalysis(RevBayesCore::MaximumLikelihoodAnalysis *m) : WorkspaceToCoreWrapperObject<RevBayesCore::MaximumLikelihoodAnalysis>( m )
{
    
    initializeMethods();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
MaximumLikelihoodAnalysis* MaximumLikelihoodAnalysis::clone(void) const
{
    
    return new MaximumLikelihoodAnalysis(*this);
}


/* Map calls to member methods */
RevPtr<RevVariable> MaximumLikelihoodAnalysis::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "run")
    {
        found = true;
        
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        
        value->run( gen );
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& MaximumLikelihoodAnalysis::getClassType(void)
{
    
    static std::string revType = "MaximumLikelihoodAnalysis";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& MaximumLikelihoodAnalysis::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::MaximumLikelihoodAnalysis>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& MaximumLikelihoodAnalysis::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("model"   , Model::getClassTypeSpec()                   , "The model graph.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule("monitors", WorkspaceVector<Monitor>::getClassTypeSpec(), "The monitors used for this analysis.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule("moves"   , WorkspaceVector<Move>::getClassTypeSpec()   , "The moves used for this analysis.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        std::vector<std::string> options;
        options.push_back( "sequential" );
        options.push_back( "random" );
        options.push_back( "single" );
        
        memberRules.push_back( new OptionRule( "moveschedule", new RlString( "random" ), options, "The strategy how the moves are used." ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& MaximumLikelihoodAnalysis::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


void MaximumLikelihoodAnalysis::initializeMethods()
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule( "generations", Natural::getClassTypeSpec(), "The number of generations to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );
    
}


/**
 * Print value
 */
void MaximumLikelihoodAnalysis::printValue(std::ostream &o) const
{
    
    o << "MaximumLikelihoodAnalysis";
}


/**
 * Set a member variable
 */
void MaximumLikelihoodAnalysis::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "model")
    {
        model = var;
    }
    else if ( name == "moves")
    {
        moves = var;
    }
    else if ( name == "monitors")
    {
        monitors = var;
    }
    else if ( name == "moveschedule")
    {
        moveschedule = var;
    }
    else
    {
        WorkspaceToCoreWrapperObject<RevBayesCore::MaximumLikelihoodAnalysis>::setConstParameter(name, var);
    }
    
}
