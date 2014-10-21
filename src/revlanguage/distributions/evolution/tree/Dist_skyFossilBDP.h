#ifndef Dist_skyFossilBDP_H
#define Dist_skyFossilBDP_H

#include "PiecewiseConstantFossilizedBirthDeathProcess.h"
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
    class Dist_skyFossilBDP :  public TypedDistribution<TimeTree> {
        
    public:
        Dist_skyFossilBDP( void );
        
        // Basic utility functions
        Dist_skyFossilBDP*   clone(void) const;          //!< Clone the object
        static const std::string&                       getClassType(void);         //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);     //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;    //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const; //!< Get member rules (const)
        
        // Distribution functions you have to override
        RevBayesCore::PiecewiseConstantFossilizedBirthDeathProcess* createDistribution(void) const;  //!< Create an internal object of the diveristy-dependent pure-birth process.
        
    protected:
        void setConstParameter(const std::string& name, const RevPtr<const Variable> &var);  //!< Set member variable
        
    private:
        // members        
        RevPtr<const Variable>                          lambda;
        RevPtr<const Variable>                          lambdaTimes;
        RevPtr<const Variable>                          mu;
        RevPtr<const Variable>                          muTimes;
        RevPtr<const Variable>                          psi;
        RevPtr<const Variable>                          psiTimes;
        RevPtr<const Variable>                          rho;
        RevPtr<const Variable>                          rhoTimes;
     // RevPtr<const Variable>                          tLast;
        RevPtr<const Variable>                          origin;                     //!< The time of the process since the origin
        RevPtr<const Variable>                          rootAge;                     //!< The time of the process since the origin
        RevPtr<const Variable>                          condition;                  //!< The condition of the process (none/survival/#Taxa)
        RevPtr<const Variable>                          taxonNames;                 //!< The taxon names that will be applied to the initally simulated tree
        RevPtr<const Variable>                          constraints;                //!< Topological constraints that will be used for calibrations
    };
}

#endif
