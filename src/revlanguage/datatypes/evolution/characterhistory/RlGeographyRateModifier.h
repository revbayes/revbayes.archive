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
        static const std::string&           getClassType(void);                                                         //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                                     //!< Get methods
        RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
    };
    
}
#endif /* defined(RlGeographyRateModifier_H) */
