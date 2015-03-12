#ifndef Dist_BirthDeathMultiRate_H
#define Dist_BirthDeathMultiRate_H

#include "MultiRateBirthDeathProcess.h"
#include "RlBirthDeathProcess.h"
#include "RlTimeTree.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the Dist_BirthDeathMultiRate
     *
     * The RevLanguage wrapper of the constant-rate birth-death process connects
     * the variables/parameters of the process and creates the internal Dist_BirthDeathMultiRate object.
     * Please read the Dist_BirthDeathMultiRate.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *
     */
    class Dist_BirthDeathMultiRate :  public BirthDeathProcess {
        
    public:
        Dist_BirthDeathMultiRate( void );
        
        // Basic utility functions
        Dist_BirthDeathMultiRate*                       clone(void) const;                                                                      //!< Clone the object
        static const std::string&                       getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                             //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::ConstantRateBirthDeathProcess*    createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                       lambda;                                                                                 //!< The speciation rate
        RevPtr<const RevVariable>                       mu;                                                                                     //!< The extinction rate
        RevPtr<const RevVariable>                       Q;                                                                                 //!< The speciation rate
        RevPtr<const RevVariable>                       pi;                                                                                 //!< The speciation rate
        
    };
    
}

#endif
