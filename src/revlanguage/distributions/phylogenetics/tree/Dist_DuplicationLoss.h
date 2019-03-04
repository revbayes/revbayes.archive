#ifndef Dist_DuplicationLoss_H
#define Dist_DuplicationLoss_H

#include "DuplicationLossProcess.h"
#include "RlTimeTree.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the Duplication-Loss process
     *
     * The RevLanguage wrapper of the Duplication-Loss process connects
     * the variables/parameters of the process and creates the internal MultispeciesCoalescent object.
     * Please read the MultispeciesCoalescent.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *
     */
    class Dist_DuplicationLoss : public  TypedDistribution<TimeTree> {
        
    public:
        Dist_DuplicationLoss( void );
        
        // Basic utility functions
        Dist_DuplicationLoss*                               clone(void) const;                                                                      //!< Clone the object
        static const std::string&                           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                         getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const TypeSpec&                                     getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                                  getParameterRules(void) const;                                                          //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::DuplicationLossProcess*               createDistribution(void) const;
        
    protected:
        
        std::vector<std::string>                            getHelpAuthor(void) const;                                                              //!< Get the author(s) of this function
        std::vector<std::string>                            getHelpDescription(void) const;                                                         //!< Get the description for this function
        std::vector<std::string>                            getHelpDetails(void) const;                                                             //!< Get the more detailed description of the function
        std::string                                         getHelpExample(void) const;                                                             //!< Get an executable and instructive example
        std::vector<RevBayesCore::RbHelpReference>          getHelpReferences(void) const;                                                          //!< Get some references/citations for this function
        std::vector<std::string>                            getHelpSeeAlso(void) const;                                                             //!< Get suggested other functions
        std::string                                         getHelpTitle(void) const;                                                               //!< Get the title of this help entry
        
        
        void                                                setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                           lambda;                                                                                     //!< The population size
        RevPtr<const RevVariable>                           mu;                                                                                     //!< The population size
        RevPtr<const RevVariable>                           origin;                                                                                     //!< The population size
        RevPtr<const RevVariable>                           individual_tree;                                                                            //!< The species tree
        RevPtr<const RevVariable>                           taxa;                                                                                   //!< The taxons
        
        
    };
    
}

#endif
