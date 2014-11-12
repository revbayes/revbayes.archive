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
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef Dist_mvtnormal_H
#define Dist_mvtnormal_H

#include "MultivariateNormalDistribution.h"
#include "ModelVector.h"
#include "Real.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    class Dist_mvtnorm :  public TypedDistribution< ModelVector<Real> > {
        
    public:
                                                        Dist_mvtnorm( void );
                                               virtual ~Dist_mvtnorm();
        
        // Basic utility functions
        Dist_mvtnorm*                                   clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::MultivariateNormalDistribution*   createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        RevPtr<const RevVariable>                          mean;
        RevPtr<const RevVariable>                          precision;
        
    };
    
}

#endif
