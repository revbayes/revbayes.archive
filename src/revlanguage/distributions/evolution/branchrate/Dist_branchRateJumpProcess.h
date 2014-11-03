#ifndef Dist_branchRateJumpProcess_H
#define Dist_branchRateJumpProcess_H

#include "BranchRateJumpProcess.h"
#include "ModelVector.h"
#include "RlTypedDistribution.h"
#include "RealPos.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the BranchRateJumpProcess.
     *
     * The RevLanguage wrapper of the branch-rate jump process.
     * This process is used to get branch-wise rate multipliers, e.g., for the random local clock.
     * See BranchRateJumpProcess.h
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-29, version 1.0
     *
     */
    class Dist_branchRateJumpProcess :  public TypedDistribution< ModelVector<RealPos>  > {
        
    public:
        Dist_branchRateJumpProcess( void );
        
        // Basic utility functions
        Dist_branchRateJumpProcess*                      clone(void) const;                                                                              //!< Clone the object
        static const std::string&                   getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                         //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;                                                                        //!< Get the type spec of the instance
        const MemberRules&                          getParameterRules(void) const;                                                                     //!< Get member rules (const)
//        void                                        printValue(std::ostream& o) const;                                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::BranchRateJumpProcess*        createDistribution(void) const;
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                      valueDistribution;
        RevPtr<const Variable>                      tree;
        RevPtr<const Variable>                      lambda;
        RevPtr<const Variable>                      instantaneousJumpProbability;
        
    };
    
}

#endif
