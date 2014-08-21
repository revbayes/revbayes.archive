
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Ellipsis.h"
#include "FileMonitor.h"
#include "ModelVector.h"
#include "Mntr_File.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Mntr_File::Mntr_File(void) : Monitor() {
    
}


/** Clone object */
Mntr_File* Mntr_File::clone(void) const {
    
	return new Mntr_File(*this);
}


void Mntr_File::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string& fn = static_cast<const RlString &>( filename->getRevObject() ).getValue();
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
    bool app = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
    bool ci = static_cast<const RlBoolean &>( chainIdx->getRevObject() ).getValue();
    bool ch = static_cast<const RlBoolean &>( chainHeat->getRevObject() ).getValue();
    
    value = new RevBayesCore::FileMonitor(n, (unsigned long)g, fn, sep, pp, l, pr, app, ci, ch);
}


/** Get Rev type of object */
const std::string& Mntr_File::getClassType(void) { 
    
    static std::string revType = "Mntr_File";
    
	return revType; 
    }

/** Get class type spec describing type of object */
const TypeSpec& Mntr_File::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Mntr_File::getMemberRules(void) const {
    
    static MemberRules filemonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        filemonitorMemberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        filemonitorMemberRules.push_back( new ArgumentRule("printgen", true, Natural::getClassTypeSpec(), new Natural(1) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("separator", true, RlString::getClassTypeSpec(), new RlString("\t") ) );
        filemonitorMemberRules.push_back( new ArgumentRule("posterior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("likelihood", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("prior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("append", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("chainIdx", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("chainHeat", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        filemonitorMemberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return filemonitorMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_File::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_File::printValue(std::ostream &o) const {
    
    o << "Mntr_File";
}


/** Set a member variable */
void Mntr_File::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
        RevObject::setConstMemberVariable(name, var);
    }
}
