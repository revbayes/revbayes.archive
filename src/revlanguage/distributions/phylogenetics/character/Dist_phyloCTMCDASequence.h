#ifndef Dist_phyloCTMCDASequence_H
#define Dist_phyloCTMCDASequence_H

#include "AbstractHomologousDiscreteCharacterData.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlRateGeneratorSequence.h"
#include "RlTypedDistribution.h"
#include "RlSimplex.h"
#include "Tree.h"

namespace RevLanguage {
    
    class Dist_phyloCTMCDASequence :  public TypedDistribution< AbstractHomologousDiscreteCharacterData > {
        
    public:
        Dist_phyloCTMCDASequence( void );
        virtual ~Dist_phyloCTMCDASequence();
        
        // Basic utility functions
        Dist_phyloCTMCDASequence*                               clone(void) const;                                                                      //!< Clone the object
        static const std::string&                       getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                     getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                          //!< Get member rules (const)
//        RevPtr<RevLanguage::RevVariable>                   executeMethod(std::string const &name, const std::vector<Argument> &args);
//        MethodTable                                     makeMethods(void) const;
        void                                            printValue(std::ostream& o) const;                                                      //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                       tree;
        RevPtr<const RevVariable>                       q;
        RevPtr<const RevVariable>                       rate;
        RevPtr<const RevVariable>                       cladoStateFreqs;
        RevPtr<const RevVariable>                       forbidExtinction;
        RevPtr<const RevVariable>                       useCladogenesis;
        RevPtr<const RevVariable>                       type;
        
    };
    
}


#endif
