/**
 * @file
 * This file contains the declaration of the character state evolution along a tree class.
 *
 * @brief Declaration and implementation of NormalDistribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Func_add.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef RlCharacterStateEvolutionAlongTree_H
#define RlCharacterStateEvolutionAlongTree_H

#include "AbstractCharacterData.h"
#include "RlAbstractCharacterData.h"
#include "RlCharacterData.h"
#include "RlTypedDistribution.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class treeType>
    class CharacterStateEvolutionAlongTree :  public TypedDistribution< AbstractCharacterData > {
        
    public:
        CharacterStateEvolutionAlongTree( void );
        virtual ~CharacterStateEvolutionAlongTree();
        
        // Basic utility functions
        CharacterStateEvolutionAlongTree*               clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        
        RbPtr<const Variable>                           tree;
        RbPtr<const Variable>                           q;
        RbPtr<const Variable>                           rate;
        RbPtr<const Variable>                           nSites;
        RbPtr<const Variable>                           type;
        
    };
    
}


#include "CharacterData.h"
#include "OptionRule.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "RlRateMatrix.h"
#include "RlString.h"
#include "RateMatrix.h"
#include "VectorAbstractElement.h"


template <class treeType>
RevLanguage::CharacterStateEvolutionAlongTree<treeType>::CharacterStateEvolutionAlongTree() : TypedDistribution< AbstractCharacterData >() {
    
}


template <class treeType>
RevLanguage::CharacterStateEvolutionAlongTree<treeType>::~CharacterStateEvolutionAlongTree() {
    
}



template <class treeType>
RevLanguage::CharacterStateEvolutionAlongTree<treeType>* RevLanguage::CharacterStateEvolutionAlongTree<treeType>::clone( void ) const {
    return new CharacterStateEvolutionAlongTree(*this);
}


template <class treeType>
RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData >* RevLanguage::CharacterStateEvolutionAlongTree<treeType>::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getValue() ).getValueNode();
    int n = static_cast<const Natural &>( nSites->getValue() ).getValue();
    const std::string& dt = static_cast<const RlString &>( type->getValue() ).getValue();
    
    
    RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData > *d = NULL;
    
    if ( dt == "DNA" ) 
    {
        RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::DnaState, typename treeType::valueType> *dist = new RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::DnaState, typename treeType::valueType>(tau, true, n);
        
        // set the clock rates
        if ( rate->getValueTypeSpec().isDerivedOf( Vector<RealPos>::getClassTypeSpec() ) ) 
        {
            RevBayesCore::TypedDagNode< std::vector<double> >* clockRates = static_cast<const Vector<RealPos> &>( rate->getValue() ).getValueNode();
            dist->setClockRate( clockRates );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getValue() ).getValueNode();
            dist->setClockRate( clockRate );
        }
        
        // set the rate matrix
        if ( q->getValueTypeSpec().isDerivedOf( Vector<RateMatrix>::getClassTypeSpec() ) ) 
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const VectorAbstractElement<RateMatrix> &>( q->getValue() ).getValueNode();
            dist->setRateMatrix( rm );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getValue() ).getValueNode();
            dist->setRateMatrix( rm );
        }
        
        d = dist;
    } 
    else if ( dt == "RNA" )
    {
        RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::RnaState, typename treeType::valueType> *dist = new RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::RnaState, typename treeType::valueType>(tau, true, n);
        
        
        if ( rate->getValueTypeSpec().isDerivedOf( Vector<RealPos>::getClassTypeSpec() ) ) 
        {
            RevBayesCore::TypedDagNode< std::vector<double> >* clockRates = static_cast<const Vector<RealPos> &>( rate->getValue() ).getValueNode();
            dist->setClockRate( clockRates );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getValue() ).getValueNode();
            dist->setClockRate( clockRate );
        }
        
        // set the rate matrix
        if ( q->getValueTypeSpec().isDerivedOf( Vector<RateMatrix>::getClassTypeSpec() ) ) 
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const VectorAbstractElement<RateMatrix> &>( q->getValue() ).getValueNode();
            dist->setRateMatrix( rm );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getValue() ).getValueNode();
            dist->setRateMatrix( rm );
        }

        d = dist;
    }
    else if ( dt == "AA" || dt == "Protein" )
    {
        RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::AminoAcidState, typename treeType::valueType> *dist = new RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::AminoAcidState, typename treeType::valueType>(tau, true, n);
        
        
        if ( rate->getValueTypeSpec().isDerivedOf( Vector<RealPos>::getClassTypeSpec() ) ) 
        {
            RevBayesCore::TypedDagNode< std::vector<double> >* clockRates = static_cast<const Vector<RealPos> &>( rate->getValue() ).getValueNode();
            dist->setClockRate( clockRates );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getValue() ).getValueNode();
            dist->setClockRate( clockRate );
        }
        
        // set the rate matrix
        if ( q->getValueTypeSpec().isDerivedOf( Vector<RateMatrix>::getClassTypeSpec() ) ) 
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const VectorAbstractElement<RateMatrix> &>( q->getValue() ).getValueNode();
            dist->setRateMatrix( rm );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getValue() ).getValueNode();
            dist->setRateMatrix( rm );
        }
        
        d = dist;
    }
    
    return d;
}



/* Get class name of object */
template <class treeType>
const std::string& RevLanguage::CharacterStateEvolutionAlongTree<treeType>::getClassName(void) { 
    
    static std::string rbClassName = "Character-State-Evolution-Along-Tree Process";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::CharacterStateEvolutionAlongTree<treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
template <class treeType>
const RevLanguage::MemberRules& RevLanguage::CharacterStateEvolutionAlongTree<treeType>::getMemberRules(void) const {
    
    static MemberRules distCharStateEvolutionMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "tree"           , true, treeType::getClassTypeSpec() ) );
        
        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( RateMatrix::getClassTypeSpec() );
        rateMatrixTypes.push_back( Vector<RateMatrix>::getClassTypeSpec() );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "Q"              , true, rateMatrixTypes ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( Vector<RealPos>::getClassTypeSpec() );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "branchRates"    , true, branchRateTypes, new RealPos(1.0) ) );
        
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "nSites"         , true, Natural::getClassTypeSpec() ) );
        
        Vector<RlString> options;
        options.push_back( RlString("DNA") );
        options.push_back( RlString("RNA") );
        options.push_back( RlString("AA") );
        options.push_back( RlString("Protein") );
        distCharStateEvolutionMemberRules.push_back( new OptionRule( "type", new RlString("DNA"), options ) );
        
        rulesSet = true;
    }
    
    return distCharStateEvolutionMemberRules;
}


template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::CharacterStateEvolutionAlongTree<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class treeType>
void RevLanguage::CharacterStateEvolutionAlongTree<treeType>::printValue(std::ostream& o) const {
    
    o << "Character-State-Evolution-Along-Tree Process(tree=";
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
    o << ", rate=";
    if ( rate != NULL ) {
        o << rate->getName();
    } else {
        o << "?";
    }
    o << ", nSites=";
    if ( nSites != NULL ) {
        o << nSites->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
template <class treeType>
void RevLanguage::CharacterStateEvolutionAlongTree<treeType>::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "tree" ) 
    {
        tree = var;
    }
    else if ( name == "Q" ) 
    {
        q = var;
    }
    else if ( name == "branchRates" ) 
    {
        rate = var;
    }
    else if ( name == "nSites" ) 
    {
        nSites = var;
    }
    else if ( name == "type" ) 
    {
        type = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}


#endif
