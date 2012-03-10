/**
 * @file
 * This file contains the declaration of Dist_cat, which is used to hold
 * parameters and functions related to a distribution on a categorical
 * variable ( a factor).
 *
 * @brief Declaration of Dist_cat
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Dist_cat_H
#define Dist_cat_H

#include "CharacterStateDiscrete.h"
#include "DistributionDiscrete.h"

#include <set>
#include <string>

class ArgumentRule;
class Categorical;
class Move;
class Simplex;
class StochasticNode;


template <typename setType>
class Dist_cat: public DistributionDiscrete {

    public:
                                                Dist_cat(void);                                                         //!< Parser constructor
        virtual                                ~Dist_cat(void) {}                                                       //!< Virtual destructor

        // Basic utility functions
        Dist_cat*                               clone(void) const;                                                      //!< Clone object
        static const std::string&               getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                //!< Get language type of the object

        // Member variable setup
        const MemberRules&                      getMemberRules(void) const;                                             //!< Get member variable rules
        void                                    setMemberVariable(const std::string& name, Variable* var);          //!< Catching the setting of the member variables.

        // Discrete distribution functions
        size_t                                  getNumberOfStates(void) const;                                          //!< Get number of states
        const Simplex&                          getProbabilityMassVector(void);                                         //!< Get probability mass vector
        const std::vector<RbLanguageObject*>&   getStateVector(void) const;
        const TypeSpec&                         getVariableType(void) const;                                            //!< Get random variable type (Simplex)
        double                                  lnPdf(const RbLanguageObject& value) const;                             //!< Ln probability density
        double                                  pdf(const RbLanguageObject& value) const;                               //!< Probability density
        const RbLanguageObject&                 rv(void);                                                               //!< Generate random variable
    
    private:

        std::vector<RbLanguageObject*>          stateVector;

        // parameters
        RbVariablePtr                           probabilities;
        RbVariablePtr                           states;
        
        // memberfunction return values
        RbLanguageObject*                       randomVariable;

};

#endif


/**
 * @file
 * This file contains the implementation of Dist_cat, which is used to hold
 * parameters and functions related to a distribution on a categorical
 * variable (a factor).
 *
 * @brief Implementation of Dist_cat
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "DAGNode.h"
#include "Move_msimplex.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "Real.h"
#include "Set.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor */
template <typename setType>
Dist_cat<setType>::Dist_cat( void ) : DistributionDiscrete( getMemberRules() ), probabilities( NULL ), states( NULL ) {
}


/** Clone this object */
template <typename setType>
Dist_cat<setType>* Dist_cat<setType>::clone( void ) const {
    
    return new Dist_cat<setType>( *this );
}


/** Get class name of object */
template <typename setType>
const std::string& Dist_cat<setType>::getClassName(void) { 
    
    static std::string rbClassName = "Categorical distribution<" + setType::getClassName() + ">";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <typename setType>
const TypeSpec& Dist_cat<setType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DistributionDiscrete::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
template <typename setType>
const TypeSpec& Dist_cat<setType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Get member variable rules */
template <typename setType>
const MemberRules& Dist_cat<setType>::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ValueRule( "probabilities", Simplex::getClassTypeSpec() ) );
        memberRules.push_back( new ValueRule( "states",        Set<setType>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get the number of states in the distribution */
template <typename setType>
size_t Dist_cat<setType>::getNumberOfStates( void ) const {
    
    return static_cast<const Simplex&>( probabilities->getValue() ).size();
}


/** Get the probability mass vector */
template <typename setType>
const Simplex& Dist_cat<setType>::getProbabilityMassVector( void ) {
    
    return static_cast<Simplex&>( probabilities->getValue() );
}


/** Get the state vector for this distribution */
template <typename setType>
const std::vector<RbLanguageObject*>& Dist_cat<setType>::getStateVector( void ) const {
    return stateVector;
}


/** Get random variable type */
template <typename setType>
const TypeSpec& Dist_cat<setType>::getVariableType( void ) const {
    
    return setType::getClassTypeSpec();
}


/**
 * This function calculates the natural log of the
 * probability of a categorical random variable.
 *
 * @brief Natural log of categorical probability
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
template <typename setType>
double Dist_cat<setType>::lnPdf( const RbLanguageObject& value ) const {
    
	// Get the value and the parameters of the categorical distribution
    std::vector<double> m = static_cast<const Simplex&               >( probabilities->getValue() ).getValue();
    const Set<setType>& x = static_cast<const Set<setType>& >( states->getValue()        );
    
    int index = x.findIndex( static_cast<const setType&>( value ) );
    if ( index < 0 )
        return std::log( 0.0 );
    else
        return std::log( m[index] );
}


/**
 * This function calculates the probability
 * of a categorical random variable.
 *
 * @brief Categorical probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
template <typename setType>
double Dist_cat<setType>::pdf( const RbLanguageObject& value ) const {
    
	// Get the value and the parameter of the categorical distribution
    std::vector<double> m = static_cast<const Simplex&      >( probabilities->getValue() ).getValue();
    const Set<setType>& s = static_cast<const Set<setType>& >( states->getValue()        );
    
    int index = s.findIndex( static_cast<const setType&>( value ) );
    if ( index < 0 )
        return 0.0;
    else
        return m[index];
}


/**
 * This function generates a categorical
 * random variable.
 *
 * @brief Random draw from categorical distribution
 *
 * @return      Random draw from categorical distribution
 */
template <typename setType>
const RbLanguageObject& Dist_cat<setType>::rv( void ) {
    
	// Get the value and the parameter of the categorical distribution
    std::vector<double> m = static_cast<const Simplex&      >( probabilities->getValue() ).getValue();
    const Set<setType>& s = static_cast<const Set<setType>& >( states->getValue()        );
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Get copy of reference object
    delete randomVariable;
    
    // Draw a random value
    double r   = rng->uniform01();
    double sum = 0.0;
    size_t i;
    for ( i=0; i<m.size(); i++ ) {
        sum += m[i];
        if ( sum > r )
            break;
    }
    
    // get the random variable by accessing our state vector, selecting the i-th element and cloning it
    randomVariable = static_cast<RbLanguageObject*>( s.getElement( i ).clone() );
    
    
    // Return draw
    return *randomVariable;
}


/** We catch here the setting of the member variables to store our parameters. */
template <typename setType>
void Dist_cat<setType>::setMemberVariable(std::string const &name, Variable *var) {
    
    if ( name == "probabilities" ) {
        probabilities = var;
    }
    else if ( name == "states" ){
        states = var;
        
        // reset the state vector
        stateVector.clear();
        Set<setType>& c = static_cast<Set<setType>& >( var->getValue() );
        for (int i = 0; i < c.size(); i++) {
            RbLanguageObject* tmp = static_cast<RbLanguageObject*>( c.getElement(i).clone() );
            stateVector.push_back(tmp);
        }
    }
    else {
        DistributionDiscrete::setMemberVariable(name, var);
    }
}


