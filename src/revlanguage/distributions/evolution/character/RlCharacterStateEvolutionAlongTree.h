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

#include "SimpleSiteHomogeneousCharEvoModel.h"
#include "RlCharacterData.h"
#include "RlTypedDistribution.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class charType, class treeType>
    class CharacterStateEvolutionAlongTree :  public TypedDistribution<CharacterData< charType > > {
        
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
        RevBayesCore::SimpleSiteHomogeneousCharEvoModel<typename charType::valueType, typename treeType::valueType>*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        
        RbPtr<const Variable>                           tree;
        RbPtr<const Variable>                           q;
        RbPtr<const Variable>                           rate;
        RbPtr<const Variable>                           nSites;
        
    };
    
}


#include "CharacterData.h"
#include "RlRateMatrix.h"
#include "RateMatrix.h"
#include "RlTree.h"


template <class charType, class treeType>
RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::CharacterStateEvolutionAlongTree() : TypedDistribution<CharacterData<charType> >() {
    
}


template <class charType, class treeType>
RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::~CharacterStateEvolutionAlongTree() {
    
}



template <class charType, class treeType>
RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>* RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::clone( void ) const {
    return new CharacterStateEvolutionAlongTree(*this);
}


template <class charType, class treeType>
RevBayesCore::SimpleSiteHomogeneousCharEvoModel<typename charType::valueType, typename treeType::valueType>* RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* cr = static_cast<const RealPos &>( rate->getValue() ).getValueNode();
    int n = static_cast<const Natural &>( nSites->getValue() ).getValueNode()->getValue();
    RevBayesCore::SimpleSiteHomogeneousCharEvoModel<typename charType::valueType, typename treeType::valueType>*   d = new RevBayesCore::SimpleSiteHomogeneousCharEvoModel<typename charType::valueType, typename treeType::valueType>(tau, rm, cr, true, n);
    
    return d;
}



/* Get class name of object */
template <class charType, class treeType>
const std::string& RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::getClassName(void) { 
    
    static std::string rbClassName = "Character-State-Evolution-Along-Tree Process";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <class charType, class treeType>
const RevLanguage::TypeSpec& RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
template <class charType, class treeType>
const RevLanguage::MemberRules& RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::getMemberRules(void) const {
    
    static MemberRules distCharStateEvolutionMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "tree"    , true, treeType::getClassTypeSpec() ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "Q"       , true, RateMatrix::getClassTypeSpec() ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "rate"    , true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "nSites"  , true, Natural::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distCharStateEvolutionMemberRules;
}


template <class charType, class treeType>
const RevLanguage::TypeSpec& RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class charType, class treeType>
void RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::printValue(std::ostream& o) const {
    
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
template <class charType, class treeType>
void RevLanguage::CharacterStateEvolutionAlongTree<charType, treeType>::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "Q" ) {
        q = var;
    }
    else if ( name == "rate" ) {
        rate = var;
    }
    else if ( name == "nSites" ) {
        nSites = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}


#endif
