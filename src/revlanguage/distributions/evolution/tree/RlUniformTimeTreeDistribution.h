#ifndef RlUniformTimeTreeDistribution_H
#define RlUniformTimeTreeDistribution_H

#include "UniformTimeTreeDistribution.h"
#include "RlTimeTree.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * @file
     * This file contains the declaration of the Rev uniform time tree distribution
     *
     * @brief Declaration of the Rev uniform time tree distribution
     *
     * @author Fredrik Ronquist
     */
 class UniformTimeTreeDistribution :  public TypedDistribution<TimeTree> {
        
    public:
                UniformTimeTreeDistribution( void );        //!< Constructor
        virtual ~UniformTimeTreeDistribution();             //!< Virtual destructor
        
        // Basic utility functions
        UniformTimeTreeDistribution*                    clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::UniformTimeTreeDistribution*      createDistribution(void) const;                                                 //!< Create the core object corresponding to this wrapper
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        
        RbPtr<const Variable>                           originTime;
        RbPtr<const Variable>                           taxonNames;
    };

}

#endif
