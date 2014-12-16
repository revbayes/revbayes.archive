//
//  PopulationDataReader.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 12/7/12.
//  Copyright (c) 2012 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__PopulationDataReader__
#define __revbayes_mlandis__PopulationDataReader__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace RevBayesCore {
    
    class SnpData;
    class MicrosatData;
    
    class PopulationDataReader {
        
    public:
        PopulationDataReader();
        SnpData* readSnpData2(const std::string& fileName);
        SnpData* readSnpData(const std::string& fileName, int thinBy=1);
        MicrosatData* readMicrosatData(const std::string& fileName, int thinBy=1);
        
    private:
        
        SnpData* snps;
        MicrosatData* msats;
        
    };
    
}


#endif /* defined(__revbayes_mlandis__PopulationDataReader__) */
