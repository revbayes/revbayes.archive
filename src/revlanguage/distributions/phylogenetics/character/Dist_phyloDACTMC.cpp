#include "Dist_phyloDACTMC.h"
#include "AminoAcidState.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "DnaState.h"
#include "GeneralTreeHistoryCtmc.h"
#include "OptionRule.h"
#include "RateMap.h"
#include "RevNullObject.h"
#include "RlString.h"
#include "RlTree.h"
#include "RnaState.h"
#include "StandardState.h"

using namespace RevLanguage;


Dist_phyloDACTMC::Dist_phyloDACTMC() : TypedDistribution< AbstractHomologousDiscreteCharacterData >()
{
    
}


Dist_phyloDACTMC::~Dist_phyloDACTMC()
{
    
}

Dist_phyloDACTMC* Dist_phyloDACTMC::clone( void ) const
{
    
    return new Dist_phyloDACTMC(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData >* Dist_phyloDACTMC::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
    size_t nStates = rm->getValue().getNumberOfStates();
    size_t nChars = rm->getValue().getNumberOfCharacters();
    
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData > *d = NULL;
    
    if ( dt == "DNA" )
    {
        RevBayesCore::GeneralTreeHistoryCtmc<RevBayesCore::DnaState> *dist = new RevBayesCore::GeneralTreeHistoryCtmc<RevBayesCore::DnaState>(tau, nStates, nChars);
        
        RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
        dist->setRateMap( rm );
        
        d = dist;
    }
    else if ( dt == "RNA" )
    {
        RevBayesCore::GeneralTreeHistoryCtmc<RevBayesCore::RnaState> *dist = new RevBayesCore::GeneralTreeHistoryCtmc<RevBayesCore::RnaState>(tau, nStates, nChars);
        
        RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
        dist->setRateMap( rm );
        
        d = dist;
    }
    else if ( dt == "AA" || dt == "Protein" )
    {
        RevBayesCore::GeneralTreeHistoryCtmc<RevBayesCore::AminoAcidState> *dist = new RevBayesCore::GeneralTreeHistoryCtmc<RevBayesCore::AminoAcidState>(tau, nStates, nChars);
        
        RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
        dist->setRateMap( rm );
        
        d = dist;
    }
    else if (dt == "Standard" )
    {
        RevBayesCore::GeneralTreeHistoryCtmc<RevBayesCore::StandardState> *dist = new RevBayesCore::GeneralTreeHistoryCtmc<RevBayesCore::StandardState>(tau, nStates, nChars);
        
        RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
        dist->setRateMap( rm );
        
        d = dist;
    }
    else if ( dt == "Biogeo" )
    {
        bool fe = static_cast<const RlBoolean&>(forbidExtinction->getRevObject()).getValue();
        bool uc = static_cast<const RlBoolean&>(useCladogenesis->getRevObject()).getValue();
        const RevBayesCore::TypedDagNode< RevBayesCore::RbVector< double > > *csf = NULL;
        
        if ( cladoStateFreqs->getRevObject() != RevNullObject::getInstance() )
        {
            csf = static_cast<const Simplex &>( cladoStateFreqs->getRevObject() ).getDagNode();
        }
        
        // (const TypedDagNode< treeType > *t, size_t nChars, size_t nSites, bool useAmbigChar=false, bool forbidExt=true, bool useClado=true)
        RevBayesCore::BiogeographicTreeHistoryCtmc<RevBayesCore::StandardState> *dist = new RevBayesCore::BiogeographicTreeHistoryCtmc<RevBayesCore::StandardState>(tau, nStates, nChars, false, fe, uc);
        
        RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
        dist->setRateMap( rm );
        dist->setCladogenicStateFrequencies( csf );
        
        d = dist;
    }
    
    return d;
}


/* Get Rev type of object */
const std::string& Dist_phyloDACTMC::getClassType(void)
{
    
    static std::string revType = "Dist_phyloDACTMC";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_phyloDACTMC::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_phyloDACTMC::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "PhyloDACTMC";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_phyloDACTMC::getParameterRules(void) const
{
    
    static MemberRules dist_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        dist_member_rules.push_back( new ArgumentRule( "tree"               , Tree::getClassTypeSpec() , "The along which the character(s) evolve.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        dist_member_rules.push_back( new ArgumentRule( "Q"                  , RateMap::getClassTypeSpec()  , "The transition rate matrix.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        dist_member_rules.push_back( new ArgumentRule( "cladoProbs"         , Simplex::getClassTypeSpec()  , "The cladogenetic probabilities.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        dist_member_rules.push_back( new ArgumentRule( "forbidExtinction"   , RlBoolean::getClassTypeSpec(), "Should we exclude complete extinction (zero areas occupied)?", ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlBoolean(true) ) );
        dist_member_rules.push_back( new ArgumentRule( "useCladogenesis"    , RlBoolean::getClassTypeSpec(), "Should we use cladigenesis?", ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlBoolean(true) ) );
        
        std::vector<std::string> options;
        options.push_back( "Biogeo" );
        options.push_back( "DNA" );
        options.push_back( "RNA" );
        options.push_back( "AA" );
        options.push_back( "Protein" );
        options.push_back( "Standard" );
        dist_member_rules.push_back( new OptionRule( "type", new RlString("DNA"), options, "The character data type used for initialization and simulation." ) );

        rules_set = true;
    }
    
    return dist_member_rules;
}


const TypeSpec& Dist_phyloDACTMC::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}

//
///* Make member methods for this class */
//template <class treeType>
//MethodTable Dist_phyloDACTMC::makeMethods(void) const
//{
//
//    MethodTable methods = MethodTable();
//
//    ArgumentRules* argRules = new ArgumentRules();
//
//    methods.addFunction("printBranchHistory", new DistributionMemberFunction<Dist_phyloDACTMC, Natural>( this, argRules ) );
//
//    // Insert inherited methods
//    methods.insertInheritedMethods( TypedDistribution<AbstractHomologousDiscreteCharacterData>::makeMethods() );
//
//    return methods;
//}

/** Print value for user */
void Dist_phyloDACTMC::printValue(std::ostream& o) const {
    
    o << "Character-State-Evolution-Along-Tree-Using-Data-Augmentation Process(tree=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    o << ", Q=";
    if ( q != NULL ) {
        o << q->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_phyloDACTMC::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "Q" )
    {
        q = var;
    }
    else if ( name == "type" )
    {
        type = var;
    }
    else if ( name == "forbidExtinction" )
    {
        forbidExtinction = var;
    }
    else if (name == "useCladogenesis")
    {
        useCladogenesis = var;
    }
    else if (name == "cladoProbs")
    {
        cladoStateFreqs = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}
