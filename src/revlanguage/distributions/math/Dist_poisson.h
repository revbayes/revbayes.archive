//
//  RlDist_poisson.h
//  RevBayes
//
//  Created by Nicolas Lartillot on 2014-03-18.
//
//

#ifndef Dist_poisson_h
#define Dist_poisson_h

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
     * See the Dist_poisson.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-08-08, version 1.0
     *
     */
    class Dist_poisson :  public TypedDistribution<Natural> {
        
    public:
        Dist_poisson( void );                                                                                                                //!< Default constructor
        
        // Basic utility functions
        Dist_poisson*                            clone(void) const;                                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                                     //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::PoissonDistribution*              createDistribution(void) const;                                                                 //!< Create the internal distribution object
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);             //!< Set member variable
        
        
    private:
        RevPtr<const RevVariable>                          lambda;                                                                                         //!< The rate of the distribution
        
    };
    
}


#endif
