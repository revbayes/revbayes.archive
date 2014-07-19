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
        AbstractCharacterData*              clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method inits
        virtual const MethodTable&          getMethods(void) const;                                                                 //!< Get methods
        virtual RevPtr<Variable>            executeMethod(const std::string& name, const std::vector<Argument>& args);              //!< Override to map member methods to internal functions
        
    private: 
        void                                initMethods(void);
        
        MethodTable                         methods;
    };
    
}


#endif

