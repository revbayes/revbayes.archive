#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Mntr_StochasticVariable.h"
#include "Model.h"
#include "StochasticVariableMonitor.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Mntr_StochasticVariable::Mntr_StochasticVariable(void) : Monitor()
{
    
}


/** Clone object */
Mntr_StochasticVariable* Mntr_StochasticVariable::clone(void) const
{
    
    return new Mntr_StochasticVariable(*this);
}


void Mntr_StochasticVariable::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
    bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
    RevBayesCore::StochasticVariableMonitor *m = new RevBayesCore::StochasticVariableMonitor((unsigned long)g, fn, sep);
    
    // now set the flags
    m->setAppend( ap );
    
    // store the new StochasticVariable into our value variable
    value = m;
}


/** Get Rev type of object */
const std::string& Mntr_StochasticVariable::getClassType(void)
{
    
    static std::string revType = "Mntr_StochasticVariable";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_StochasticVariable::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Mntr_StochasticVariable::getParameterRules(void) const {
    
    static MemberRules StochasticVariableMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        StochasticVariableMonitorMemberRules.push_back( new ArgumentRule("filename"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
        StochasticVariableMonitorMemberRules.push_back( new ArgumentRule("printgen"      , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        StochasticVariableMonitorMemberRules.push_back( new ArgumentRule("separator"     , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        StochasticVariableMonitorMemberRules.push_back( new ArgumentRule("append"        , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        
        rulesSet = true;
    }
    
    return StochasticVariableMonitorMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_StochasticVariable::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_StochasticVariable::printValue(std::ostream &o) const
{
    
    o << "Mntr_StochasticVariable";
}


/** Set a member variable */
void Mntr_StochasticVariable::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "filename" )
    {
        filename = var;
    }
    else if ( name == "separator" )
    {
        separator = var;
    }
    else if ( name == "printgen" )
    {
        printgen = var;
    }
    else if ( name == "append" )
    {
        append = var;
    }
    else
    {
        Monitor::setConstParameter(name, var);
    }
    
}
