/**
 * @file
 * This file contains the declaration of the geometric Brownian Motion, which is used create
 * random variables of uniform distributions.
 *
 * @brief Declaration and implementation of geometric Brownian motion
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

#ifndef RlGeometricBrownianMotion_H
#define RlGeometricBrownianMotion_H

#include "Real.h"
#include "GeometricBrownianMotion.h"
#include "RlTypedDistribution.h"
#include "RlUnivariateFunction.h"

namespace RevLanguage {
    
    class GeometricBrownianMotion :  public TypedDistribution<UnivariateFunction> {
        
    public:
        GeometricBrownianMotion( void );
        virtual ~GeometricBrownianMotion();
        
        // Basic utility functions
        GeometricBrownianMotion*                        clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution<RevBayesCore::UnivariateFunction>*        createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        RbPtr<const Variable>                           initial;
        RbPtr<const Variable>                           sigma;
        RbPtr<const Variable>                           time;
        RbPtr<const Variable>                           nBlocks;
        
    };
    
}

#endif
