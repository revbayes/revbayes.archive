#ifndef RlDiscreteCharacterData_H
#define RlDiscreteCharacterData_H

#include "DiscreteCharacterData.h"
#include "RlAbstractCharacterData.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class DiscreteCharacterData : public ModelObject<RevBayesCore::AbstractCharacterData> {
        
    public:
        
        DiscreteCharacterData( RevLanguage::DiscreteCharacterData *o);                                                                                                 //!< Constructor requires character type
        
        MethodTable                                         getCharacterDataMethods(void) const;                                                                            //!< Get the common character data methods
        RevPtr<RevVariable>                                 executeCharacterDataMethod(const std::string& name, const std::vector<Argument>& args, bool &found);            //!< Override to map member methods to internal functions
        void                                                setCharacterDataObject( RevBayesCore::AbstractCharacterData *o );                                               //!< Set the pointer to the internal object
        void                                                setCharacterDataObject( DiscreteCharacterData *o );                                             //!< Set the pointer to the internal object
        
        
    private:
        
        RevLanguage::DiscreteCharacterData*              charDataObject;
        
    };
    
}


#endif


