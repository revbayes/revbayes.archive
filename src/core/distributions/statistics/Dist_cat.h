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
class VectorString;


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

