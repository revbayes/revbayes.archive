#ifndef RlMonitor_H
#define RlMonitor_H

#include "Monitor.h"
#include "RbHelpMonitor.h"
#include "RevVariable.h"
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"

#include <ostream>
#include <cstring>

namespace RevLanguage {
    
    class Monitor : public WorkspaceToCoreWrapperObject<RevBayesCore::Monitor> {
        
    public:
        
        Monitor(void);                                                                                                                      //!< Default constructor
        Monitor(RevBayesCore::Monitor *m);                                                                                                  //!< constructor with internal value
        
        // Basic utility functions
        virtual Monitor*                            clone(void) const = 0;                                                                  //!< Clone object
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        virtual RevBayesCore::RbHelpMonitor*        getHelpEntry(void) const;                                                               //!< Get the help entry for this function
        virtual void                                printValue(std::ostream& o, bool user) const;                                           //!< Print value (for user)
        virtual void                                printValue(std::ostream& o) const;                                           //!< Print value (for user)


        // Member method functions
        virtual RevPtr<RevVariable>                 executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);         //!< Map member methods to internal functions
        
        std::vector<std::string>                    getConstructorFunctionAliases(void) const;                                                  //!< Get the alternative names used for the constructor function in Rev.
        std::string                                 getConstructorFunctionName(void) const;
        virtual std::vector<std::string>            getMonitorAliases(void) const { return std::vector<std::string>(); }                        //!< Get the alternative names used for the constructor function in Rev.
        virtual std::string                         getMonitorName(void) const = 0;
        
    protected:
        
        
        virtual std::string                                 getConstructorUsage(void) const { return ""; }
        virtual std::vector<std::string>                    getConstructorDetails(void) const { return std::vector<std::string>(); }
        virtual std::string                                 getConstructorExample(void) const { return ""; }
        virtual std::vector<std::string>                    getHelpAuthor(void) const { return std::vector<std::string>(); }
        virtual std::vector<std::string>                    getHelpDescription(void) const { return std::vector<std::string>(); }
        virtual std::vector<RevBayesCore::RbHelpReference>  getHelpReferences(void) const { return std::vector<RevBayesCore::RbHelpReference>(); }
        virtual std::vector<std::string>                    getHelpSeeAlso(void) const { return std::vector<std::string>(); }
        virtual std::string                                 getHelpTitle(void) const { return ""; }

        static bool                                 compareVarNames(RevPtr<const RevVariable> i, RevPtr<const RevVariable> j) { return strcmp(i->getName().c_str(), j->getName().c_str()) < 0; }

    };
    
}

#endif
