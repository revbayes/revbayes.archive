#ifndef Mntr_AncestralState_H
#define Mntr_AncestralState_H

#include "AncestralStateMonitor.h"
#include "RlMonitor.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Mntr_AncestralState : public Monitor {
        
    public:
        
        Mntr_AncestralState(void);                                                                                                 //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Mntr_AncestralState*                 clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Mntr_AncestralState.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
		std::vector<RevPtr<const Variable> >        vars;
        RevPtr<const Variable>                      filename;
        RevPtr<const Variable>                      printgen;
		RevPtr<const Variable>                      tree;
		RevPtr<const Variable>                      character;
        RevPtr<const Variable>                      separator;
        RevPtr<const Variable>                      append;
		RevPtr<const Variable>						monitorType;
        
    };
    
}

#endif