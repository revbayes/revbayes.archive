#include "Mntr_AncestralState.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "OptionRule.h"
#include "Model.h"
#include "AncestralStateMonitor.h"
#include "RlMonitor.h"
#include "ModelVector.h"
#include "Ellipsis.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlModel.h"
#include "RlTimeTree.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "NaturalNumbersState.h"
#include "DnaState.h"

using namespace RevLanguage;

Mntr_AncestralState::Mntr_AncestralState(void) : Monitor()
{
    
}


/** Clone object */
Mntr_AncestralState* Mntr_AncestralState::clone(void) const
{
    
    return new Mntr_AncestralState(*this);
}


void Mntr_AncestralState::constructInternalObject( void )
{
    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* t = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::DagNode*				ch		= ctmc->getRevObject().getDagNode();
    bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
    std::string							character = static_cast<const RlString &>( monitorType->getRevObject() ).getValue();
    
    delete value;
    if (character == "NaturalNumbers") {
        
        RevBayesCore::AncestralStateMonitor<RevBayesCore::NaturalNumbersState> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::NaturalNumbersState>(t, ch, (unsigned long)g, fn, sep);
        m->setAppend( ap );
        value = m;
        
    } else if (character == "DNA") {
        
        RevBayesCore::AncestralStateMonitor<RevBayesCore::DnaState> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::DnaState>(t, ch, (unsigned long)g, fn, sep);
        m->setAppend( ap );
        value = m;
        
    } else {
        throw RbException( "Incorrect character type specified. Valid options are: NaturalNumbers, DNA" );
    }
    
    
    
    
}


/** Get Rev type of object */
const std::string& Mntr_AncestralState::getClassType(void)
{
    
    static std::string revType = "Mntr_AncestralState";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_AncestralState::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Mntr_AncestralState::getParameterRules(void) const
{
    
    static MemberRules asMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        asMonitorMemberRules.push_back( new ArgumentRule("tree"          , Tree::getClassTypeSpec()     , "The tree which we monitor.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        asMonitorMemberRules.push_back( new ArgumentRule("ctmc"          , RevObject::getClassTypeSpec(), "The CTMC process.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        asMonitorMemberRules.push_back( new ArgumentRule("filename"      , RlString::getClassTypeSpec() , "The name of the file for storing the samples.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        asMonitorMemberRules.push_back( new ArgumentRule("type"          , RlString::getClassTypeSpec() , "The type of data to store.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        asMonitorMemberRules.push_back( new ArgumentRule("printgen"      , Natural::getClassTypeSpec()  , "The frequency how often to sample.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        asMonitorMemberRules.push_back( new ArgumentRule("separator"     , RlString::getClassTypeSpec() , "The separator between columns in the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        asMonitorMemberRules.push_back( new ArgumentRule("append"        , RlBoolean::getClassTypeSpec(), "Should we append or overwrite if the file exists?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        rulesSet = true;
    }
    
    return asMonitorMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_AncestralState::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_AncestralState::printValue(std::ostream &o) const
{
    
    o << "Mntr_AncestralState";
}


/** Set a member variable */
void Mntr_AncestralState::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "" ) {
        vars.push_back( var );
    }
    else if ( name == "filename" )
    {
        filename = var;
    }
    else if ( name == "separator" )
    {
        separator = var;
    }
    else if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "type" )
    {
        monitorType = var;
    }
    else if ( name == "ctmc" )
    {
        ctmc = var;
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
