#ifndef RlBimodalLognormalDistribution_H
#define RlBimodalLognormalDistribution_H

#include "BimodalLognormalDistribution.h"
#include "RlContinuousDistribution.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the bimodal lognormal distribution.
     *
     * The RevLanguage wrapper of the bimodal lognormal distribution takes care of create the internal distribution object
     * and provides the RevLanguage object that can be used within Rev.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-04-09, version 1.0
     *
     */
    class BimodalLognormalDistribution :  public ContinuousDistribution {
        
    public:
        BimodalLognormalDistribution( void );
        
        // Basic utility functions
        BimodalLognormalDistribution*                   clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::BimodalLognormalDistribution*     createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        
        RbPtr<const Variable>                           mean1;
        RbPtr<const Variable>                           mean2;
        RbPtr<const Variable>                           sd1;
        RbPtr<const Variable>                           sd2;
        RbPtr<const Variable>                           prob;
        
    };
    
}

#endif
