#ifndef PomoState4Converter_H
#define PomoState4Converter_H

#include <map>

#include "AbstractHomologousDiscreteCharacterData.h"
#include "HomologousDiscreteCharacterData.h"
#include "PomoState4.h"


namespace RevBayesCore {
    
    /**
     * This class enables conversion of a DNA matrix into a PomoState4 matrix.
     * The PomoState4 matrix can then be analyzed with the Pomo model.
     *
     * This class currently has only one functionality,
     * to convert the DNA data into PomoState4 data.
     *
     */
    class PomoState4Converter {
        
    public:
        PomoState4Converter();
        
        HomologousDiscreteCharacterData<PomoState4>*  convertData(const AbstractHomologousDiscreteCharacterData &d, const unsigned int virtualPopulationSize, const std::map<std::string, std::string> sequenceNameToSpeciesName);
        
    private:
        PomoState4* convertCounts(std::vector<double> &counts,
                                        const unsigned int virtualPopulationSize,
                                        std::vector< std::vector<double> > &frequencies);
        
    };
    
}


#endif /* defined(PomoState4Converter_H) */
