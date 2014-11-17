#ifndef RlAbstractDiscreteCharacterData_H
#define RlAbstractDiscreteCharacterData_H

#include "AbstractDiscreteCharacterData.h"
#include "ModelObject.h"
#include "RlAbstractCharacterData.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class AbstractDiscreteCharacterData : public ModelObject<RevBayesCore::AbstractDiscreteCharacterData>, AbstractCharacterData {
        
    public:
        AbstractDiscreteCharacterData(void);                                                                                                //!< Constructor requires character type
        AbstractDiscreteCharacterData(const RevBayesCore::AbstractDiscreteCharacterData &d);                                                //!< Constructor requires character type
        AbstractDiscreteCharacterData(RevBayesCore::AbstractDiscreteCharacterData *d);                                                      //!< Constructor requires character type
        AbstractDiscreteCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData>*d);                           //!< Constructor requires character type
        
        typedef RevBayesCore::AbstractDiscreteCharacterData valueType;
        
        // Basic utility functions
        AbstractDiscreteCharacterData*              add(const RevObject& d) const;                                                          //!< Addition operator used for example in '+=' statements
        AbstractDiscreteCharacterData*              add(const AbstractDiscreteCharacterData& d) const;                                      //!< Addition operator used for example in '+=' statements
        virtual AbstractDiscreteCharacterData*      clone(void) const;                                                                      //!< Clone object
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method functions
        virtual RevPtr<RevVariable>                 executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
    };
    
}


#endif

