
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "FileMonitor.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RlFileMonitor.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

FileMonitor::FileMonitor(void) : Monitor() {
    
}


/** Clone object */
FileMonitor* FileMonitor::clone(void) const {
    
	return new FileMonitor(*this);
}


void FileMonitor::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string& fn = static_cast<const RlString &>( filename->getValue() ).getValue();
    const std::string& sep = static_cast<const RlString &>( separator->getValue() ).getValue();
    int g = static_cast<const Natural &>( printgen->getValue() ).getValue();
    std::set<RevBayesCore::DagNode *> n;
    for (std::set<RbPtr<const Variable> >::iterator i = vars.begin(); i != vars.end(); ++i) {
        RevBayesCore::DagNode* node = (*i)->getValue().getValueNode();
        n.insert( node );
    }
    bool pp = static_cast<const RlBoolean &>( posterior->getValue() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getValue() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getValue() ).getValue();
    bool app = static_cast<const RlBoolean &>( append->getValue() ).getValue();
    bool ci = static_cast<const RlBoolean &>( chainIdx->getValue() ).getValue();
    bool ch = static_cast<const RlBoolean &>( chainHeat->getValue() ).getValue();
    
    value = new RevBayesCore::FileMonitor(n, (unsigned long)g, fn, sep, pp, l, pr, app, ci, ch);
}


/** Get class name of object */
const std::string& FileMonitor::getClassName(void) { 
    
    static std::string rbClassName = "Filemonitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& FileMonitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& FileMonitor::getMemberRules(void) const {
    
    static MemberRules filemonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        filemonitorMemberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        filemonitorMemberRules.push_back( new ArgumentRule("printgen", true, Natural::getClassTypeSpec(), new Natural(1) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("separator", true, RlString::getClassTypeSpec(), new RlString(" ") ) );
        filemonitorMemberRules.push_back( new ArgumentRule("posterior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("likelihood", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("prior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("append", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("chainIdx", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("chainHeat", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        filemonitorMemberRules.push_back( new Ellipsis( RbLanguageObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return filemonitorMemberRules;
}

/** Get type spec */
const TypeSpec& FileMonitor::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void FileMonitor::printValue(std::ostream &o) const {
    
    o << "FileMonitor";
}


/** Set a member variable */
void FileMonitor::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "" ) {
        vars.insert( var );
    }
    else if ( name == "filename" ) {
        filename = var;
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
    else if (name == "chainIdx") {
        chainIdx = var;
    }
    else if (name == "chainHeat") {
        chainHeat = var;
    }
    else if (name == "append") {
        append = var;
    }
    else {
        RbLanguageObject::setConstMemberVariable(name, var);
    }
}
