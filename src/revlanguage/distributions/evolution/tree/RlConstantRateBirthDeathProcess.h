#ifndef RlConstantRateBirthDeathProcess_H
#define RlConstantRateBirthDeathProcess_H

#include "ConstantRateBirthDeathProcess.h"
#include "RlBirthDeathProcess.h"
#include "RlTimeTree.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the ConstantRateBirthDeathProcess
     *
     * The RevLanguage wrapper of the constant-rate birth-death process connects
     * the variables/parameters of the process and creates the internal ConstantRateBirthDeathProcess object.
     * Please read the ConstantRateBirthDeathProcess.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *
     */
    class ConstantRateBirthDeathProcess :  public BirthDeathProcess {
        
    public:
        ConstantRateBirthDeathProcess( void );
        
        // Basic utility functions
        ConstantRateBirthDeathProcess*                  clone(void) const;                                                                      //!< Clone the object
        static const std::string&                       getClassName(void);                                                                     //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                             //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::ConstantRateBirthDeathProcess*    createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);      //!< Set member variable
        
        
    private:

        RbPtr<const Variable>                           lambda;                                                                                 //!< The speciation rate
        RbPtr<const Variable>                           mu;                                                                                     //!< The extinction rate
        
    };
    
}

#endif
