#ifndef RlNormalDistribution_H
#define RlNormalDistribution_H

#include "NormalDistribution.h"
#include "RlContinuousDistribution.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the normal distribution.
     *
     * The RevLanguage wrapper of the normal distribution takes care of create the internal distribution object
     * and provides the RevLanguage object that can be used within Rev.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-08-06, version 1.0
     *
     */
    class NormalDistribution :  public ContinuousDistribution {
    
        public:
        NormalDistribution( void );
    
        // Basic utility functions
        NormalDistribution*                             clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')

    
        // Distribution functions you have to override
        RevBayesCore::NormalDistribution*               createDistribution(void) const;
    
        protected:
    
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable

    
        private:
        
        RevPtr<const Variable>                          mean;
        RevPtr<const Variable>                          sd;
    
    };
    
}

#endif
