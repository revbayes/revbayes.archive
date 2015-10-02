#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "Probability.h"
#include "RlTreeTrace.h"
#include "RlUtils.h"
#include "TreeSummary.h"


TreeTrace::TreeTrace() : WorkspaceToCoreWrapperObject<RevBayesCore::TreeTrace>()
{
    
    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    summarizeArgRules->push_back( new ArgumentRule("credibleTreeSetSize", Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95)) );
    summarizeArgRules->push_back( new ArgumentRule("minCladeProbability", Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.05)) );
    this->methods.addFunction("summarize", new MemberProcedure( RlUtils::Void, summarizeArgRules) );
    
}



TreeTrace::TreeTrace(const RevBayesCore::TreeTrace &m) : WorkspaceToCoreWrapperObject<RevBayesCore::TreeTrace>( new RevBayesCore::TreeTrace( m ) )
{
    
    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    summarizeArgRules->push_back( new ArgumentRule("credibleTreeSetSize", Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95)) );
    summarizeArgRules->push_back( new ArgumentRule("minCladeProbability", Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.05)) );
    this->methods.addFunction("summarize", new MemberProcedure( RlUtils::Void, summarizeArgRules) );
    
}


/** Clone object */

TreeTrace* TreeTrace::clone(void) const
{
    
    return new TreeTrace(*this);
}



void TreeTrace::constructInternalObject( void )
{
    throw RbException("We do not support a constructor function for TreeTrace.");
}


/* Map calls to member methods */

RevPtr<RevVariable> TreeTrace::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "summarize")
    {
        found = true;
        
        double f            = static_cast<const Probability &>( args[0].getVariable()->getRevObject() ).getValue();
        double treeCI       = static_cast<const Probability &>( args[1].getVariable()->getRevObject() ).getValue();
        double minCladeProb = static_cast<const Probability &>( args[2].getVariable()->getRevObject() ).getValue();
        RevBayesCore::TreeSummary summary = RevBayesCore::TreeSummary( *this->value );
        int b = int( floor( this->value->size()*f ) );
        summary.summarizeTrees( b );
        summary.printTreeSummary(std::cout, treeCI);
        summary.summarizeClades( b );
        summary.printCladeSummary(std::cout, minCladeProb);
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */

const std::string& TreeTrace::getClassType(void)
{
    
    static std::string revType = "TreeTrace";
    
    return revType;
}

/** Get class type spec describing type of object */

const TypeSpec& TreeTrace::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::TreeTrace>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */

const MemberRules& TreeTrace::getParameterRules(void) const
{
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/** Get type spec */

const TypeSpec& TreeTrace::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */

void TreeTrace::printValue(std::ostream &o) const
{
    
    o << "TreeTrace";
}


/** Set a member variable */

void TreeTrace::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "xxx") {
        
    }
    else {
        RevObject::setConstParameter(name, var);
    }
}
