#ifndef RlMcmcmc_H
#define RlMcmcmc_H


#include "Mcmcmc.h"
#include "Mcmc.h"
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    
    /**
     * @brief Rev-wrapper of MCMCMC class.
     *
     * This class is the Rev-wrapper for MCMCMC core objetcs. See MCMC.h for more information.
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Michael Landis and Sebastian Hoehna)
     * @since Version 1.0, 2014-12-18
     *
     */
    class Mcmcmc : public WorkspaceToCoreWrapperObject<RevBayesCore::Mcmcmc> {
        
    public:
        
        Mcmcmc(void);                                                                                                               //!< Default constructor
        
        // Basic utility functions
        virtual Mcmcmc*                             clone(void) const;                                                                      //!< Clone object
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
        RevPtr<const RevVariable>                   deltaHeat;
        RevPtr<const RevVariable>                   swapInterval;
        RevPtr<const RevVariable>                   append;
    };
    
}

#endif /* defined(__rb_mlandis__RlMcmcmc__) */
