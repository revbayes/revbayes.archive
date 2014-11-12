/**
 * @file
 * This file contains the declaration of the Binomial distribution, which is used create
 * random variables of Bernoulli distributions.
 *
 * @brief Declaration and implementation of Dist_binomial
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef Dist_binomial_H
#define Dist_binomial_H

#include "BinomialDistribution.h"
#include "Natural.h"
#include "Probability.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    class Dist_binomial :  public TypedDistribution<Natural> {
        
    public:
                                                        Dist_binomial(void);
        virtual                                        ~Dist_binomial(void);
        
        // Basic utility functions
        Dist_binomial*                                  clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::BinomialDistribution*             createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                           p;
        RevPtr<const RevVariable>                           n;
    };
    
}

#endif
