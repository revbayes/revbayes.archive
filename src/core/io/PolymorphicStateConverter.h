//
//  PolymorphicStateConverter.h
//  RevBayes
//
//  Created by Bastien on 12/08/14.
//  Copyright (c) 2014 Bastien Boussau. All rights reserved.
//

#ifndef PolymorphicStateConverter_H
#define PolymorphicStateConverter_H

#include <map>

#include "AbstractDiscreteCharacterData.h"
#include "DiscreteCharacterData.h"
#include "PolymorphicState.h"


namespace RevBayesCore {
    
    /**
     * This class enables conversion of a DNA matrix into a PolymorphicState matrix.
     * The PolymorphicState matrix can then be analyzed with the Pomo model.
     *
     * This class currently has only one functionality,
     * to convert the DNA data into PolymorphicState data.
     *
     */
    class PolymorphicStateConverter {
        
    public:
        PolymorphicStateConverter();
        
        DiscreteCharacterData<PolymorphicState>*  convertData(const AbstractCharacterData &d, const unsigned int virtualPopulationSize, const std::map<std::string, std::string> sequenceNameToSpeciesName);
        
    private:
        PolymorphicState* convertCounts(std::vector<double> &counts,
                                        const unsigned int virtualPopulationSize,
                                        std::vector< std::vector<double> > &frequencies);
        
    };
    
}


#endif /* defined(PolymorphicStateConverter_H) */
