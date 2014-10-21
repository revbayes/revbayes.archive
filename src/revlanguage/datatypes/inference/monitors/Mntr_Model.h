#ifndef Mntr_Model_H
#define Mntr_Model_H

#include "ModelMonitor.h"
#include "RlMonitor.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Mntr_Model : public Monitor {
        
    public:
        
        Mntr_Model(void);                                                                                                 //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Mntr_Model*                         clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Mntr_Model.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        RevPtr<const Variable>                      filename;
        RevPtr<const Variable>                      printgen;
        RevPtr<const Variable>                      separator;
        RevPtr<const Variable>                      prior;
        RevPtr<const Variable>                      posterior;
        RevPtr<const Variable>                      likelihood;
        RevPtr<const Variable>                      append;
        RevPtr<const Variable>                      stochOnly;
        
    };
    
}

#endif
