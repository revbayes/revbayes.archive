//
//  RlWhiteNoiseProcess.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RlWhiteNoiseProcess__
#define __revbayes__RlWhiteNoiseProcess__

#include <iostream>


#include "WhiteNoisePhyloProcess.h"
#include "ModelVector.h"
#include "RlTypedDistribution.h"
#include "RlSimplex.h"
#include "RealPos.h"

namespace RevLanguage {

    class Dist_whiteNoise :  public TypedDistribution< ModelVector<RealPos>  > {
        
    public:
        Dist_whiteNoise( void );
        virtual ~Dist_whiteNoise();
        
        // Basic utility functions
        Dist_whiteNoise*                                clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        
        // Distribution functions you have to override
        RevBayesCore::WhiteNoisePhyloProcess*           createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          sigma;
        
    };
    
}

#endif /* defined(__revbayes__RlWhiteNoiseProcess__) */
