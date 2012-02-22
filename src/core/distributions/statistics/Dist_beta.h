/**
 * @file
 * This file contains the declaration of Dist_beta, which is used to hold
 * parameters and functions related to an beta distribution.
 *
 * @brief Declaration of Dist_beta
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

#ifndef Dist_beta_H
#define Dist_beta_H

#include "DistributionContinuous.h"
#include "Probability.h"

#include <ostream>
#include <string>

class DAGNode;
class StochasticNode;
class VectorString;

class Dist_beta: public DistributionContinuous {

    public:
                                    Dist_beta(void);                                                    //!< Parser constructor

        // Basic utility functions
        Dist_beta*                  clone(void) const;                                                  //!< Clone object
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
        RbVariablePtr               alpha; 
        RbVariablePtr               beta;
    
        // memberfunction return value
        Probability                 randomVariable;
    
};

#endif

