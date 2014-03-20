//
//  RlPoissonDistribution.h
//  RevBayes
//
//  Created by Nicolas Lartillot on 2014-03-18.
//
//

#ifndef RlPoissonDistribution_h
#define RlPoissonDistribution_h

#include "PoissonDistribution.h"
#include "Natural.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the Poisson distribution.
     *
     * The RevLanguage wrapper of the Poisson distribution simply
     * manages the interactions through the Rev with our core.
     * That is, the internal distribution object can be constructed and hooked up
     * in a model graph.
     * See the PoissonDistribution.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-08-08, version 1.0
     *
     */
    class PoissonDistribution :  public TypedDistribution<Natural> {
        
    public:
        PoissonDistribution( void );                                                                                                                //!< Default constructor
        
        // Basic utility functions
        PoissonDistribution*                          clone(void) const;                                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                                     //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::PoissonDistribution*            createDistribution(void) const;                                                                 //!< Create the internal distribution object
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        
    private:
        RbPtr<const Variable>                           lambda;                                                                                         //!< The rate of the distribution
        
    };
    
}


#endif
