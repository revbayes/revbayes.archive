#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Mntr_Model.h"
#include "Model.h"
#include "ModelMonitor.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlModel.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Mntr_Model::Mntr_Model(void) : Monitor() 
{
    
}


/** Clone object */
Mntr_Model* Mntr_Model::clone(void) const 
{
    
	return new Mntr_Model(*this);
}


void Mntr_Model::constructInternalObject( void ) 
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
    bool                                pp      = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool                                l       = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool                                pr      = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
    bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
    bool                                so      = static_cast<const RlBoolean &>( stochOnly->getRevObject() ).getValue();
    RevBayesCore::ModelMonitor *m = new RevBayesCore::ModelMonitor((unsigned long)g, fn, sep);
    
    // now set the flags
    m->setAppend( ap );
    m->setPrintLikelihood( l );
    m->setPrintPosterior( pp );
    m->setPrintPrior( pr );
    m->setStochasticNodesOnly( so );
    
    // store the new model into our value variable
    value = m;
}


/** Get Rev type of object */
const std::string& Mntr_Model::getClassType(void) 
{ 
    
    static std::string revType = "Mntr_Model";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_Model::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Mntr_Model::getParameterRules(void) const {
    
    static MemberRules modelMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        modelMonitorMemberRules.push_back( new ArgumentRule("filename"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("printgen"      , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("separator"     , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("posterior"     , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("likelihood"    , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("prior"         , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("append"        , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("stochasticOnly", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        
        rulesSet = true;
    }
    
    return modelMonitorMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_Model::getTypeSpec( void ) const 
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_Model::printValue(std::ostream &o) const 
{
    
    o << "Mntr_Model";
}


/** Set a member variable */
void Mntr_Model::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) 
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
    else if ( name == "prior" ) 
    {
        prior = var;
    }
    else if ( name == "posterior" ) 
    {
        posterior = var;
    }
    else if ( name == "likelihood" ) 
    {
        likelihood = var;
    }
    else if ( name == "append" ) 
    {
        append = var;
    }
    else if ( name == "stochasticOnly" ) 
    {
        stochOnly = var;
    }
    else 
    {
        Monitor::setConstParameter(name, var);
    }
    
}
