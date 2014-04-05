//
//  SnpData.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/14/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "SnpData.h"

using namespace RevBayesCore;

double SnpData::getSnpFrequencies(std::string a, int b) const
{
    for (unsigned i = 0; i < numPopulations; i++)
    {
        if (populationNames[i] == a)
        {
            return snpFrequencies[i][b];
        }
    }
    std::cout << "ERROR " << a << "\n";
    return -1.0;
}

const std::vector<double>& SnpData::getSnpFrequencies(std::string a) const
{
    for (unsigned i = 0; i < numPopulations; i++)
    {
        if (populationNames[i] == a)
        {
            return snpFrequencies[i];
        }
    }
    std::cout << a << " not found in SnpData\n";
    return snpFrequencies[-1];
}
