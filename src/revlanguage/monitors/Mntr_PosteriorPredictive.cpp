#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Mntr_PosteriorPredictive.h"
#include "Model.h"
#include "PosteriorPredictiveMonitor.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlModel.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Mntr_PosteriorPredictive::Mntr_PosteriorPredictive(void) : Monitor()
{
    
}


/** Clone object */
Mntr_PosteriorPredictive* Mntr_PosteriorPredictive::clone(void) const
{
    
    return new Mntr_PosteriorPredictive(*this);
}


void Mntr_PosteriorPredictive::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
    bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
    RevBayesCore::PosteriorPredictiveMonitor *m = new RevBayesCore::PosteriorPredictiveMonitor((unsigned long)g, fn, sep);
    
    // now set the flags
    m->setAppend( ap );
    
    // store the new PosteriorPredictive into our value variable
    value = m;
}


/** Get Rev type of object */
const std::string& Mntr_PosteriorPredictive::getClassType(void)
{
    
    static std::string revType = "Mntr_PosteriorPredictive";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_PosteriorPredictive::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Mntr_PosteriorPredictive::getParameterRules(void) const {
    
    static MemberRules PosteriorPredictiveMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        PosteriorPredictiveMonitorMemberRules.push_back( new ArgumentRule("filename"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
        PosteriorPredictiveMonitorMemberRules.push_back( new ArgumentRule("printgen"      , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        PosteriorPredictiveMonitorMemberRules.push_back( new ArgumentRule("separator"     , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        PosteriorPredictiveMonitorMemberRules.push_back( new ArgumentRule("append"        , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        rulesSet = true;
    }
    
    return PosteriorPredictiveMonitorMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_PosteriorPredictive::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_PosteriorPredictive::printValue(std::ostream &o) const
{
    
    o << "Mntr_PosteriorPredictive";
}


/** Set a member variable */
void Mntr_PosteriorPredictive::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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
