
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ExtendedNewickTreeMonitor.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlExtendedNewickFileMonitor.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

ExtendedNewickFileMonitor::ExtendedNewickFileMonitor(void) : Monitor() {
    
}


/** Clone object */
ExtendedNewickFileMonitor* ExtendedNewickFileMonitor::clone(void) const {
    
	return new ExtendedNewickFileMonitor(*this);
}


void ExtendedNewickFileMonitor::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string& fn = static_cast<const RlString &>( filename->getValue() ).getValue();
    const std::string& sep = static_cast<const RlString &>( separator->getValue() ).getValue();
    int g = static_cast<const Natural &>( printgen->getValue() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *t = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    std::set<RevBayesCore::TypedDagNode<std::vector<double> > *> n;
    for (std::set<RbPtr<const Variable> >::iterator i = vars.begin(); i != vars.end(); ++i) {
        RevBayesCore::TypedDagNode<std::vector<double> >* node = static_cast< const Vector<Real> & >((*i)->getValue()).getValueNode();
        n.insert( node );
    }
    bool pp = static_cast<const RlBoolean &>( posterior->getValue() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getValue() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getValue() ).getValue();
    value = new RevBayesCore::ExtendedNewickTreeMonitor(t, n, size_t(g), fn, sep, pp, l, pr);
}


/** Get class name of object */
const std::string& ExtendedNewickFileMonitor::getClassName(void) { 
    
    static std::string rbClassName = "ExtendedNewickFileMonitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ExtendedNewickFileMonitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& ExtendedNewickFileMonitor::getMemberRules(void) const {
    
    static MemberRules ExtendedNewickFileMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        ExtendedNewickFileMonitorMemberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        ExtendedNewickFileMonitorMemberRules.push_back( new ArgumentRule("tree", true, TimeTree::getClassTypeSpec() ) );
        ExtendedNewickFileMonitorMemberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        ExtendedNewickFileMonitorMemberRules.push_back( new ArgumentRule("printgen", true, Natural::getClassTypeSpec(), new Natural(1) ) );
        ExtendedNewickFileMonitorMemberRules.push_back( new ArgumentRule("separator", true, RlString::getClassTypeSpec(), new RlString(" ") ) );
        ExtendedNewickFileMonitorMemberRules.push_back( new ArgumentRule("posterior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        ExtendedNewickFileMonitorMemberRules.push_back( new ArgumentRule("likelihood", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        ExtendedNewickFileMonitorMemberRules.push_back( new ArgumentRule("prior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        
        rulesSet = true;
    }
    
    return ExtendedNewickFileMonitorMemberRules;
}

/** Get type spec */
const TypeSpec& ExtendedNewickFileMonitor::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void ExtendedNewickFileMonitor::printValue(std::ostream &o) const {
    
    o << "ExtendedNewickFileMonitor";
}


/** Set a member variable */
void ExtendedNewickFileMonitor::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "" ) {
        vars.insert( var );
    }
    else if ( name == "filename" ) {
        filename = var;
    }
    else if ( name == "tree" ) {
        tree = var;
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
