/**
 * @file
 * This file contains the declaration of Dist_norm, which is used to hold
 * parameters and functions related to a normal distribution.
 *
 * @brief Declaration of Dist_norm
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

#ifndef Dist_norm_H
#define Dist_norm_H

#include "DistributionContinuous.h"

#include <ostream>
#include <string>

class Real;

class Dist_norm: public DistributionContinuous {

    public:
                                    Dist_norm(void);                                                    //!< Parser constructor

        // Basic utility functions
        Dist_norm*                  clone(void) const;                                                  //!< Clone object
        static const std::string&   getClassName(void);                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
        void                        setMemberVariable(const std::string& name, const Variable* var);    //!< Catching the setting of the member variables.

        // Normal distribution functions
        double                      cdf(const RbLanguageObject& value);                                 //!< Cumulative density
        const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (RealPos)
        double                      lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
        double                      pdf(const RbLanguageObject& value) const;                           //!< Probability density
        const Real&                 quantile(const double p);                                           //!< Quantile
        const RbLanguageObject&     rv(void);                                                           //!< Generate random variable
    
    private:

        // parameters
        RbConstVariablePtr          mu;
        RbConstVariablePtr          sigma;
    
        // memberfunction return values
        Real                        randomVariable;

};

#endif

