/**
 * @file
 * This file contains the declaration of Dist_gamma, which is used to hold
 * parameters and functions related to an gamma distribution.
 *
 * @brief Declaration of Dist_gamma
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

#ifndef Dist_gamma_H
#define Dist_gamma_H

#include "DistributionContinuous.h"
#include "RealPos.h"
#include "Variable.h"

#include <ostream>
#include <string>

class Dist_gamma: public DistributionContinuous {

    public:
                                    Dist_gamma(void);                                                   //!< Parser constructor

        // Basic utility functions
        Dist_gamma*                 clone(void) const;                                                  //!< Clone object
        static const std::string&   getClassName(void);                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
        void                        setMemberVariable(const std::string& name, Variable* var);          //!< Catching the setting of the member variables.

        // Exponential distribution functions
        double                      cdf(const RbLanguageObject& value);                                 //!< Cumulative density
        const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (RealPos)
        double                      lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
        double                      pdf(const RbLanguageObject& value) const;                           //!< Probability density
        const Real&                 quantile(const double p);                                           //!< Quantile
        const RbLanguageObject&     rv(void);                                                           //!< Generate random variable
    
    private:

        // parameters
        RbVariablePtr               shape;
        RbVariablePtr               rate;
    
        // memberfunction return variables
        RealPos                     randomVariable;

};

#endif

