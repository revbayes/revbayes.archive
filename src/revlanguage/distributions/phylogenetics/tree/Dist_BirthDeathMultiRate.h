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
     * @since 2015-03-11, version 1.0
     *
     */
    class Dist_BirthDeathMultiRate : public TypedDistribution<TimeTree> {
        
    public:
        Dist_BirthDeathMultiRate( void );
        
        // Basic utility functions
        Dist_BirthDeathMultiRate*                       clone(void) const;                                                                      //!< Clone the object
        static const std::string&                       getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::vector<std::string>                        getDistributionFunctionAliases( void ) const;
        std::string                                     getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                          //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::MultiRateBirthDeathProcess*       createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        double                                          lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const { throw RbException("Cannot compute P(nTaxa)."); }

        
    private:
        
        RevPtr<const RevVariable>                       rootAge;                                                                                //!< The time of the process since the origin
        RevPtr<const RevVariable>                       rho;                                                                                    //!< The sampling probability of a species at present
        RevPtr<const RevVariable>                       condition;                                                                              //!< The condition of the process (none/survival/#Taxa)
        RevPtr<const RevVariable>                       taxa;                                                                                   //!< The taxon names that will be applied to the initally simulated tree
        RevPtr<const RevVariable>                       lambda;                                                                                 //!< The speciation rate
        RevPtr<const RevVariable>                       mu;                                                                                     //!< The extinction rate
        RevPtr<const RevVariable>                       Q;                                                                                      //!< The rate matrix
        RevPtr<const RevVariable>                       event_rate;                                                                             //!< The speciation rate
        RevPtr<const RevVariable>                       pi;                                                                                     //!< The stationary frequenies
        
    };
    
}

#endif
