#ifndef Dist_CharacterDependentFossilizedBirthDeathProcess_H
#define Dist_CharacterDependentFossilizedBirthDeathProcess_H

#include "Dist_CharacterDependentBirthDeathProcess.h"
#include "RlTimeTree.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * @file
     * This file contains the declaration of the Rev character-dependent birth-death tree distribution
     *
     *
     * @author Sebastian Hoehna
     */
    class Dist_CharacterDependentFossilizedBirthDeathProcess : public Dist_CharacterDependentBirthDeathProcess {
        
    public:
        Dist_CharacterDependentFossilizedBirthDeathProcess( void );        //!< Constructor
        virtual ~Dist_CharacterDependentFossilizedBirthDeathProcess();             //!< Virtual destructor
        
        // Basic utility functions
        Dist_CharacterDependentFossilizedBirthDeathProcess*         clone(void) const;                                                                      //!< Clone the object
        static const std::string&                                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                                 getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const MemberRules&                                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution<RevBayesCore::Tree>*        createDistribution(void) const;                                                         //!< Create the core object corresponding to this wrapper
        
    protected:
        
        void                                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                                   fossilization_rates;
    };
    
}

#endif
