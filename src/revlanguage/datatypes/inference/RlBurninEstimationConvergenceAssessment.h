#ifndef RlBurninEstimationConvergenceAssessment_H
#define RlBurninEstimationConvergenceAssessment_H

#include "Trace.h"
#include "TypedDagNode.h"
#include "RevObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev-wrapper of BurninEstimationConvergenceAssessment class.
     *
     * This class is the Rev-wrapper for BurninEstimationConvergenceAssessment core objetcs. See BurninEstimationConvergenceAssessment.h for more information.
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2012-09-06
     *
     */
    class BurninEstimationConvergenceAssessment : public RevObject {
        
    public:
        
        BurninEstimationConvergenceAssessment(void);                                                                                                             //!< Default constructor
        
        // Basic utility functions
        virtual BurninEstimationConvergenceAssessment*      clone(void) const;                                                          //!< Clone object
        static const std::string&                           getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                                                     //!< Get class type spec
        const MemberRules&                                  getMemberRules(void) const;                                                 //!< Get member rules (const)
        virtual const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language type of the object
        virtual void                                        printValue(std::ostream& o) const;                                          //!< Print value (for user)
        
        // Member method inits
        RevPtr<Variable>                                    executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        void                                                initializeMethods(void) const;                                              //!< Initialize methods
        
    protected:
        
        void                                                readTrace(const std::string &fn, std::vector<RevBayesCore::Trace> &data);
        void                                                setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        std::string                                         delimiter;
        std::set<std::string>                               filenames;
        std::string                                         burninMethod;
        bool                                                verbose;
    };
    
}

#endif
