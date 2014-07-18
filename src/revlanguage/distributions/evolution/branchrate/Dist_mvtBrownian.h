//
//  RlWhiteNoiseProcess.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__Dist_mvtBrownian__
#define __revbayes__Dist_mvtBrownian__

#include <iostream>


#include "MultivariateBrownianPhyloProcess.h"
#include "RlMultivariatePhyloProcess.h"
#include "RlTypedDistribution.h"
#include "RealMatrix.h"
#include "Real.h"

namespace RevLanguage {

    class Dist_mvtBrownian :  public TypedDistribution< MultivariatePhyloProcess  > {
        
    public:
        Dist_mvtBrownian( void ) {};
        virtual ~Dist_mvtBrownian() {};
        
        // Basic utility functions
        Dist_mvtBrownian*                               clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        /*
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RevObject*                      executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        */
        
        // Distribution functions you have to override
        RevBayesCore::MultivariateBrownianPhyloProcess* createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          sigma;
 //       RevPtr<const Variable>                          rootval;
        
    };
    
}

#endif /* defined(__revbayes__Dist_mvtBrownian__) */
