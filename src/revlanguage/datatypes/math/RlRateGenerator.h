#ifndef RlRateGenerator_H
#define RlRateGenerator_H

#include "ModelObject.h"
#include "RateGenerator.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class RateGenerator : public ModelObject<RevBayesCore::RateGenerator> {
        
    public:
        
        RateGenerator(void);                                                                                                        //!< Default constructor
        RateGenerator(const RevBayesCore::RateGenerator& m);                                                                        //!< Default constructor
        RateGenerator(RevBayesCore::RateGenerator *m);                                                                              //!< Default constructor
        RateGenerator(RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator> *d);                                                                                                        //!< Default constructor
        
        // Basic utility functions
        RateGenerator*                      clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method functions
        virtual RevPtr<RevVariable>         executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Map member methods to internal functions
<<<<<<< HEAD
      
        std::string                         getGuiName(void) { return ""; }
        std::string                         getGuiSymbol(void) { return ""; }
        std::string                         getGuiInfo(void) { return ""; }
=======
>>>>>>> c54d844d6bccb46f193d8e4b18ac6f817cb88097

    protected:
        void                                initMethods(void);
    };
    
}

#endif /* defined(__revbayes_proj__RlRateGenerator__) */
