//
//  Mntr_JointConditionalJointConditionalAncestralState.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/28/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Mntr_JointConditionalJointConditionalAncestralState__
#define __revbayes_proj__Mntr_JointConditionalJointConditionalAncestralState__


#include "JointConditionalAncestralStateMonitor.h"
#include "RlMonitor.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Mntr_JointConditionalAncestralState : public Monitor {
        
    public:
        
        Mntr_JointConditionalAncestralState(void);                                                                          //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Mntr_JointConditionalAncestralState*    clone(void) const;                                      //!< Clone object
        void                                            constructInternalObject(void);                                          //!< We construct the a new internal Mntr_JointConditionalAncestralState.
        static const std::string&                       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                              getParameterRules(void) const;                                          //!< Get member rules (const)
        virtual const TypeSpec&                         getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                    printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
		std::vector<RevPtr<const RevVariable> >         vars;
        RevPtr<const RevVariable>                       filename;
        RevPtr<const RevVariable>                       printgen;
		RevPtr<const RevVariable>                       tree;
		RevPtr<const RevVariable>                       ctmc;
        RevPtr<const RevVariable>                       separator;
        RevPtr<const RevVariable>                       append;
		RevPtr<const RevVariable>                       monitorType;
        RevPtr<const RevVariable>                       withTips;
        RevPtr<const RevVariable>                       withStartStates;
    };
    
}

#endif /* defined(__revbayes_proj__Mntr_JointConditionalJointConditionalAncestralState__) */
