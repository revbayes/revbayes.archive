/**
 * @file
 * This file contains the declaration of the Beta distribution, which is used create
 * random variables of beta distributions.
 *
 * @brief Declaration and implementation of Dist_beta
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Dist_beta_H
#define Dist_beta_H

#include "BetaDistribution.h"
#include "Probability.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    class Dist_beta :  public TypedDistribution<Probability> {
        
    public:
        Dist_beta( void );
        virtual ~Dist_beta();
        
        // Basic utility functions
        Dist_beta*                                      clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override                  
        virtual Probability*                            createRandomVariable(void) const;  
        RevBayesCore::BetaDistribution*                 createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        RevPtr<const RevVariable>                          alpha;
        RevPtr<const RevVariable>                          beta;
        
    };
    
}

#endif
