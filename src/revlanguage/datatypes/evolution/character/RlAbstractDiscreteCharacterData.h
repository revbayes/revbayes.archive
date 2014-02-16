#ifndef RlAbstractDiscreteCharacterData_H
#define RlAbstractDiscreteCharacterData_H

#include "AbstractDiscreteCharacterData.h"
#include "RlAbstractCharacterData.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class AbstractDiscreteCharacterData : public AbstractCharacterData {
        
    public:
        AbstractDiscreteCharacterData(void);                                                                                                                //!< Constructor requires character type
        AbstractDiscreteCharacterData(RevBayesCore::AbstractDiscreteCharacterData *v);                                                                      //!< Constructor requires character type
        AbstractDiscreteCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData>*d);                                                   //!< Constructor requires character type
        
        typedef RevBayesCore::AbstractCharacterData valueType;
        
        // Basic utility functions
        AbstractDiscreteCharacterData*                          clone(void) const;                                                                          //!< Clone object
        static const std::string&                               getClassName(void);                                                                         //!< Get class name
        static const TypeSpec&                                  getClassTypeSpec(void);                                                                     //!< Get class type spec
        const TypeSpec&                                         getTypeSpec(void) const;                                                                    //!< Get language type of the object
        
        // Member method
        const RevBayesCore::AbstractDiscreteCharacterData&      getValue(void) const;
        
    };
    
}


#endif

