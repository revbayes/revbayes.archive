//
//  RlParallelMcmcmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 3/25/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RlParallelMcmcmc__
#define __rb_mlandis__RlParallelMcmcmc__


#include "ParallelMcmcmc.h"
#include "Mcmc.h"
#include "RlControlVariableWrapper.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ParallelMcmcmc : public RlControlVariableWrapper<RevBayesCore::ParallelMcmcmc> {
        
    public:
        
        ParallelMcmcmc(void);                                                                                               //!< Default constructor
        ParallelMcmcmc(const ParallelMcmcmc& x);                                                                            //!< Copy constructor
        
        // Basic utility functions
        virtual ParallelMcmcmc*                     clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal MCMC object.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
        // Member method inits
        const MethodTable&                          getMethods(void) const;                                             //!< Get methods
        RbLanguageObject*                           executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        RbPtr<const Variable>                       model;
        RbPtr<const Variable>                       moves;
        RbPtr<const Variable>                       monitors;
        RbPtr<const Variable>                       moveSchedule;
        RbPtr<const Variable>                       numChains;
        RbPtr<const Variable>                       numProcessors;
        RbPtr<const Variable>                       startHeat;
        RbPtr<const Variable>                       deltaHeat;
        RbPtr<const Variable>                       sigmaHeat;
        RbPtr<const Variable>                       swapInterval;
        RbPtr<const Variable>                       append;
    };
    
}

#endif /* defined(__rb_mlandis__RlParallelMcmcmc__) */
