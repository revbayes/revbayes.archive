#ifndef RlAbstractCharacterData_H
#define RlAbstractCharacterData_H

#include "AbstractCharacterData.h"
#include "ModelObject.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class AbstractCharacterData {
        
    public:
    
        AbstractCharacterData( RevBayesCore::AbstractCharacterData *o);                                                                                                 //!< Constructor requires character type
        
        virtual MethodTable                             getCharacterDataMethods(void) const;                                                                            //!< Get the common character data methods
        virtual RevPtr<RevVariable>                     executeCharacterDataMethod(const std::string& name, const std::vector<Argument>& args, bool &found);            //!< Override to map member methods to internal functions
        virtual void                                    setCharacterDataObject( RevBayesCore::AbstractCharacterData *o );                                               //!< Set the pointer to the internal object
        
        
    private:
        
        RevBayesCore::AbstractCharacterData*            charDataObject;
        
    };
    
}


#endif

