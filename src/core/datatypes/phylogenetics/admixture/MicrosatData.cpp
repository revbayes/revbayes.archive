//
//  MicrosatData.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/14/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "MicrosatData.h"

using namespace RevBayesCore;

double MicrosatData::getMicrosatMeans(std::string a, int b) const
{
    for (unsigned i = 0; i < numPopulations; i++)
    {
        if (populationNames[i] == a)
        {
            return microsatMeans[i][b];
        }
    }
    std::cout << "ERROR " << a << "\n";
    return -1.0;
}

const std::vector<double>& MicrosatData::getMicrosatMeans(std::string a) const
{
    for (unsigned i = 0; i < numPopulations; i++)
    {
        if (populationNames[i] == a)
        {
            return microsatMeans[i];
        }
    }
    std::cout << a << " not found in MicrosatData\n";
    return microsatMeans[-1];
}
