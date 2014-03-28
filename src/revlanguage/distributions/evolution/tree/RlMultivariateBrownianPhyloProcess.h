//
//  RlWhiteNoiseProcess.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RlMultivariateBrownianPhyloProcess__
#define __revbayes__RlMultivariateBrownianPhyloProcess__

#include <iostream>


#include "MultivariateBrownianPhyloProcess.h"
#include "RlTypedDistribution.h"
#include "RealMatrix.h"
#include "Real.h"

namespace RevLanguage {

    class MultivariateBrownianPhyloProcess :  public TypedDistribution< RealMatrix  > {
        
    public:
        MultivariateBrownianPhyloProcess( void ) {};
        virtual ~MultivariateBrownianPhyloProcess() {};
        
        // Basic utility functions
        MultivariateBrownianPhyloProcess*               clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::MultivariateBrownianPhyloProcess*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        
        RbPtr<const Variable>                           tree;
        RbPtr<const Variable>                           omega;
        RbPtr<const Variable>                           rootval;
        
    };
    
}

#endif /* defined(__revbayes__RlMultivariateBrownianPhyloProcess__) */
