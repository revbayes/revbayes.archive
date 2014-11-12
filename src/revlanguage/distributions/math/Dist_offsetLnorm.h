/**
 * @file
 * This file contains the declaration of the lognormal distribution, which is used create
 * random variables of uniform distributions.
 *
 * @brief Declaration and implementation of LognormalDistribution
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

#ifndef Dist_offsetLognormal_H
#define Dist_offsetLognormal_H

#include "LognormalDistribution.h"
#include "RlContinuousDistribution.h"

namespace RevLanguage {
    
    class Dist_offsetLnorm :  public ContinuousDistribution {
        
    public:
        Dist_offsetLnorm( void );
        
        // Basic utility functions
        Dist_offsetLnorm*                       clone(void) const;                                                              //!< Clone the object
        static const std::string&               getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                      getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                    printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::LognormalDistribution*    createDistribution(void) const;
        
    protected:
        
        void                                    setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                  mean;
        RevPtr<const RevVariable>                  sd;
        RevPtr<const RevVariable>                  offset;
        
    };
    
}

#endif
