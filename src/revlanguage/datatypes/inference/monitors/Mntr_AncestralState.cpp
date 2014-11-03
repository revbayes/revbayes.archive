#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Mntr_AncestralState.h"
#include "Model.h"
#include "AncestralStateMonitor.h"
#include "RlMonitor.h"
#include "ModelVector.h"
#include "Ellipsis.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlModel.h"
#include "RlTree.h"
#include "RlTimeTree.h"
#include "RlBranchLengthTree.h"
#include "Tree.h"
#include "RlString.h"
#include "TypeSpec.h"


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
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
	RevBayesCore::TypedDagNode<RevBayesCore::Tree> *t = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::DagNode*				ch		= character->getRevObject().getDagNode();
	bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
    
	
	RevBayesCore::AncestralStateMonitor *m = new RevBayesCore::AncestralStateMonitor(t, ch, (unsigned long)g, fn, sep);
    
    // now set the flags
    m->setAppend( ap );

    // store the new model into our value variable
    value = m;
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
const MemberRules& Mntr_AncestralState::getMemberRules(void) const {
    
    static MemberRules modelMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
// TODO: use templates to use timetree or a branchlengthtree!!	
//       modelMonitorMemberRules.push_back( new ArgumentRule("tree"    , Tree::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
//		modelMonitorMemberRules.push_back( new ArgumentRule("timetree"    , TimeTree::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
		modelMonitorMemberRules.push_back( new ArgumentRule("branchlengthtree"    , BranchLengthTree::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
		modelMonitorMemberRules.push_back( new ArgumentRule("character"    , RevObject::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("filename"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("printgen"      , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("separator"     , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("append"        , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        
        rulesSet = true;
    }
    
    return modelMonitorMemberRules;
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
void Mntr_AncestralState::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
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
	else if ( name == "tree" || name == "timetree" || name == "branchlengthtree" ) {
        tree = var;
    }
	else if ( name == "character" ) 
    {
        character = var;
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
        Monitor::setConstMemberVariable(name, var);
    }
    
}
