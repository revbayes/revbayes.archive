//
//  RlWhiteNoiseProcess.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef Dist_PhyloBrownian_H
#define Dist_PhyloBrownian_H

#include <iostream>


#include "PhyloBrownianProcess.h"
#include "ModelVector.h"
#include "RlTypedDistribution.h"
#include "RlSimplex.h"
#include "Real.h"

namespace RevLanguage {

    class Dist_PhyloBrownian :  public TypedDistribution< ModelVector<Real>  > {
        
    public:
                                                        Dist_PhyloBrownian( void ) {};
        virtual                                        ~Dist_PhyloBrownian() {};
        
        // Basic utility functions
        Dist_PhyloBrownian*                             clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::PhyloBrownianProcess*             createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                       tree;
        RevPtr<const RevVariable>                       sigma;
        RevPtr<const RevVariable>                       drift;
        
    };
    
}

#endif /* defined(__revbayes__Dist_brownian__) */
