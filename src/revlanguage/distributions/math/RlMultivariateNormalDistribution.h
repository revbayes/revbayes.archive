/**
 * @file
 * This file contains the declaration of the MultivariateNormal distribution, which is used create
 * random variables of MultivariateNormal distributions.
 *
 * @brief Declaration of the MultivariateNormal distribution
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

#ifndef RlMultivariateNormalDistribution_H
#define RlMultivariateNormalDistribution_H

#include "MultivariateNormalDistribution.h"
#include "Vector.h"
#include "Real.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    class MultivariateNormalDistribution :  public TypedDistribution< Vector<Real> > {
        
    public:
        MultivariateNormalDistribution( void );
        virtual ~MultivariateNormalDistribution();
        
        // Basic utility functions
        MultivariateNormalDistribution*                          clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::MultivariateNormalDistribution*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        RbPtr<const Variable>                           mean;
        RbPtr<const Variable>                           precision;
        
    };
    
}

#endif
