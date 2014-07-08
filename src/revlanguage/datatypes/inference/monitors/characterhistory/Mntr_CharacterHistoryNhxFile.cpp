#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "ConstantNode.h"
#include "TreeCharacterHistoryNhxMonitor.h"
#include "Mntr_CharacterHistoryNhxFile.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlAbstractCharacterData.h"
#include "RlAtlas.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "StandardState.h"
#include "TimeAtlas.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

Mntr_CharacterHistoryNhxFile::Mntr_CharacterHistoryNhxFile(void) : Monitor() {
    
}


/** Clone object */
Mntr_CharacterHistoryNhxFile* Mntr_CharacterHistoryNhxFile::clone(void) const {
    
	return new Mntr_CharacterHistoryNhxFile(*this);
}


void Mntr_CharacterHistoryNhxFile::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string& fn = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string& sep = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int g = static_cast<const Natural &>( samplegen->getRevObject() ).getValue();
    int mg = static_cast<const Natural &>( samplegen->getRevObject() ).getValue();
    double b = static_cast<const Probability &>( burnin->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *t = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData>* ctmc_tdn   = static_cast<const RevLanguage::AbstractCharacterData&>( ctmc->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::AbstractCharacterData>* ctmc_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::AbstractCharacterData>* >(ctmc_tdn);
    
    const RevBayesCore::TimeAtlas* atl = &( static_cast<const RlAtlas&>( atlas->getRevObject() ).getValue() );
    
    bool pp = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
    
    bool ap = false; // append disabled for now
    bool sm = false; // show metadata disabled for now
    bool sr = false; // show rates
    
    std::string mt = static_cast<const RlString&>( type->getRevObject() ).getValue();
    if (mt == "std")
        ; // value = XXXXXX
    else if (mt == "biogeo")
        value = new RevBayesCore::TreeCharacterHistoryNhxMonitor<RevBayesCore::StandardState,RevBayesCore::TimeTree>(ctmc_sn, t, atl, size_t(g), (unsigned long)(mg), int(b*mg), fn, sep, pp, l, pr, ap, sm, sr);
}


/** Get class name of object */
const std::string& Mntr_CharacterHistoryNhxFile::getClassName(void) {
    
    static std::string rbClassName = "Mntr_CharacterHistoryNhxFile";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_CharacterHistoryNhxFile::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& Mntr_CharacterHistoryNhxFile::getMemberRules(void) const {
    
    static MemberRules Mntr_CharacterHistoryNhxFileMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("ctmc", true, AbstractCharacterData::getClassTypeSpec() ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("tree", true, TimeTree::getClassTypeSpec() ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("atlas", true, RlAtlas::getClassTypeSpec() ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("samplegen", true, Natural::getClassTypeSpec(), new Natural(1) ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("maxgen", true, Natural::getClassTypeSpec() ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("burnin", true, Probability::getClassTypeSpec(), new Probability(0.2) ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("separator", true, RlString::getClassTypeSpec(), new RlString(" ") ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("posterior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("likelihood", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("prior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        Vector<RlString> options;
//        options.push_back( RlString("std") );
        options.push_back( RlString("biogeo") );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new OptionRule( "type", new RlString("biogeo"), options ) );
        
        rulesSet = true;
    }
    
    return Mntr_CharacterHistoryNhxFileMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_CharacterHistoryNhxFile::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_CharacterHistoryNhxFile::printValue(std::ostream &o) const {
    
    o << "Mntr_CharacterHistoryNhxFile";
}


/** Set a member variable */
void Mntr_CharacterHistoryNhxFile::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "" ) {
        vars.insert( var );
    }
    else if ( name == "filename" ) {
        filename = var;
    }
    else if ( name == "ctmc" ) {
        ctmc = var;
    }
    else if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "atlas" ) {
        atlas = var;
    }
    else if ( name == "separator" ) {
        separator = var;
    }
    else if ( name == "samplegen" ) {
        samplegen = var;
    }
    else if ( name == "maxgen" ) {
        samplegen = var;
    }
    else if ( name == "burnin" ) {
        burnin = var;
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
    else if ( name == "type" ) {
        type = var;
    }
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}
