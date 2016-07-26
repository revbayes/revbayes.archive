#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "Probability.h"
#include "RlClade.h"
#include "RlTraceTree.h"
#include "RlUtils.h"
#include "TreeSummary.h"


TraceTree::TraceTree() : WorkspaceToCoreWrapperObject<RevBayesCore::TraceTree>()
{
    
    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), "The fraction of samples to disregard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    summarizeArgRules->push_back( new ArgumentRule("credibleTreeSetSize", Probability::getClassTypeSpec(), "The size of the credible set to print.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95)) );
    summarizeArgRules->push_back( new ArgumentRule("minCladeProbability", Probability::getClassTypeSpec(), "The minimum clade probability used when printing.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.05)) );
    this->methods.addFunction( new MemberProcedure( "summarize", RlUtils::Void, summarizeArgRules) );

    ArgumentRules* cladeProbArgRules = new ArgumentRules();
    cladeProbArgRules->push_back( new ArgumentRule("clade", Clade::getClassTypeSpec(), "The (monophyletic) clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY) );
    cladeProbArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), "The fraction of samples to disregard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    this->methods.addFunction( new MemberProcedure( "cladeProbability", Probability::getClassTypeSpec(), cladeProbArgRules) );

}



TraceTree::TraceTree(const RevBayesCore::TraceTree &m) : WorkspaceToCoreWrapperObject<RevBayesCore::TraceTree>( new RevBayesCore::TraceTree( m ) )
{
    
    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), "The fraction of samples to disregard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    summarizeArgRules->push_back( new ArgumentRule("credibleTreeSetSize", Probability::getClassTypeSpec(), "The size of the credible set to print.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95)) );
    summarizeArgRules->push_back( new ArgumentRule("minCladeProbability", Probability::getClassTypeSpec(), "The minimum clade probability used when printing.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.05)) );
    this->methods.addFunction( new MemberProcedure( "summarize", RlUtils::Void, summarizeArgRules) );
    
    ArgumentRules* cladeProbArgRules = new ArgumentRules();
    cladeProbArgRules->push_back( new ArgumentRule("clade", Clade::getClassTypeSpec(), "The (monophyletic) clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY) );
    cladeProbArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), "The fraction of samples to disregard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    this->methods.addFunction( new MemberProcedure( "cladeProbability", Probability::getClassTypeSpec(), cladeProbArgRules) );

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */

TraceTree* TraceTree::clone(void) const
{
    
    return new TraceTree(*this);
}



void TraceTree::constructInternalObject( void )
{
    throw RbException("We do not support a constructor function for TraceTree.");
}


/* Map calls to member methods */

RevPtr<RevVariable> TraceTree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if ( name == "summarize" )
    {
        found = true;
        
        double f            = static_cast<const Probability &>( args[0].getVariable()->getRevObject() ).getValue();
        double treeCI       = static_cast<const Probability &>( args[1].getVariable()->getRevObject() ).getValue();
        double minCladeProb = static_cast<const Probability &>( args[2].getVariable()->getRevObject() ).getValue();
        RevBayesCore::TreeSummary summary = RevBayesCore::TreeSummary( *this->value );
        int b = int( floor( this->value->size()*f ) );
        summary.setBurnin( b );
        summary.summarizeTrees();
        summary.printTreeSummary(std::cout, treeCI);
        summary.summarizeClades( true );
        summary.printCladeSummary(std::cout, minCladeProb);
        
        return NULL;
    }
    else if ( name == "cladeProbability" )
    {
        found = true;
        
        RevBayesCore::TreeSummary summary = RevBayesCore::TreeSummary( *this->value );
        
        const RevBayesCore::Clade &c    = static_cast<const Clade &>( args[0].getVariable()->getRevObject() ).getValue();
        double f                        = static_cast<const Probability &>( args[1].getVariable()->getRevObject() ).getValue();

        
        int b = int( floor( this->value->size()*f ) );
        summary.setBurnin( b );
        double p = summary.cladeProbability( c );
        
        return new RevVariable( new Probability( p ) );

    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */

const std::string& TraceTree::getClassType(void)
{
    
    static std::string revType = "TraceTree";
    
    return revType;
}

/** Get class type spec describing type of object */

const TypeSpec& TraceTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::TraceTree>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */

const MemberRules& TraceTree::getParameterRules(void) const
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

const TypeSpec& TraceTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */

void TraceTree::printValue(std::ostream &o) const
{
    
    o << "TraceTree";
}


/** Set a member variable */

void TraceTree::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "xxx") {
        
    }
    else {
        RevObject::setConstParameter(name, var);
    }
}
