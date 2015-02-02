//
//  PomoStateConverter.h
//  RevBayes
//
//  Created by Bastien on 12/08/14.
//  Copyright (c) 2014 Bastien Boussau. All rights reserved.
//

#ifndef PomoStateConverter_H
#define PomoStateConverter_H

#include <map>

#include "AbstractDiscreteCharacterData.h"
#include "DiscreteCharacterData.h"
#include "PomoState.h"


namespace RevBayesCore {
    
    /**
     * This class enables conversion of a DNA matrix into a PomoState matrix.
     * The PomoState matrix can then be analyzed with the Pomo model.
     *
     * This class currently has only one functionality,
     * to convert the DNA data into PomoState data.
     *
     */
    class PomoStateConverter {
        
    public:
        PomoStateConverter();
        
        DiscreteCharacterData<PomoState>*  convertData(const AbstractDiscreteCharacterData &d, const unsigned int virtualPopulationSize, const std::map<std::string, std::string> sequenceNameToSpeciesName);
        
    private:
        PomoState* convertCounts(std::vector<double> &counts,
                                        const unsigned int virtualPopulationSize,
                                        std::vector< std::vector<double> > &frequencies);
        
    };
    
}


#endif /* defined(PomoStateConverter_H) */
