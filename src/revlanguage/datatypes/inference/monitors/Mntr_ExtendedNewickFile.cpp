
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Ellipsis.h"
#include "ExtendedNewickTreeMonitor.h"
#include "Mntr_ExtendedNewickFile.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RevObject.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Mntr_ExtendedNewickFile::Mntr_ExtendedNewickFile(void) : Monitor() {
    
}


/** Clone object */
Mntr_ExtendedNewickFile* Mntr_ExtendedNewickFile::clone(void) const {
    
	return new Mntr_ExtendedNewickFile(*this);
}


void Mntr_ExtendedNewickFile::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string& fn = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string& sep = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int g = static_cast<const Natural &>( printgen->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *t = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    std::set<RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> > *> n;
    for (std::set<RevPtr<const Variable> >::iterator i = vars.begin(); i != vars.end(); ++i) {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* node = static_cast< const ModelVector<Real> & >((*i)->getRevObject()).getDagNode();
        n.insert( node );
    }
    bool pp = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
    value = new RevBayesCore::ExtendedNewickTreeMonitor(t, n, size_t(g), fn, sep, pp, l, pr);
}


/** Get Rev type of object */
const std::string& Mntr_ExtendedNewickFile::getClassType(void) { 
    
    static std::string revType = "Mntr_ExtendedNewickFile";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_ExtendedNewickFile::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Mntr_ExtendedNewickFile::getMemberRules(void) const {
    
    static MemberRules Mntr_ExtendedNewickFileMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
    
        Mntr_ExtendedNewickFileMemberRules.push_back( new ArgumentRule("filename", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        Mntr_ExtendedNewickFileMemberRules.push_back( new ArgumentRule("tree"    , TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        Mntr_ExtendedNewickFileMemberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        Mntr_ExtendedNewickFileMemberRules.push_back( new ArgumentRule("printgen"  , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        Mntr_ExtendedNewickFileMemberRules.push_back( new ArgumentRule("separator" , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString(" ") ) );
        Mntr_ExtendedNewickFileMemberRules.push_back( new ArgumentRule("posterior" , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        Mntr_ExtendedNewickFileMemberRules.push_back( new ArgumentRule("likelihood", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        Mntr_ExtendedNewickFileMemberRules.push_back( new ArgumentRule("prior"     , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        
        rulesSet = true;
    }
    
    return Mntr_ExtendedNewickFileMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_ExtendedNewickFile::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_ExtendedNewickFile::printValue(std::ostream &o) const {
    
    o << "Mntr_ExtendedNewickFile";
}


/** Set a member variable */
void Mntr_ExtendedNewickFile::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
