//
//  Dist_branchHeterogeneousOrnsteinUhlenbeck.h
//  RevBayes
//
//  Created by Bastien on 05/10/14.
//  Copyright (c) 2014 Bastien Boussau. All rights reserved.
//

#ifndef __RevBayes__Dist_branchHeterogeneousOrnsteinUhlenbeck__
#define __RevBayes__Dist_branchHeterogeneousOrnsteinUhlenbeck__

#include <stdio.h>

#endif /* defined(__RevBayes__Dist_branchHeterogeneousOrnsteinUhlenbeck__) */



/*
 * File:   Dist_branchHeterogeneousOrnsteinUhlenbeck.h
 * Author: Bastien Boussau
 *
 * Created on 05/10/14.
 */

#ifndef DIST_BRANCHHETEROGENEOUSORNSTEINUHLENBECK_H
#define	DIST_BRANCHHETEROGENEOUSORNSTEINUHLENBECK_H


#include <iostream>


#include "BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess.h"
#include "ModelVector.h"
#include "RlTypedDistribution.h"
#include "RlRealNodeValTree.h"
#include "RlSimplex.h"
#include "Real.h"

namespace RevLanguage {
    
    class Dist_branchHeterogeneousOrnsteinUhlenbeck :  public TypedDistribution< RealNodeValTree  > {
        
    public:
        Dist_branchHeterogeneousOrnsteinUhlenbeck( void ) {};
        virtual ~Dist_branchHeterogeneousOrnsteinUhlenbeck() {};
        
        // Basic utility functions
        Dist_branchHeterogeneousOrnsteinUhlenbeck*                           clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          sigma;
        RevPtr<const Variable>                          mean;
        RevPtr<const Variable>                          phi;
        RevPtr<const Variable>                          rootMean;
        RevPtr<const Variable>                          rootStdDev;

    };
    
}


#endif	/* DIST_BRANCHHETEROGENEOUSORNSTEINUHLENBECK_H */

