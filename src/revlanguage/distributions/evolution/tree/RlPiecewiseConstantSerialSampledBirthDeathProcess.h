#ifndef RlPiecewiseConstantSerialSampledBirthDeathProcess_H
#define RlPiecewiseConstantSerialSampledBirthDeathProcess_h

#include "PiecewiseConstantSerialSampledBirthDeathProcess.h"
#include "RlTypedDistribution.h"
#include "RlTimeTree.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the PiecewiseConstantSerialSampledBirthDeathProcess
     *
     * The RevLanguage wrapper of the constant-rate serial-sampled birth-death process connects
     * the variables/parameters of the process and creates the internal PiecewiseConstantSerialSampledBirthDeathProcess object.
     * Please read the PiecewiseConstantSerialSampledBirthDeathProcess.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-19, version 1.0
     *
     */
    class PiecewiseConstantSerialSampledBirthDeathProcess :  public TypedDistribution<TimeTree> {
        
    public:
        PiecewiseConstantSerialSampledBirthDeathProcess( void );
        
        // Basic utility functions
        PiecewiseConstantSerialSampledBirthDeathProcess*                    clone(void) const;                                                                  //!< Clone the object
        static const std::string&                                           getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&                                              getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&                                                     getTypeSpec(void) const;                                                            //!< Get the type spec of the instance
        const MemberRules&                                                  getMemberRules(void) const;                                                         //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::PiecewiseConstantSerialSampledBirthDeathProcess*      createDistribution(void) const;                                                     //!< Create an internal object of the diveristy-dependent pure-birth process.
        
    protected:
        
        void                                                                setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);  //!< Set member variable
        
        
    private:
        
        // members        
        RbPtr<const Variable>                                               lambda;
        RbPtr<const Variable>                                               lambdaTimes;
        RbPtr<const Variable>                                               mu;
        RbPtr<const Variable>                                               muTimes;
        RbPtr<const Variable>                                               psi;
        RbPtr<const Variable>                                               psiTimes;
        RbPtr<const Variable>                                               rho;
        RbPtr<const Variable>                                               rhoTimes;
        RbPtr<const Variable>                                               tLast;
        RbPtr<const Variable>                                               origin;                                                                             //!< The time of the process since the origin
        RbPtr<const Variable>                                               condition;                                                                          //!< The condition of the process (none/survival/#Taxa)
        RbPtr<const Variable>                                               taxonNames;                                                                         //!< The taxon names that will be applied to the initally simulated tree
        RbPtr<const Variable>                                               constraints;                                                                        //!< Topological constraints that will be used for calibrations
        
    };
    
}

#endif
