//
//  RlDist_wishart.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__Dist_wishart__
#define __revbayes__Dist_wishart__

#include <iostream>


#include "RealSymmetricMatrix.h"
#include "RlTypedDistribution.h"
#include "WishartDistribution.h"

namespace RevLanguage {
    
    class Dist_wishart :  public  TypedDistribution<RealSymmetricMatrix> {
        
    public:
        Dist_wishart( void );
        virtual ~Dist_wishart();
        
        // Basic utility functions
        Dist_wishart*                            clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::WishartDistribution*              createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
 //       RevPtr<const Variable>                          omega;
        RevPtr<const Variable>                          kappa;
        RevPtr<const Variable>                          df;
        RevPtr<const Variable>                          dim;
    };
    
}
#endif /* defined(__revbayes__Dist_wishart__) */
