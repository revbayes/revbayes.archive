//
//  GeographicGridRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 11/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <cmath>
#include "CharacterEvent.h"
#include "GeographicArea.h"
#include "GeographicGridRateModifier.h"
#include "RbConstants.h"

#define EARTHRADIUSKM 6371

using namespace RevBayesCore;

GeographicGridRateModifier::GeographicGridRateModifier(TimeAtlas* ta, int idx) : atlas(ta), index(idx)
{
    // assign areas for timeslice
    gridAreas.clear();
    std::vector<GeographicArea*> tmpAreas = atlas->getAreas()[index];
    for (size_t i = 0; i < tmpAreas.size(); i++)
        gridAreas.push_back(new GeographicArea(*tmpAreas[i]));

}

GeographicGridRateModifier::GeographicGridRateModifier(const GeographicGridRateModifier& p)
{
    if (&p != this)
    {
        numAreas = p.numAreas;
        index = p.index;
        *atlas = *p.atlas;
        gridAreas.clear();
        for (size_t i = 0; i < p.gridAreas.size(); i++)
            gridAreas.push_back(new GeographicArea(*p.gridAreas[i]));
    }
}


GeographicGridRateModifier* GeographicGridRateModifier::clone(void) const
{
    return new GeographicGridRateModifier(*this);
}


void GeographicGridRateModifier::update(void)
{
    ;
}

double GeographicGridRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    
    // determine which areas are present and which are absent
    std::set<CharacterEvent*> present;
    std::set<CharacterEvent*> absent;
    for (unsigned i = 0; i < numAreas; i++)
    {
        if (currState[i]->getState() == 0)
            absent.insert(currState[i]);
        else
            present.insert(currState[i]);
    }
    
    // get sum of distances_ij^beta
    double rate = 0.0;
    double sum = 0.0;
    std::set<GeographicArea*>::iterator it_adj;
    
    std::set<CharacterEvent*>::iterator it_p;
    std::set<CharacterEvent*>::iterator it_a;
    for (it_p = present.begin(); it_p != present.end(); it_p++)
    {
        size_t idx_p = (*it_p)->getIndex();
    
        const std::set<GeographicArea*> p_adj = gridAreas[idx_p]->getAdjacentAreas();
        for (it_adj = p_adj.begin(); it_adj != p_adj.end(); it_adj++)
        {
            size_t adj_idx = (*it_adj)->getIndex();
            if (currState[adj_idx]->getState() == 0)
                sum += 1.0;
            if (adj_idx == newState->getIndex())
                rate += 1.0;
        }
    }
    
    // get sum-normalized rate-modifier
    double r = absent.size() * rate / sum;
    
    return r;
}
