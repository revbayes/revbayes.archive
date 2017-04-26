#ifndef RlGeographyRateModifier_H
#define RlGeographyRateModifier_H

#include "ModelObject.h"
#include "GeographyRateModifier.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class GeographyRateModifier : public ModelObject<RevBayesCore::GeographyRateModifier> {
        
    public:
        
        GeographyRateModifier(void);                                                                                  //!< Default constructor
        GeographyRateModifier(RevBayesCore::GeographyRateModifier *m);                                       //!< Default constructor
        GeographyRateModifier(RevBayesCore::TypedDagNode<RevBayesCore::GeographyRateModifier> *d);           //!< Default constructor
        
        // Basic utility functions
        GeographyRateModifier*              clone(void) const;                                                          //!< Clone object
        static const std::string&           getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        
<<<<<<< HEAD
        std::string                         getGuiName(void) { return ""; }
        std::string                         getGuiSymbol(void) { return ""; }
        std::string                         getGuiInfo(void) { return ""; }
=======
>>>>>>> c54d844d6bccb46f193d8e4b18ac6f817cb88097
    };
}
#endif
