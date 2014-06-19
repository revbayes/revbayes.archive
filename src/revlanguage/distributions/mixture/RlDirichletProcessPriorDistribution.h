/**
 * @file
 * This file contains the declaration of the Dirichlet process prior distribution, which is used create
 * random variables of DPP distributions.
 *
 * @brief Declaration of the Dirichlet distribution
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

#ifndef RlDirichletProcessPriorDistribution_H
#define RlDirichletProcessPriorDistribution_H

#include "DirichletProcessPriorDistribution.h"
#include "RlTypedDistribution.h"
#include "Vector.h"
#include "RealPos.h"
#include "TypeSpec.h"

namespace RevLanguage {
    
    template <typename valType>
    class DirichletProcessPriorDistribution :  public TypedDistribution< Vector<valType> >{
        
    public:
        DirichletProcessPriorDistribution( void );
        virtual ~DirichletProcessPriorDistribution();
        
        // Basic utility functions
        DirichletProcessPriorDistribution*                          clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
//        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::DirichletProcessPriorDistribution<typename valType::valueType>*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        RbPtr<const Variable>                           concentration;
        RbPtr<const Variable>                           baseDistribution;
        RbPtr<const Variable>                           numElements;
        
    };
    
}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Real.h"
#include "RealPos.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "TypedDistribution.h"


template <typename valType>
RevLanguage::DirichletProcessPriorDistribution<valType>::DirichletProcessPriorDistribution() : TypedDistribution< Vector<valType> >(), 
concentration( NULL ),
baseDistribution( NULL ),
numElements( NULL )
{
    
}


template <typename valType>
RevLanguage::DirichletProcessPriorDistribution<valType>::~DirichletProcessPriorDistribution() {
    
}



template <typename valType>
RevLanguage::DirichletProcessPriorDistribution<valType>* RevLanguage::DirichletProcessPriorDistribution<valType>::clone( void ) const {
    return new DirichletProcessPriorDistribution(*this);
}


template <typename valType>
RevBayesCore::DirichletProcessPriorDistribution<typename valType::valueType>* RevLanguage::DirichletProcessPriorDistribution<valType>::createDistribution( void ) const {
	
    // get the parameters
    const Distribution& rlDistribution									= static_cast<const Distribution &>( baseDistribution->getValue() );
    RevBayesCore::TypedDistribution<typename valType::valueType>* g0    = static_cast<RevBayesCore::TypedDistribution<typename valType::valueType>* >( rlDistribution.createDistribution() );    
    RevBayesCore::TypedDagNode<double>* cp								= static_cast<const RealPos &>( concentration->getValue() ).getValueNode();

    int ne = static_cast<const RealPos &>( numElements->getValue() ).getValue();
    RevBayesCore::DirichletProcessPriorDistribution<typename valType::valueType>* d		= new RevBayesCore::DirichletProcessPriorDistribution<typename valType::valueType>(g0, cp, ne);
    
    return d;
}



/* Get class name of object */
template <typename valType>
const std::string& RevLanguage::DirichletProcessPriorDistribution<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Dirichlet process prior distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::DirichletProcessPriorDistribution<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedDistribution< Vector<RealPos> >::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
template <typename valType>
const RevLanguage::MemberRules& RevLanguage::DirichletProcessPriorDistribution<valType>::getMemberRules(void) const {
    
    static MemberRules distDPPMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distDPPMemberRules.push_back( new ArgumentRule( "concentration",     true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        distDPPMemberRules.push_back( new ArgumentRule( "baseDistribution",  true, TypedDistribution<valType>::getClassTypeSpec() ) );
        distDPPMemberRules.push_back( new ArgumentRule( "numElements",       true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        
        rulesSet = true;
    }
    
    return distDPPMemberRules;
}


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::DirichletProcessPriorDistribution<valType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}



/** Set a member variable */
template <typename valType>
void RevLanguage::DirichletProcessPriorDistribution<valType>::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "concentration" ) {
        concentration = var;
    }
    else if ( name == "baseDistribution" ) {
        baseDistribution = var;
    }
    else if ( name == "numElements" ) {
        numElements = var;
    }
    else {
        TypedDistribution< Vector< valType > >::setConstMemberVariable(name, var);
    }
}


#endif
