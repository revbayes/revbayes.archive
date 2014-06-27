#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Model.h"
#include "ModelMonitor.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlModel.h"
#include "RlModelMonitor.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

ModelMonitor::ModelMonitor(void) : Monitor() 
{
    
}


/** Clone object */
ModelMonitor* ModelMonitor::clone(void) const 
{
    
	return new ModelMonitor(*this);
}


void ModelMonitor::constructInternalObject( void ) 
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


/** Get class name of object */
const std::string& ModelMonitor::getClassName(void) 
{ 
    
    static std::string rbClassName = "Mntr_Model";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ModelMonitor::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& ModelMonitor::getMemberRules(void) const {
    
    static MemberRules modelMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        modelMonitorMemberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("printgen", true, Natural::getClassTypeSpec(), new Natural(1) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("separator", true, RlString::getClassTypeSpec(), new RlString(" ") ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("posterior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("likelihood", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("prior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("append", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("stochasticOnly", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        
        
        rulesSet = true;
    }
    
    return modelMonitorMemberRules;
}

/** Get type spec */
const TypeSpec& ModelMonitor::getTypeSpec( void ) const 
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void ModelMonitor::printValue(std::ostream &o) const 
{
    
    o << "ModelMonitor";
}


/** Set a member variable */
void ModelMonitor::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
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
        Monitor::setConstMemberVariable(name, var);
    }
    
}
