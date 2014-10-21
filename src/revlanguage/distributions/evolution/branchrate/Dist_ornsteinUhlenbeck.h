/* 
 * File:   Dist_ornsteinUhlenbeck.h
 * Author: nl
 *
 * Created on 26 juillet 2014, 15:24
 */

#ifndef DIST_ORNSTEINUHLENBECK_H
#define	DIST_ORNSTEINUHLENBECK_H


#include <iostream>


#include "OrnsteinUhlenbeckPhyloProcess.h"
#include "ModelVector.h"
#include "RlTypedDistribution.h"
#include "RlRealNodeValTree.h"
#include "RlSimplex.h"
#include "Real.h"

namespace RevLanguage {

    class Dist_ornsteinUhlenbeck :  public TypedDistribution< RealNodeValTree  > {
        
    public:
        Dist_ornsteinUhlenbeck( void ) {};
        virtual ~Dist_ornsteinUhlenbeck() {};
        
        // Basic utility functions
        Dist_ornsteinUhlenbeck*                           clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::OrnsteinUhlenbeckPhyloProcess*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          sigma;
        RevPtr<const Variable>                          mean;
        RevPtr<const Variable>                          phi;
        
    };
    
}


#endif	/* DIST_ORNSTEINUHLENBECK_H */

