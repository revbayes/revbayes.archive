#ifndef Dist_FBDP_H
#define Dist_FBDP_H

#include "ConstantRateFossilizedBirthDeathProcess.h"
#include "RlBirthDeathProcess.h"
#include "RlTimeTree.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the constant-rate Fossilized-Birth-Death Process
     *
     * The RevLanguage wrapper of the constant-rate fossilzed-birth-death process connects
     * the variables/parameters of the process and creates the internal ConstantRateFossilizedBirthDeathProcess object.
     * Please read the ConstantRateFossilizedBirthDeathProcess.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *c
     */
    class Dist_FBDP : public BirthDeathProcess {
        
    public:
        Dist_FBDP( void );
        
        // Basic utility functions
        Dist_FBDP*                                              clone(void) const;                                                                      //!< Clone the object
        static const std::string&                               getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                                  getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::vector<std::string>                                getDistributionFunctionAliases(void) const;                                             //!< Get the alternative names used for the constructor function in Rev.
        std::string                                             getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const TypeSpec&                                         getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                                      getParameterRules(void) const;                                                          //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::AbstractBirthDeathProcess*                createDistribution(void) const;
        
    protected:
        
        void                                                    setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
        
    private:

        std::string                                             startCondition;                                                                         //!< The process start condition
        RevPtr<const RevVariable>                               startAge;                                                                               //!< The process start age
        RevPtr<const RevVariable>                               lambda;                                                                                 //!< The speciation rate(s)
        RevPtr<const RevVariable>                               mu;                                                                                     //!< The extinction rate(s)
        RevPtr<const RevVariable>                               psi;                                                                                    //!< The fossilization rate(s)
        RevPtr<const RevVariable>                               times;                                                                                  //!< The epoch times
        
    };
    
}

#endif
