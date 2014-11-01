#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "TreeCharacterHistoryNhxMonitor.h"
#include "Mntr_CharacterHistoryNhxFile.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlAtlas.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "StandardState.h"
#include "TimeAtlas.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


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
    int mg = static_cast<const Natural &>( maxgen->getRevObject() ).getValue();
    double b = static_cast<const Probability &>( burnin->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *t = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData>* ctmc_tdn   = static_cast<const RevLanguage::AbstractDiscreteCharacterData&>( ctmc->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::AbstractDiscreteCharacterData>* ctmc_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::AbstractDiscreteCharacterData>* >(ctmc_tdn);
    
    const RevBayesCore::TimeAtlas* atl = &( static_cast<const RlAtlas&>( atlas->getRevObject() ).getValue() );
    
    bool pp = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
    
    bool ap = false; // append disabled for now
    bool sm = true; // show metadata disabled for now
    bool sr = false; // show rates
    
    std::string mt = static_cast<const RlString&>( type->getRevObject() ).getValue();
    if (mt == "std")
        ; // value = XXXXXX
    else if (mt == "biogeo")
        value = new RevBayesCore::TreeCharacterHistoryNhxMonitor<RevBayesCore::StandardState,RevBayesCore::TimeTree>(ctmc_sn, t, atl, size_t(g), (unsigned long)(mg), int(b*mg), fn, sep, pp, l, pr, ap, sm, sr);
}


/** Get class name of object */
const std::string& Mntr_CharacterHistoryNhxFile::getClassType(void) {
    
    static std::string revClassType = "Mntr_CharacterHistoryNhxFile";
    
	return revClassType;
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_CharacterHistoryNhxFile::getClassTypeSpec(void) {
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Mntr_CharacterHistoryNhxFile::getParameterRules(void) const {
    
    static MemberRules Mntr_CharacterHistoryNhxFileMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("filename"  , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("ctmc"      , AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("tree"      , TimeTree::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("atlas"     , RlAtlas::getClassTypeSpec()              , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("samplegen" , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new Natural(1) ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("maxgen"    , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("burnin"    , Probability::getClassTypeSpec()          , ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new Probability(0.2) ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("separator" , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlString(" ") ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("posterior" , RlBoolean::getClassTypeSpec()            , ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlBoolean(true) ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("likelihood", RlBoolean::getClassTypeSpec()            , ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlBoolean(true) ) );
        Mntr_CharacterHistoryNhxFileMemberRules.push_back( new ArgumentRule("prior"     , RlBoolean::getClassTypeSpec()            , ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlBoolean(true) ) );
        
        std::vector<std::string> options;
//        options.push_back( "std" );
        options.push_back( "biogeo" );
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
void Mntr_CharacterHistoryNhxFile::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
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
        maxgen = var;
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
        RevObject::setConstParameter(name, var);
    }
}
