//
//  RlWhiteNoiseProcess.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RlBrownianPhyloProcess__
#define __revbayes__RlBrownianPhyloProcess__

#include <iostream>


#include "BrownianPhyloProcess.h"
#include "RlTypedDistribution.h"
#include "RlSimplex.h"
#include "Real.h"
#include "Vector.h"

namespace RevLanguage {

    class BrownianPhyloProcess :  public TypedDistribution< Vector<Real>  > {
        
    public:
        BrownianPhyloProcess( void ) {};
        virtual ~BrownianPhyloProcess() {};
        
        // Basic utility functions
        BrownianPhyloProcess*                           clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::BrownianPhyloProcess*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          sigma;
        RevPtr<const Variable>                          rootval;
        
    };
    
}

#endif /* defined(__revbayes__RlBrownianPhyloProcess__) */
