/**
 * @file
 * This file contains the declaration of the gamma distribution, which is used create
 * random variables of gamma distributions.
 *
 * @brief Declaration and implementation of GammaDistribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Func_add.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef RlGammaDistribution_H
#define RlGammaDistribution_H

#include "GammaDistribution.h"
#include "RlPositiveContinuousDistribution.h"

namespace RevLanguage {
    
    class GammaDistribution :  public PositiveContinuousDistribution {
        
    public:
        GammaDistribution( void );
        virtual ~GammaDistribution();
        
        // Basic utility functions
        GammaDistribution*                              clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::GammaDistribution*                createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        RbPtr<const Variable>                           shape;
        RbPtr<const Variable>                           rate;
        
    };
    
}

#endif
