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
    
    class PopulationDataReader {
        
    public:
        PopulationDataReader();
        SnpData* readSnpData(const std::string& fileName);
        SnpData* readSnpData2(const std::string& fileName);
        
    private:
        
        SnpData* snps;
        
    };
    
}


#endif /* defined(__revbayes_mlandis__PopulationDataReader__) */
