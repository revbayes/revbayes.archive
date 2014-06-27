
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlScreenMonitor.h"
#include "RlString.h"
#include "ScreenMonitor.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

ScreenMonitor::ScreenMonitor(void) : Monitor() {
    
}


/** Clone object */
ScreenMonitor* ScreenMonitor::clone(void) const {
    
	return new ScreenMonitor(*this);
}


void ScreenMonitor::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate space for a new ScreenMonitor object
    const std::string& sep = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int g = static_cast<const Natural &>( printgen->getRevObject() ).getValue();
    std::set<RevBayesCore::DagNode *> n;
    for (std::set<RevPtr<const Variable> >::iterator i = vars.begin(); i != vars.end(); ++i) {
        RevBayesCore::DagNode* node = (*i)->getRevObject().getDagNode();
        n.insert( node );
    }
    bool pp = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
    value = new RevBayesCore::ScreenMonitor(n, g, sep, pp, l, pr);
}


/** Get class name of object */
const std::string& ScreenMonitor::getClassName(void) { 
    
    static std::string rbClassName = "Mntr_Screen";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ScreenMonitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& ScreenMonitor::getMemberRules(void) const {
    
    static MemberRules filemonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        filemonitorMemberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        filemonitorMemberRules.push_back( new ArgumentRule("printgen", true, Natural::getClassTypeSpec(), new Natural(1) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("separator", true, RlString::getClassTypeSpec(), new RlString(" ") ) );
        filemonitorMemberRules.push_back( new ArgumentRule("posterior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("likelihood", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("prior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        
        rulesSet = true;
    }
    
    return filemonitorMemberRules;
}

/** Get type spec */
const TypeSpec& ScreenMonitor::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void ScreenMonitor::printValue(std::ostream &o) const {
    
    o << "ScreenMonitor";
}


/** Set a member variable */
void ScreenMonitor::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "" ) {
        vars.insert( var );
    }
    else if ( name == "separator" ) {
        separator = var;
    }
    else if ( name == "printgen" ) {
        printgen = var;
    }
    else if ( name == "prior" ) {
        prior = var;
    }
    else if ( name == "posterior" ) {
        posterior = var;
    }
    else if ( name == "likelihood" ) {
        likelihood = var;
    }
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}
