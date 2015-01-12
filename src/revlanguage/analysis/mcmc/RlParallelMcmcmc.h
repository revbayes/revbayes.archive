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
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ParallelMcmcmc : public WorkspaceToCoreWrapperObject<RevBayesCore::ParallelMcmcmc> {
        
    public:
        
        ParallelMcmcmc(void);                                                                                                               //!< Default constructor
        
        // Basic utility functions
        virtual ParallelMcmcmc*                     clone(void) const;                                                                      //!< Clone object
        void                                        constructInternalObject(void);                                                          //!< We construct the a new internal MCMC object.
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        
        // Member method inits
        virtual RevPtr<RevVariable>                 executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);          //!< Set member variable
        
        RevPtr<const RevVariable>                   model;
        RevPtr<const RevVariable>                   moves;
        RevPtr<const RevVariable>                   monitors;
        RevPtr<const RevVariable>                   moveSchedule;
        RevPtr<const RevVariable>                   numChains;
        RevPtr<const RevVariable>                   startHeat;
        RevPtr<const RevVariable>                   deltaHeat;
        RevPtr<const RevVariable>                   sigmaHeat;
        RevPtr<const RevVariable>                   swapInterval;
        RevPtr<const RevVariable>                   append;
    };
    
}

#endif /* defined(__rb_mlandis__RlParallelMcmcmc__) */
