#ifndef RlHomologousCharacterData_H
#define RlHomologousCharacterData_H

#include "HomologousCharacterData.h"
#include "RlAbstractCharacterData.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class HomologousCharacterData : public AbstractCharacterData {
        
    public:
        
        HomologousCharacterData( RevBayesCore::HomologousCharacterData *o);                                                                                                 //!< Constructor requires character type
        
        MethodTable                                         getCharacterDataMethods(void) const;                                                                            //!< Get the common character data methods
        RevPtr<RevVariable>                                 executeCharacterDataMethod(const std::string& name, const std::vector<Argument>& args, bool &found);            //!< Override to map member methods to internal functions
        void                                                setCharacterDataObject( RevBayesCore::AbstractCharacterData *o );                                               //!< Set the pointer to the internal object
        void                                                setCharacterDataObject( RevBayesCore::HomologousCharacterData *o );                                             //!< Set the pointer to the internal object
        
        
    private:
        
        RevBayesCore::HomologousCharacterData*              charDataObject;
        
    };
    
}


#endif

