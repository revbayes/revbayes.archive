//
//  AreaSizeRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 10/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AreaSizeRateModifier.h"


#include <cmath>
#include "CharacterEvent.h"
#include "RbConstants.h"

using namespace RevBayesCore;

AreaSizeRateModifier::AreaSizeRateModifier(std::vector<double> asz, unsigned int sm) : areaSizes(asz), stateModified(sm), numAreas((unsigned int)asz.size())
{
    
    areaPowers.resize(numAreas,1.0);
    
    update();
}

AreaSizeRateModifier::AreaSizeRateModifier(const AreaSizeRateModifier& m) : areaSizes(m.areaSizes), areaPowers(m.areaPowers), stateModified(m.stateModified), numAreas(m.numAreas)
{
    ;
}

double AreaSizeRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    if (newState->getState() != stateModified)
        return 1.0;
    
    return computeRateModifier(currState, (size_t)newState->getIndex());
}

double AreaSizeRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, size_t areaIdx)
{
    double mean = 0.0;
    int n = 0;
    
    for (size_t i = 0; i < numAreas; i++)
    {
        // normalize vs characters which may potentially take value==stateModified
        if (currState[i]->getState() != stateModified)
        {
   //         std::cout << i << " ";
            mean += areaPowers[i];
            n++;
        }
    }
    mean /= n;

    /*
    std::cout << "mean " << mean << "\n";

    std::cout << "chk ";
    double chk = 0.0;
    for (size_t i = 0; i < numAreas; i++)
    {
        if (currState[i]->getState() == stateModified)
        {
            std::cout << "\t" << areaPowers[i]/mean << " " << areaPowers[i] << " " << mean << "\n";
            chk += areaPowers[i] / mean;
        }
    }
    std::cout << chk << "\n";
    */
    
    double r = areaPowers[areaIdx] / mean;
    //std::cout << areaIdx << " " << r << " " << power << "\n";
    
    ;
    return r;
}

AreaSizeRateModifier* AreaSizeRateModifier::clone(void) const
{
    return new AreaSizeRateModifier(*this);
}

void AreaSizeRateModifier::update(void)
{
    for (size_t i = 0; i < areaSizes.size(); i++)
    {
        double d = pow(areaSizes[i], power);
        areaPowers[i] = d;
    }
}

void AreaSizeRateModifier::updatePower(double p, bool upd)
{
    power = p;
    if (upd == true)
        update();
}
