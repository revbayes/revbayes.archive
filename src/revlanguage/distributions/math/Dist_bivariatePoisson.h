#ifndef Dist_bivariatePoisson_H
#define Dist_bivariatePoisson_H

#include "BivariatePoissonDistribution.h"
#include "ModelVector.h"
#include "RbVector.h"
#include "RlTypedDistribution.h"

#include <vector>

namespace RevLanguage {


    /**
     *
     */
    class Dist_bivariatePoisson : public TypedDistribution< RbVector<int> > {

    public:
        Dist_bivariatePoisson( void );
        virtual ~Dist_bivariatePoisson();

        // Basic utility functions
        Dist_bivariatePoisson*                          clone(void) const;                                                                      //!< Clone the object
        static const std::string&                       getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        std::string                                     getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const MemberRules&                              getParameterRules(void) const;                                                          //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                                      //!< Print the general information on the function ('usage')


        // Distribution functions you have to override
        RevBayesCore::BivariatePoissonDistribution*     createDistribution(void) const;

    protected:

        std::vector<std::string>                        getHelpAuthor(void) const;                                                              //!< Get the author(s) of this function
        std::string                                     getHelpDescription(void) const;                                                         //!< Get the description for this function
        std::string                                     getHelpDetails(void) const;                                                             //!< Get the more detailed description of the function
        std::string                                     getHelpExample(void) const;                                                             //!< Get an executable and instructive example
        std::vector<RevBayesCore::RbHelpReference>      getHelpReferences(void) const;                                                          //!< Get some references/citations for this function
        std::vector<std::string>                        getHelpSeeAlso(void) const;                                                             //!< Get suggested other functions
        std::string                                     getHelpTitle(void) const;                                                               //!< Get the title of this help entry

        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable


    private:
        RevPtr<const RevVariable>                       theta0;
        RevPtr<const RevVariable>                       theta1;
        RevPtr<const RevVariable>                       theta2;

    };

}

#endif
