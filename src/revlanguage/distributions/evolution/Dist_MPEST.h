//
//  Dist_MPEST.h
//  RevBayes
//
//  Created by Bastien on 30/10/14.
//  Copyright (c) 2014 Bastien Boussau. All rights reserved.
//
#ifndef Dist_MPEST_H
#define Dist_MPEST_H

#include "MPEST.h"
#include "RlTimeTree.h"
#include "RlTypedDistribution.h"
#include "RlRootedTripletDistribution.h"


namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the MPEST process
     *
     * The RevLanguage wrapper of the Multispecies Coalescent with constant population process connects
     * the variables/parameters of the process and creates the internal MPEST object.
     * Please read the MPEST.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *
     */
    class Dist_MPEST :  public  TypedDistribution<RootedTripletDistribution> {
        
    public:
        Dist_MPEST( void );
        
        // Basic utility functions
        Dist_MPEST*    clone(void) const;                                                                      //!< Clone the object
        static const std::string&                           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                                     getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                                  getMemberRules(void) const;                                                             //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::MPEST*               createDistribution(void) const;
        
    protected:
        
        void                                                setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                              geneTrees;                                                                                     //!< The population size
        RevPtr<const Variable>                              speciesTree;                                                                            //!< The species tree
        RevPtr<const Variable>                              useSpecies;                                                                                 //!< The taxons
        
        
    };
    
}

#endif
