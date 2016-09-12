#ifndef Dist_CharacterDependentCladoBirthDeathProcess_H
#define Dist_CharacterDependentCladoBirthDeathProcess_H

#include "CharacterDependentCladoBirthDeathProcess.h"
#include "RlTimeTree.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * @file
     * This file contains the declaration of the Rev character-dependent cladogenetic 
     * birth-death tree distribution: ClaSSE from Goldberg & Igic 2012
     *
     * @author Will Freyman
     */
    class Dist_CharacterDependentCladoBirthDeathProcess : public TypedDistribution<TimeTree> {
        
    public:
        Dist_CharacterDependentCladoBirthDeathProcess( void );      //!< Constructor
        virtual ~Dist_CharacterDependentCladoBirthDeathProcess();   //!< Virtual destructor
        
        // Basic utility functions
        Dist_CharacterDependentCladoBirthDeathProcess*              clone(void) const;                                                                      //!< Clone the object
        static const std::string&                                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                                 getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        virtual MethodTable                                         getDistributionMethods( void ) const;                                                                       //!< Get the member methods
        const TypeSpec&                                             getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        
        // Distribution functions you have to override
        RevBayesCore::CharacterDependentCladoBirthDeathProcess*     createDistribution(void) const;                                                         //!< Create the core object corresponding to this wrapper
        
    protected:
        
        void                                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
    private:
        
        RevPtr<const RevVariable>                                   cladoEvents;
        RevPtr<const RevVariable>                                   root_age;
        RevPtr<const RevVariable>                                   root_frequencies;
        RevPtr<const RevVariable>                                   extinction_rates;
        RevPtr<const RevVariable>                                   event_rate_matrix;
        RevPtr<const RevVariable>                                   event_rate;
        RevPtr<const RevVariable>                                   rho;
        RevPtr<const RevVariable>                                   condition;                                                                              //!< The condition of the process (none/survival/#Taxa)
        RevPtr<const RevVariable>                                   taxa;
        RevPtr<const RevVariable>                                   num_time_slices;
        
    };
    
}

#endif
