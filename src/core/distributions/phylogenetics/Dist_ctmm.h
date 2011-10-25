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
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef Dist_ctmm_H
#define Dist_ctmm_H

#include "CharacterStateDiscrete.h"
#include "DistributionDiscrete.h"

#include <ostream>
#include <string>

class RateMatrix;
class VectorString;

const std::string Dist_ctmm_name = "Dist_ctmm";

class Dist_ctmm: public DistributionDiscrete {

    public:
                                        Dist_ctmm(void);                                                    //!< Parser constructor

        // Basic utility functions
        Dist_ctmm*                      clone(void) const;                                                  //!< Clone object
        const VectorString&             getClass(void) const;                                               //!< Get class vector
        const TypeSpec&                 getTypeSpec(void) const;                                            //!< Get language type of the object

        // Member variable setup
        const RbPtr<MemberRules>        getMemberRules(void) const;                                         //!< Get member variable rules
        void                            setMemberVariable(const std::string& name, RbPtr<Variable> var);    //!< Set member variable (ensure number of states is consistent)

        // Discrete distribution functions
        size_t                          getNumberOfStates(void) const;                                      //!< Get number of states
        virtual const RbPtr<Simplex>    getProbabilityMassVector(void);                                     //!< Get probability mass vector
        const TypeSpec&                 getVariableType(void) const;                                        //!< Get random variable type (Simplex)
        double                          lnPdf(const RbPtr<RbLanguageObject> value);                         //!< Ln probability density
        double                          pdf(const RbPtr<RbLanguageObject> value);                           //!< Probability density
        RbPtr<RbLanguageObject>         rv(void);                                                           //!< Generate random variable
    
    private:
        static const TypeSpec           typeSpec;
        static const TypeSpec           varTypeSpec;
};

#endif

