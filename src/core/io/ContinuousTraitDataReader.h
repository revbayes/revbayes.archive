#ifndef ContinuousTraitDataReader_H
#define ContinuousTraitDataReader_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace RevBayesCore {
    
    class TraitData;
    
    class ContinuousTraitDataReader {
        
    public:
        ContinuousTraitDataReader();
        TraitData* readTraitData(const std::string& fileName);
//        TraitData* readTraitData2(const std::string& fileName);
        
    private:
        
        TraitData* traits;
        
    };
    
}


#endif /* defined(__revbayes_mlandis__PopulationDataReader__) */
