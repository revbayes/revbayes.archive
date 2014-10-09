#ifndef Dist_phyloDACTMC_H
#define Dist_phyloDACTMC_H

#include "AbstractDiscreteCharacterData.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlRateMap.h"
#include "RlTypedDistribution.h"
#include "RlSimplex.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class treeType>
    class Dist_phyloDACTMC :  public TypedDistribution< AbstractDiscreteCharacterData > {
        
    public:
        Dist_phyloDACTMC( void );
        virtual ~Dist_phyloDACTMC();
        
        // Basic utility functions
        Dist_phyloDACTMC*                               clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
//        const MethodTable&                              getMethods(void) const;                                                         //!< Get member methods
//        RevPtr<RevLanguage::Variable>                   executeMethod(std::string const &name, const std::vector<Argument> &args);
//        MethodTable                                     makeMethods(void) const;
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          q;
        RevPtr<const Variable>                          cladoStateFreqs;
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
RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData >* RevLanguage::Dist_phyloDACTMC<treeType>::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
    size_t nStates = rm->getValue().getNumberOfStates();
    size_t nChars = rm->getValue().getNumberOfCharacters();
    
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData > *d = NULL;
    
    if ( dt == "biogeo" )
    {
        bool fe = static_cast<const RlBoolean&>(forbidExtinction->getRevObject()).getValue();
        bool uc = static_cast<const RlBoolean&>(useCladogenesis->getRevObject()).getValue();
        const RevBayesCore::TypedDagNode< RevBayesCore::RbVector< double > > *csf = NULL;
        
        if ( cladoStateFreqs->getRevObject() != RevNullObject::getInstance() )
        {
            csf = static_cast<const Simplex &>( cladoStateFreqs->getRevObject() ).getDagNode();
        }
        
        // (const TypedDagNode< treeType > *t, size_t nChars, size_t nSites, bool useAmbigChar=false, bool forbidExt=true, bool useClado=true)
        RevBayesCore::BiogeographicTreeHistoryCtmc<RevBayesCore::StandardState, typename treeType::valueType> *dist = new RevBayesCore::BiogeographicTreeHistoryCtmc<RevBayesCore::StandardState, typename treeType::valueType>(tau, nStates, nChars, false, fe, uc);
        
        RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* rm = static_cast<const RateMap &>( q->getRevObject() ).getDagNode();
        dist->setRateMap( rm );
        dist->setCladogenicStateFrequencies( csf );
        
        d = dist;
    }
    return d;
}

///* Map calls to member methods */
//template <class treeType>
//RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::Dist_phyloDACTMC<treeType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
//    
//    if (name == "printBranchHistory")
//    {
//        // get the member with give index
//        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
//        
////        if (this->dagNode->getValue().getHistories().size() < (size_t)(index.getValue()) )
////        {
////            throw RbException("Index out of bounds in []");
////        }
////
////        const RevBayesCore::DiscreteTaxonData<typename charType::valueType>& element = static_cast< RevBayesCore::DiscreteCharacterData<typename charType::valueType>& >( this->dagNode->getValue() ).getTaxonData(size_t(index.getValue()) - 1);
////        
////        return new Variable( new DiscreteTaxonData<charType>( new RevBayesCore::DiscreteTaxonData<typename charType::valueType>( element ) ) );
////        return new Variable( new Dist_phyloDACTMC<treeType>( new RevBayesCore::BiogeographicTreeHistoryCtmc<RevBayesCore::StandardState, typename treeType::valueType>() ) );
//    }
//    
//    return TypedDistribution< AbstractDiscreteCharacterData >::executeMethod( name, args );
//}


/* Get Rev type of object */
template <class treeType>
const std::string& RevLanguage::Dist_phyloDACTMC<treeType>::getClassType(void) {
    
    static std::string revType = "Dist_phyloDACTMC";
    
	return revType;
}

/* Get class type spec describing type of object */
template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloDACTMC<treeType>::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Return member rules (no members) */
template <class treeType>
const RevLanguage::MemberRules& RevLanguage::Dist_phyloDACTMC<treeType>::getMemberRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        distMemberRules.push_back( new ArgumentRule( "tree"               , treeType::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMemberRules.push_back( new ArgumentRule( "Q"                  , RateMap::getClassTypeSpec()  , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMemberRules.push_back( new ArgumentRule( "C"                  , Simplex::getClassTypeSpec()  , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distMemberRules.push_back( new ArgumentRule( "forbidExtinction"   , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlBoolean(true) ) );
        distMemberRules.push_back( new ArgumentRule( "useCladogenesis"    , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new RlBoolean(true) ) );
        
        std::vector<std::string> options;
        options.push_back( "biogeo" );
        distMemberRules.push_back( new OptionRule( "type", new RlString("biogeo"), options ) );
        rulesSet = true;
    }
    
    return distMemberRules;
}

//template <class treeType>
//const RevLanguage::MethodTable& RevLanguage::Dist_phyloDACTMC<treeType>::getMethods( void ) const
//{
//    static MethodTable  myMethods   = MethodTable();
//    static bool         methodsSet  = false;
//    
//    if ( !methodsSet )
//    {quit
//        myMethods = makeMethods();
//        methodsSet = true;
//    }
//    
//    return myMethods;
//}


template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloDACTMC<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}

//
///* Make member methods for this class */
//template <class treeType>
//RevLanguage::MethodTable RevLanguage::Dist_phyloDACTMC<treeType>::makeMethods(void) const
//{
//    
//    MethodTable methods = MethodTable();
//    
//    ArgumentRules* argRules = new ArgumentRules();
//    
//    methods.addFunction("printBranchHistory", new DistributionMemberFunction<Dist_phyloDACTMC<treeType>, Natural>( this, argRules ) );
//    
//    // Insert inherited methods
//    methods.insertInheritedMethods( TypedDistribution<AbstractDiscreteCharacterData>::makeMethods() );
//    
//    return methods;
//}

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
    else if (name == "C")
    {
        cladoStateFreqs = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}


#endif
