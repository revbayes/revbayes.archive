#ifndef RlAbstractCharacterData_H
#define RlAbstractCharacterData_H

#include "AbstractCharacterData.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class AbstractCharacterData : public ModelObject<RevBayesCore::AbstractCharacterData> {
        
    public:
        AbstractCharacterData(void);                                                                                                //!< Constructor requires character type
        AbstractCharacterData(RevBayesCore::AbstractCharacterData *v);                                                              //!< Constructor requires character type
        AbstractCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData>*d);                                   //!< Constructor requires character type
        
        typedef RevBayesCore::AbstractCharacterData valueType;
        
        // Basic utility functions
        AbstractCharacterData*              add(const RevObject& d) const;                                                          //!< Addition operator used for example in '+=' statements
        AbstractCharacterData*              add(const AbstractCharacterData& d) const;                                              //!< Addition operator used for example in '+=' statements
        virtual AbstractCharacterData*      clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        virtual const TypeSpec&             getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method functions
        virtual void                        initializeMethods(void) const;                                                          //!< Initialize methods
        virtual RevPtr<Variable>            executeMethod(const std::string& name, const std::vector<Argument>& args);              //!< Override to map member methods to internal functions

    };
    
}


#endif

