#ifndef RlNonHomologousCharacterData_H
#define RlNonHomologousCharacterData_H

#include "NonHomologousCharacterData.h"
#include "RlAbstractCharacterData.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class NonHomologousCharacterData : public AbstractCharacterData {
        
    public:
        
        NonHomologousCharacterData( RevBayesCore::NonHomologousCharacterData *o);                                                                                                 //!< Constructor requires character type
        
        MethodTable                                         getCharacterDataMethods(void) const;                                                                            //!< Get the common character data methods
        RevPtr<RevVariable>                                 executeCharacterDataMethod(const std::string& name, const std::vector<Argument>& args, bool &found);            //!< Override to map member methods to internal functions
        void                                                setCharacterDataObject( RevBayesCore::AbstractCharacterData *o );                                               //!< Set the pointer to the internal object
        void                                                setCharacterDataObject( RevBayesCore::NonHomologousCharacterData *o );                                             //!< Set the pointer to the internal object
        
        
    private:
        
        RevBayesCore::NonHomologousCharacterData*           charDataObject;
        
    };
    
}


#endif

