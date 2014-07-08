#ifndef Dist_phyloDACTMC_H
#define Dist_phyloDACTMC_H

#include "AbstractDiscreteCharacterData.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlRateMap.h"
#include "RlTypedDistribution.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class treeType>
    class Dist_phyloDACTMC :  public TypedDistribution< AbstractDiscreteCharacterData > {
        
    public:
        Dist_phyloDACTMC( void );
        virtual ~Dist_phyloDACTMC();
        
        // Basic utility functions
        Dist_phyloDACTMC*                               clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          q;
        RevPtr<const Variable>                          forbidExtinction;
        RevPtr<const Variable>                          useCladogenesis;
        RevPtr<const Variable>                          type;
        
    };
    
}


#include "OptionRule.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "RevNullObject.h"
#include "RlString.h"
#include "RateMap.h"
#include "StandardState.h"
#include "VectorAbstractElement.h"


template <class treeType>
RevLanguage::Dist_phyloDACTMC<treeType>::Dist_phyloDACTMC() : TypedDistribution< AbstractDiscreteCharacterData >() {
    
}


template <class treeType>
RevLanguage::Dist_phyloDACTMC<treeType>::~Dist_phyloDACTMC() {
    
}

template <class treeType>
RevLanguage::Dist_phyloDACTMC<treeType>* RevLanguage::Dist_phyloDACTMC<treeType>::clone( void ) const {
    
    return new Dist_phyloDACTMC(*this);
}


template <class treeType>
RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData >* RevLanguage::Dist_phyloDACTMC<treeType>::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
    size_t nStates = rm->getValue().getNumberOfStates();
    size_t nChars = rm->getValue().getNumberOfCharacters();
    
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData > *d = NULL;
    
    if ( dt == "biogeo" )
    {
        bool fe = static_cast<const RlBoolean&>(forbidExtinction->getRevObject()).getValue();
        bool uc = static_cast<const RlBoolean&>(useCladogenesis->getRevObject()).getValue();
        
        // (const TypedDagNode< treeType > *t, size_t nChars, size_t nSites, bool useAmbigChar=false, bool forbidExt=true, bool useClado=true)
        RevBayesCore::BiogeographicTreeHistoryCtmc<RevBayesCore::StandardState, typename treeType::valueType> *dist = new RevBayesCore::BiogeographicTreeHistoryCtmc<RevBayesCore::StandardState, typename treeType::valueType>(tau, nStates, nChars, false, fe, uc);
        
        RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
        dist->setRateMap( rm );
        
        d = dist;
    }
    return d;
}



/* Get class name of object */
template <class treeType>
const std::string& RevLanguage::Dist_phyloDACTMC<treeType>::getClassName(void) {
    
    static std::string rbClassName = "Dist_phyloDACTMC";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloDACTMC<treeType>::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}


/** Return member rules (no members) */
template <class treeType>
const RevLanguage::MemberRules& RevLanguage::Dist_phyloDACTMC<treeType>::getMemberRules(void) const {
    
    static MemberRules distCharStateEvolutionMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "tree"               , true, treeType::getClassTypeSpec() ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "Q"                  , true, RateMap::getClassTypeSpec() ) );
//        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "nStates"            , true, Natural::getClassTypeSpec() ) );
//        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "nChars"             , true, Natural::getClassTypeSpec() ) );
//        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "ambiguous"   , true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "forbidExtinction"   , true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "useCladogenesis"    , true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        Vector<RlString> options;
        options.push_back( RlString("biogeo") );
        distCharStateEvolutionMemberRules.push_back( new OptionRule( "type", new RlString("biogeo"), options ) );
        rulesSet = true;
    }
    
    return distCharStateEvolutionMemberRules;
}


template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloDACTMC<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class treeType>
void RevLanguage::Dist_phyloDACTMC<treeType>::printValue(std::ostream& o) const {
    
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
template <class treeType>
void RevLanguage::Dist_phyloDACTMC<treeType>::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}


#endif
