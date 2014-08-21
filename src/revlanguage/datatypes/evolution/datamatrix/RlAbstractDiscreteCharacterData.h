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
        virtual AbstractDiscreteCharacterData*              clone(void) const;                      //!< Clone object
        static const std::string&                           getClassType(void);                     //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                 //!< Get class type spec
        virtual const TypeSpec&                             getTypeSpec(void) const;                //!< Get language type of the object

        // Member method functions
        virtual const MethodTable&                          getMethods(void) const;                 //!< Get methods

        // Regular functions
        const RevBayesCore::AbstractDiscreteCharacterData&  getValue(void) const;                   //!< Get value as base class reference
        
    };
    
}


#endif

