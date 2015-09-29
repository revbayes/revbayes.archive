#ifndef Dist_serialBDP_H
#define Dist_serialBDP_H

#include "ConstantRateSerialSampledBirthDeathProcess.h"
#include "RlTypedDistribution.h"
#include "RlTimeTree.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the ConstantRateSerialSampledBirthDeathProcess
     *
     * The RevLanguage wrapper of the constant-rate serial-sampled birth-death process connects
     * the variables/parameters of the process and creates the internal ConstantRateSerialSampledBirthDeathProcess object.
     * Please read the ConstantRateSerialSampledBirthDeathProcess.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-19, version 1.0
     *
     */
    class Dist_serialBDP :  public TypedDistribution<TimeTree> {
        
    public:
        Dist_serialBDP( void );
        
        // Basic utility functions
        Dist_serialBDP*                 clone(void) const;                                                                  //!< Clone the object
        static const std::string&                                   getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                                      getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&                                             getTypeSpec(void) const;                                                            //!< Get the type spec of the instance
        const MemberRules&                                          getParameterRules(void) const;                                                         //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::ConstantRateSerialSampledBirthDeathProcess*   createDistribution(void) const;                                                     //!< Create an internal object of the diveristy-dependent pure-birth process.
        
    protected:
        
        void                                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var); //!< Set member variable
        
        
    private:
        
        // members        
        RevPtr<const RevVariable>                                   lambda;
        RevPtr<const RevVariable>                                   mu;
        RevPtr<const RevVariable>                                   psi;
        RevPtr<const RevVariable>                                   tLast;
        RevPtr<const RevVariable>                                   rho;
        RevPtr<const RevVariable>                                   origin;                                                                             //!< The time of the process since the origin
        RevPtr<const RevVariable>                                   rootAge;                                                                             //!< The time of the process since the origin
        RevPtr<const RevVariable>                                   condition;                                                                          //!< The condition of the process (none/survival/#Taxa)
        RevPtr<const RevVariable>                                   taxonNames;                                                                         //!< The taxon names that will be applied to the initally simulated tree
        RevPtr<const RevVariable>                                   constraints;                                                                        //!< Topological constraints that will be used for calibrations
        
    };
    
}

#endif
