#ifndef RlPosteriorPredictiveCharacterDataSimulation_H
#define RlPosteriorPredictiveCharacterDataSimulation_H

#include "PosteriorPredictiveCharacterDataSimulation.h"
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    
    /**
     * RevLanguage wrapper class for the posterior predictive simulation object.
     *
     *
     * The wraper class provides the Rev interface to the core class PosteriorPredictiveSimulation.
     * See PosteriorPredictiveSimulation.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     *
     */
    class PosteriorPredictiveCharacterDataSimulation : public WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveCharacterDataSimulation> {
        
    public:
        
        PosteriorPredictiveCharacterDataSimulation(void);                                                                                                                    //!< Default constructor
        PosteriorPredictiveCharacterDataSimulation(const RevBayesCore::PosteriorPredictiveCharacterDataSimulation& x);                                                                                              //!< Copy constructor
        
        // Basic utility functions
        virtual PosteriorPredictiveCharacterDataSimulation*     clone(void) const;                                                                      //!< Clone object
        void                                                    constructInternalObject(void);                                                          //!< We construct the a new internal MCMC object.
        static const std::string&                               getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                                  getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                                      getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method inits
        virtual RevPtr<RevVariable>                             executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
    protected:
        
        virtual void                                            printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        void                                                    setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);          //!< Set member variable
        
        RevPtr<const RevVariable>                               model;
        RevPtr<const RevVariable>                               filetype;
        RevPtr<const RevVariable>                               directory;
        RevPtr<const RevVariable>                               trace;
        
    };
    
}

#endif
