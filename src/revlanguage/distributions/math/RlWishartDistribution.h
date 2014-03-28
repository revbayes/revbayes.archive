//
//  RlWishartDistribution.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RlWishartDistribution__
#define __revbayes__RlWishartDistribution__

#include <iostream>


#include "WishartDistribution.h"
#include "RlTypedDistribution.h"
#include "RealSymmetricMatrix.h"

namespace RevLanguage {
    
    class WishartDistribution :  public  TypedDistribution<RealSymmetricMatrix> {
        
    public:
        WishartDistribution( void );
        virtual ~WishartDistribution();
        
        // Basic utility functions
        WishartDistribution*                             clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::WishartDistribution*               createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        RbPtr<const Variable>                           omega;
        RbPtr<const Variable>                           kappa;
        RbPtr<const Variable>                           df;
        RbPtr<const Variable>                           dim;
    };
    
}
#endif /* defined(__revbayes__RlWishartDistribution__) */
