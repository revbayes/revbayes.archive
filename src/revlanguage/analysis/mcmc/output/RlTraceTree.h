#ifndef RlTraceTree_H
#define RlTraceTree_H

#include "TraceTree.h"
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"
#include "TreeSummary.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class TraceTree : public WorkspaceToCoreWrapperObject<RevBayesCore::TraceTree> {
        
    public:
        
//        TraceTree(void);                                                                                                                    //!< Default constructor
        TraceTree(const RevBayesCore::TraceTree& x);                                                                                              //!< Copy constructor
        
        // Basic utility functions
        virtual TraceTree*                          clone(void) const;                                                                      //!< Clone object
        void                                        constructInternalObject(void);                                                          //!< We construct the a new internal MCMC object.
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method inits
        virtual RevPtr<RevVariable>                 executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
    protected:
        
        
        void                                        initMethods(void);
        virtual void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);          //!< Set member variable
        
    private:
        
        RevBayesCore::TreeSummary                   tree_summary;
    };
    
}


#endif
