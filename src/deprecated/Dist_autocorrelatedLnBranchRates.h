/* 
 * File:   Dist_autocorrelatedLnBranchRates.h
 */

#ifndef Dist_autocorrelatedLnBranchRates_H
#define	Dist_autocorrelatedLnBranchRates_H


#include <iostream>


#include "AutocorrelatedLognormalRateDistribution.h"
#include "ModelVector.h"
#include "RlTypedDistribution.h"
#include "RlRealNodeValTree.h"
#include "RlSimplex.h"
#include "Real.h"

namespace RevLanguage {

    class Dist_autocorrelatedLnBranchRates :  public TypedDistribution< ModelVector<RealPos>  > {
        
    public:
        Dist_autocorrelatedLnBranchRates( void ) {};
        virtual ~Dist_autocorrelatedLnBranchRates() {};
        
        // Basic utility functions
        Dist_autocorrelatedLnBranchRates*                           clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::AutocorrelatedLognormalRateDistribution*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                          tree;
        RevPtr<const RevVariable>                          sigma;
        RevPtr<const RevVariable>                          rootRate;
        RevPtr<const RevVariable>                          scaleVal;
        
    };
    
}


#endif	/* Dist_autocorrelatedLnBranchRates_H */

