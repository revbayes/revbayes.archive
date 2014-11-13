/* 
 * File:   Dist_InverseWishart.h
 * Author: nl
 *
 * Created on 15 juillet 2014, 10:31
 */

#ifndef DIST_INVERSEWISHART_H
#define	DIST_INVERSEWISHART_H


#include <iostream>

#include "RealSymmetricMatrix.h"
#include "RlTypedDistribution.h"
#include "InverseWishartDistribution.h"

namespace RevLanguage {
    
    class Dist_inverseWishart :  public  TypedDistribution<RealSymmetricMatrix> {
        
    public:
        Dist_inverseWishart( void );
        virtual ~Dist_inverseWishart();
        
        // Basic utility functions
        Dist_inverseWishart*                            clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::InverseWishartDistribution*              createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        RevPtr<const RevVariable>                          sigma;
        RevPtr<const RevVariable>                          diagonal;
        RevPtr<const RevVariable>                          kappa;
        RevPtr<const RevVariable>                          df;
        RevPtr<const RevVariable>                          dim;
    };
    
}

#endif	/* DIST_INVERSEWISHART_H */

