/**
 * @file
 * This file contains the declaration of Distribution, which is
 * the interface and abstract base class for RevLanguage distribution objects.
 *
 * @brief Declaration of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface Distribution
 * @package distributions
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Function.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef RlDistribution_H
#define RlDistribution_H

#include "Distribution.h"
#include "RbHelpDistribution.h"
#include "RevObject.h"

namespace RevLanguage {

    class Distribution : public RevObject {
    
    public:
        virtual                                            ~Distribution(void);                                                                 //!< Destructor
    
        // Basic utility functions you have to override
        virtual Distribution*                               clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                           getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                                                             //!< Get class type spec
       
//        virtual RevBayesCore::RbHelpDistribution*       getHelpEntry(void) const = 0;
        virtual RevBayesCore::RbHelpDistribution*           getHelpEntry(void) const;                                                           //!< Get the help entry for this function

        std::vector<std::string>                            getConstructorFunctionAliases(void) const;                                              //!< Get the alternative names used for the constructor function in Rev.
        std::string                                         getConstructorFunctionName(void) const;
        virtual std::vector<std::string>                    getDistributionFunctionAliases(void) const { return std::vector<std::string>(); }   //!< Get the alternative names used for the constructor function in Rev.
        virtual std::string                                 getDistributionFunctionName(void) const = 0;

        
        // Basit utility functions
        virtual const TypeSpec&                             getVariableTypeSpec(void) const = 0;                                                //!< Get the variable type spec of this distribution
        void                                                printValue(std::ostream& o) const;                                                  //!< Print value for user

        // GUI methods
        std::string                                         getGuiDistributionName(void) { return guiDistributionName; }
        std::string                                         getGuiDistributionToolTip(void) { return guiDistributionToolTip; }
        void                                                setGuiDistributionName(std::string s) { guiDistributionName = s; }
        void                                                setGuiDistributionToolTip(std::string s) { guiDistributionToolTip = s; }

        // Distribution functions you have to override
        virtual RevObject*                                  createRandomVariable(void) const = 0;                                               //!< Create a random variable from this distribution
        virtual RevBayesCore::Distribution*                 createDistribution(void) const = 0;                                                 //!< Create a random variable from this distribution
    
    
    protected:
        Distribution(void);
        
        
//        virtual std::string                                 getConstructorUsage(void) const = 0;
//        virtual std::vector<std::string>                    getConstructorDetails(void) const = 0;
//        virtual std::string                                 getConstructorExample(void) const = 0;
//        virtual std::string                                 getHelpAuthor(void) const = 0;
//        virtual std::vector<std::string>                    getHelpDescription(void) const = 0;
//        virtual std::vector<RevBayesCore::RbHelpReference>  getHelpReferences(void) const = 0;
//        virtual std::vector<std::string>                    getHelpSeeAlso(void) const = 0;
//        virtual std::string                                 getHelpTitle(void) const = 0;
        
        virtual std::string                                 getConstructorUsage(void) const { return ""; }
        virtual std::vector<std::string>                    getConstructorDetails(void) const { return std::vector<std::string>(); }
        virtual std::string                                 getConstructorExample(void) const { return ""; }
        virtual std::vector<std::string>                    getHelpAuthor(void) const { return std::vector<std::string>(); }
        virtual std::vector<std::string>                    getHelpDescription(void) const { return std::vector<std::string>(); }
        virtual std::vector<RevBayesCore::RbHelpReference>  getHelpReferences(void) const { return std::vector<RevBayesCore::RbHelpReference>(); }
        virtual std::vector<std::string>                    getHelpSeeAlso(void) const { return std::vector<std::string>(); }
        virtual std::string                                 getHelpTitle(void) const { return ""; }

        
    private:
        std::string                                         guiDistributionName;
        std::string                                         guiDistributionToolTip;
    };
    
}

#endif

