#include "RlTrace.h"

#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RlUtils.h"

using namespace RevLanguage;

Trace::Trace() : WorkspaceToCoreWrapperObject<RevBayesCore::TraceNumeric>()
{

    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burnin", Natural::getClassTypeSpec(), "The number of samples to discregard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(0)) );
    methods.addFunction( new MemberProcedure( "summarize", RlUtils::Void, summarizeArgRules) );

}


Trace::Trace(const RevBayesCore::TraceNumeric &t) : WorkspaceToCoreWrapperObject<RevBayesCore::TraceNumeric>( new RevBayesCore::TraceNumeric( t ) )
{

    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burnin", Natural::getClassTypeSpec(), "The number of samples to discregard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(0)) );
    methods.addFunction( new MemberProcedure( "summarize", RlUtils::Void, summarizeArgRules) );

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Trace* Trace::clone(void) const
{
    
	return new Trace(*this);
}


void Trace::computeStatistics( void )
{
    value->computeStatistics();
}


void Trace::constructInternalObject( void )
{
    
    throw RbException("We do not support a constructor function for Trace.");
}


/* Map calls to member methods */
RevPtr<RevVariable> Trace::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "summarize")
    {
        found = true;
        
//        int b = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
//        RevBayesCore::TreeSummary<RevBayesCore::Tree> summary = RevBayesCore::TreeSummary<RevBayesCore::Tree>( *this->value );
//        summary.summarize( b );
//        summary.printTreeSummary(std::cerr);
        
        return NULL;
    } 
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& Trace::getClassType(void) { 
    
    static std::string rev_type = "Trace";
    
	return rev_type; 
}

/** Get class type spec describing type of object */
const TypeSpec& Trace::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::TraceNumeric>::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}



/** Return member rules (no members) */
const MemberRules& Trace::getParameterRules(void) const
{
    
    static MemberRules modelMemberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        rules_set = true;
    }
    
    return modelMemberRules;
}


/** Get type spec */
const TypeSpec& Trace::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get type spec */
void Trace::printValue(std::ostream &o) const
{
    
    o << "Filename:                  " << value->getFileName() << std::endl;
    o << "Parameter:                 " << value->getParameterName() << std::endl;
    o << "Samples:                   " << value->getSamples() << std::endl;
    o << "Stepsize:                  " << value->getStepSize() << std::endl;
    o << "ESS:                       " << value->getEss() << std::endl;
    o << "Burnin:                    " << value->getBurnin() << std::endl;
    o << "Mean:                      " << value->getMean() << std::endl;
    o << "SEM:                       " << value->getSem() << std::endl;
    o << "Geweke-test:               " << (value->hasPassedGewekeTest() ? "TRUE" : "FALSE") << std::endl;
    o << "stationarity-test:         " << (value->hasPassedStationarityTest() ? "TRUE" : "FALSE") << std::endl;
//    o << "Heidelberger-Welch-test:   " << (value->hasPassedHeidelbergerWelchTest() ? "TRUE" : "FALSE") << std::endl;
}


/** Set a member variable */
void Trace::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "xxx") {
        
    } 
    else {
        RevObject::setConstParameter(name, var);
    }
}


std::ostream& RevLanguage::operator<<(std::ostream& o, const RevLanguage::Trace& x) {
//    o << x.getParameterName();
    o << "Trace (";
//    const std::vector<double>& values = x.getValues();
//    for (std::vector<double>::const_iterator it = values.begin(); it != values.end(); ++it) {
//        if ( it != values.begin() ) {
//            o << ", ";
//        }
//        o << *it;
//    }
    o << ")";
    
    return o;
}
