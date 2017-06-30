#ifndef RlAbstractCharacterData_H
#define RlAbstractCharacterData_H

#include "AbstractCharacterData.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class AbstractCharacterData : public RevObject {
        
    public:
    
        AbstractCharacterData(void);                                                                                                 //!< Constructor requires character type
        
        MethodTable                                             getCharacterDataMethods(void) const;                                                                            //!< Get the common character data methods
        RevPtr<RevVariable>                                     executeCharacterDataMethod(const std::string& name, const std::vector<Argument>& args, bool &found);            //!< Override to map member methods to internal functions
        
        virtual AbstractCharacterData*                          clone(void) const = 0;                                                                          //!< Clone object
        static const std::string&                               getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                                  getClassTypeSpec(void);                                                                         //!< Get class type spec
        virtual const TypeSpec&                                 getTypeSpec(void) const = 0;                                                                    //!< Get the object type spec of the instance
        virtual RevBayesCore::AbstractCharacterData&            getValue(void) = 0;
        virtual const RevBayesCore::AbstractCharacterData&      getValue(void) const  = 0;
    };
    
}


#endif

