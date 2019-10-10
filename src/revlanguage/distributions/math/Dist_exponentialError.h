#ifndef DIST_EXPONENTIALERROR_H
#define DIST_EXPONENTIALERROR_H


#include <iostream>

#include "DistanceMatrix.h"
#include "TypedDistribution.h"
#include "RlDistanceMatrix.h"
#include "RlTypedDistribution.h"
#include "ExponentialErrorDistribution.h"

namespace RevLanguage {
    
    /*
     * The RevLanguage wrapper of the exponential error distribution.
     *
     * The RevLanguage wrapper of the exponential error distribution simply
     * manages the interactions through the Rev with our core.
     * That is, the internal distribution object can be constructed and hooked up
     * in a model graph.
     * See the ExponentialErrorDistribution.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author David Cerny
     * @since 2010-10-08
     *
     */
    class Dist_exponentialError : public TypedDistribution<DistanceMatrix> {
        
    public:
        Dist_exponentialError( void );
        virtual ~Dist_exponentialError();
        
        // Basic utility functions
        Dist_exponentialError*                          clone(void) const;                                                                      // < Clone the object
        static const std::string&                       getClassType(void);                                                                     // < Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                                 // < Get class type spec
        std::vector<std::string>                        getDistributionFunctionAliases(void) const;                                            // < Get the alternative names used for the constructor function in Rev.
        std::string                                     getDistributionFunctionName(void) const;                                            // < Get the Rev-name for this distribution.
        const TypeSpec&                                 getTypeSpec(void) const;                                                                // < Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                          // < Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                                      // < Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::ExponentialErrorDistribution*     createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);      // < Set member variable
        
        
    private:
        RevPtr<const RevVariable>                       avgDistMat;
        RevPtr<const RevVariable>                       lambda;
    };
    
}

#endif    /* DIST_EXPONENTIALERROR_H */
