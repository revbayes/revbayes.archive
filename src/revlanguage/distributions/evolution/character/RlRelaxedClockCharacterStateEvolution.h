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

#ifndef RlRelaxedClockCharacterStateEvolution_H
#define RlRelaxedClockCharacterStateEvolution_H

#include "SimpleBranchRateTimeCharEvoModel.h"
#include "RlAbstractCharacterData.h"
#include "RlCharacterData.h"
#include "RlTypedDistribution.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class charType, class treeType>
    class RelaxedClockCharacterStateEvolution :  public TypedDistribution< AbstractCharacterData > {
        
    public:
        RelaxedClockCharacterStateEvolution( void );
        virtual ~RelaxedClockCharacterStateEvolution();
        
        // Basic utility functions
        RelaxedClockCharacterStateEvolution*            clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::SimpleBranchRateTimeCharEvoModel<typename charType::valueType, typename treeType::valueType>*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        
        RbPtr<const Variable>                           tree;
        RbPtr<const Variable>                           q;
        RbPtr<const Variable>                           clockRates;
        RbPtr<const Variable>                           nSites;
        
    };
    
}


#include "CharacterData.h"
#include "RlTimeTree.h"


template <class charType, class treeType>
RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::RelaxedClockCharacterStateEvolution() : TypedDistribution< AbstractCharacterData >() {
    
}


template <class charType, class treeType>
RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::~RelaxedClockCharacterStateEvolution() {
    
}



template <class charType, class treeType>
RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>* RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::clone( void ) const {
    return new RelaxedClockCharacterStateEvolution(*this);
}


template <class charType, class treeType>
RevBayesCore::SimpleBranchRateTimeCharEvoModel<typename charType::valueType, typename treeType::valueType>* RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* cr = static_cast<const Vector<RealPos> &>( clockRates->getValue() ).getValueNode();
    int n = static_cast<const Natural &>( nSites->getValue() ).getValueNode()->getValue();
    RevBayesCore::SimpleBranchRateTimeCharEvoModel<typename charType::valueType, typename treeType::valueType>*   d = new RevBayesCore::SimpleBranchRateTimeCharEvoModel<typename charType::valueType, typename treeType::valueType>(tau, rm, cr, true, n);
    
    return d;
}



/* Get class name of object */
template <class charType, class treeType>
const std::string& RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::getClassName(void) { 
    
    static std::string rbClassName = "Character-State-Evolution-Along-Tree Process";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <class charType, class treeType>
const RevLanguage::TypeSpec& RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules */
template <class charType, class treeType>
const RevLanguage::MemberRules& RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::getMemberRules(void) const {
    
    static MemberRules distCharStateEvolutionMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "tree", true, treeType::getClassTypeSpec() ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "Q"  , true, RateMatrix::getClassTypeSpec() ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "clockRates"  , true, Vector<RealPos>::getClassTypeSpec(), new RealPos(1.0) ) );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "nSites"  , true, Natural::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distCharStateEvolutionMemberRules;
}


template <class charType, class treeType>
const RevLanguage::TypeSpec& RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class charType, class treeType>
void RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::printValue(std::ostream& o) const {
    
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
    o << ", clockRates=";
    if ( clockRates != NULL ) {
        o << clockRates->getName();
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
void RevLanguage::RelaxedClockCharacterStateEvolution<charType, treeType>::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "Q" ) {
        q = var;
    }
    else if ( name == "clockRates" ) {
        clockRates = var;
    }
    else if ( name == "nSites" ) {
        nSites = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}


#endif
