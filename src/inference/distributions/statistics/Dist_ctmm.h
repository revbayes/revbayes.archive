/**
 * @file
 * This file contains the declaration of Dist_ctmm, which holds
 * the parameters and functions related to a distribution on
 * discrete states from a continuous-time discrete-state Markov
 * model.
 *
 * @brief Declaration of Dist_ctmm
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

#ifndef Dist_ctmm_H
#define Dist_ctmm_H

#include "CharacterStateDiscrete.h"
#include "DistributionDiscrete.h"
#include "Variable.h"

#include <ostream>
#include <string>

class RateMatrix;

class Dist_ctmm: public DistributionDiscrete {

    public:
                                                Dist_ctmm(void);                                                    //!< Constructor
                                                Dist_ctmm(const Dist_ctmm& d);                                      //!< Copy constructor
        virtual                                ~Dist_ctmm(void);
        
        // overloaded operators
        Dist_ctmm&                              operator=(const Dist_ctmm& d);
    
        // Basic utility functions
        Dist_ctmm*                              clone(void) const;                                                  //!< Clone object
        static const std::string&               getClassName(void);                                                 //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object

        // Member variable setup
        const MemberRules&                      getMemberRules(void) const;                                         //!< Get member variable rules
        void                                    setMemberVariable(const std::string& name, const Variable* var);    //!< Set member variable (ensure number of states is consistent)

        // Discrete distribution functions
        size_t                                  getNumberOfStates(void) const;                                      //!< Get number of states
        const Simplex&                          getProbabilityMassVector(void);                                     //!< Get probability mass vector
        const std::vector<RbLanguageObject*>&   getStateVector(void) const;
        const TypeSpec&                         getVariableType(void) const;                                        //!< Get random variable type (Simplex)
        double                                  lnPdf( const RbLanguageObject& value) const;                        //!< Ln probability density
        double                                  pdf( const RbLanguageObject& value) const;                          //!< Probability density
        const RbLanguageObject&                 rv(void);                                                           //!< Generate random variable
    
    private:

        std::vector<RbLanguageObject*>          stateVector;
        
        // parameters
        RbConstVariablePtr                      rateMatrix;
        RbConstVariablePtr                      time;
        RbConstVariablePtr                      initialState;
    
        // memberfunction return values
        CharacterStateDiscrete*                 randomVariable;

};

#endif

