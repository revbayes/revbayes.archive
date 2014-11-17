#ifndef Dist_mvtnormal_H
#define Dist_mvtnormal_H

#include "MultivariateNormalDistribution.h"
#include "ModelVector.h"
#include "Real.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the multivariate normal distribution.
     *
     * The RevLanguage wrapper of the multivariate normal distribution simply
     * manages the interactions through the Rev with our core.
     * That is, the internal distribution object can be constructed and hooked up
     * in a model graph.
     * See the MultivariateNormalDistribution for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Nicolas Lartillot)
     * @since 2014-07-15, version 1.0
     *
     */
    class Dist_mvtnorm :  public TypedDistribution< ModelVector<Real> > {
        
    public:
                                                        Dist_mvtnorm( void );
                                               virtual ~Dist_mvtnorm();
        
        // Basic utility functions
        Dist_mvtnorm*                                   clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::MultivariateNormalDistribution*   createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        RevPtr<const RevVariable>                          mean;
        RevPtr<const RevVariable>                          precision;
        
    };
    
}

#endif
