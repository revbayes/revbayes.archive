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
    
        AbstractCharacterData( RevBayesCore::AbstractCharacterData *o);                                                                                                 //!< Constructor requires character type
        
        MethodTable                                     getCharacterDataMethods(void) const;                                                                            //!< Get the common character data methods
        RevPtr<RevVariable>                             executeCharacterDataMethod(const std::string& name, const std::vector<Argument>& args, bool &found);            //!< Override to map member methods to internal functions
        void                                            setCharacterDataObject( RevBayesCore::AbstractCharacterData *o );                                               //!< Set the pointer to the internal object
        RevBayesCore::AbstractCharacterData*            getValue(void) { return charDataObject; }
        RevBayesCore::AbstractCharacterData*            getValue(void) const { return charDataObject; }
        
        virtual AbstractCharacterData*                  clone(void) const = 0;                                                                          //!< Clone object
        static const std::string&                       getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                         //!< Get class type spec
        virtual const TypeSpec&                         getTypeSpec(void) const = 0;                                                                    //!< Get the object type spec of the instance

    private:
        
        RevBayesCore::AbstractCharacterData*            charDataObject;
        
    };
    
}


#endif

