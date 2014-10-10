#ifndef RlGeographyRateModifier_H
#define RlGeographyRateModifier_H

#include "ModelObject.h"
#include "GeographyRateModifier.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class RlGeographyRateModifier : public ModelObject<RevBayesCore::GeographyRateModifier> {
        
    public:
        
        RlGeographyRateModifier(void);                                                                                  //!< Default constructor
        RlGeographyRateModifier(RevBayesCore::GeographyRateModifier *m);                                       //!< Default constructor
        RlGeographyRateModifier(RevBayesCore::TypedDagNode<RevBayesCore::GeographyRateModifier> *d);           //!< Default constructor
        
        // Basic utility functions
        RlGeographyRateModifier*            clone(void) const;                                                          //!< Clone object
        static const std::string&           getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        
    };
    
}
#endif /* defined(RlGeographyRateModifier_H) */
