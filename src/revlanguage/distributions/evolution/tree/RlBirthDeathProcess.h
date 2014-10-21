#ifndef RlBirthDeathProcess_H
#define RlBirthDeathProcess_H

#include "BirthDeathProcess.h"
#include "RlTimeTree.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the BirthDeathProcess.
     *
     * The RevLanguage wrapper of the birth-death process is the base-class for all
     * wrappers of homogeneous (i.e. each species has the same rate) birth-death process.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *
     */
    class BirthDeathProcess : public TypedDistribution<TimeTree> {
        
    public:
        BirthDeathProcess( void );
        
        // Basic utility functions you have to overwrite
        virtual BirthDeathProcess*                          clone(void) const = 0;                                                              //!< Clone the object

        // Basic utility functions you may want to overwrite
        const MemberRules&                                  getParameterRules(void) const;                                                         //!< Get member rules (const)
        
        // Basic utility functions
        static const std::string&                           getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                                                             //!< Get class type spec
        
        
        // Distribution functions you have to override
        virtual RevBayesCore::BirthDeathProcess*            createDistribution(void) const = 0;                                                 //!< Create an internal object of the diveristy-dependent pure-birth process.
        
    protected:
        
        void                                                setConstParameter(const std::string& name, const RevPtr<const Variable> &var); //!< Set member variable
    
        // members        
        RevPtr<const Variable>                              origin;                                                                             //!< The time of the process since the origin
        RevPtr<const Variable>                              rootAge;                                                                             //!< The time of the process since the origin
        RevPtr<const Variable>                              rho;                                                                                //!< The sampling probability of a species at present
        RevPtr<const Variable>                              samplingStrategy;                                                                   //!< The applied sampling strategy (uniform/diversified)
        RevPtr<const Variable>                              condition;                                                                          //!< The condition of the process (none/survival/#Taxa)
        RevPtr<const Variable>                              numTaxa;                                                                            //!< The number of taxa (only needed for simulation)
        RevPtr<const Variable>                              taxonNames;                                                                         //!< The taxon names that will be applied to the initally simulated tree
        RevPtr<const Variable>                              constraints;                                                                        //!< Topological constraints that will be used for calibrations
        
    };
    
}

#endif
