/**
 * @file
 * This file contains the declaration of the lognormal distribution, which is used create
 * random variables of uniform distributions.
 *
 * @brief Declaration and implementation of Dist_lnorm
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

#ifndef Dist_lnorm_H
#define Dist_lnorm_H

#include "LognormalDistribution.h"
#include "RlPositiveContinuousDistribution.h"

namespace RevLanguage {
    
    class Dist_lnorm :  public PositiveContinuousDistribution {
        
    public:
        Dist_lnorm( void );
        
        // Basic utility functions
        Dist_lnorm*                          clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::LognormalDistribution*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          mean;
        RevPtr<const Variable>                          sd;
        RevPtr<const Variable>                          offset;
        
    };
    
}

#endif
