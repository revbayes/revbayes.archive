#ifndef Mntr_PosteriorPredictive_H
#define Mntr_PosteriorPredictive_H

#include "PosteriorPredictiveMonitor.h"
#include "RlMonitor.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Mntr_PosteriorPredictive : public Monitor {
        
    public:
        
        Mntr_PosteriorPredictive(void);                                                                                                 //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Mntr_PosteriorPredictive*           clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Mntr_PosteriorPredictive.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        RevPtr<const RevVariable>                   filename;
        RevPtr<const RevVariable>                   printgen;
        RevPtr<const RevVariable>                   separator;
        RevPtr<const RevVariable>                   append;
        
    };
    
}

#endif
