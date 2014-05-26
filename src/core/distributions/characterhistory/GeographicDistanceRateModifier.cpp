//
//  GeographicDistanceRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <iomanip>
#include <cmath>
#include "CharacterEvent.h"
#include "GeographicArea.h"
#include "GeographicDistanceRateModifier.h"
#include "RbConstants.h"
#include "TimeAtlas.h"
#define EARTHRADIUSKM 6371

using namespace RevBayesCore;

GeographicDistanceRateModifier::GeographicDistanceRateModifier(TimeAtlas* ta, int idx, double dp, double th, std::string dt )

{
    // initialize
    atlas = ta;
    index = idx;
    distancePower = dp;
    threshhold = th;
    distanceType = dt;

    
    const std::vector<std::vector<GeographicArea*> > tmpAreas = atlas->getAreas();
    
    
    numAreas = 0;
    epochs = atlas->getEpochs();
    numEpochs = epochs.size();
    if (numEpochs > 0)
        numAreas = tmpAreas[0].size();
    
    epochOffset = numAreas * numAreas;
    areaOffset = numAreas;
    
    
    // assign areas for timeslice
    areas.resize(numEpochs);
    for (size_t i = 0; i < numEpochs; i++)
    {
        for (size_t j = 0; j < numAreas; j++)
        {
            GeographicArea* g = new GeographicArea( *tmpAreas[i][j] );
            areas[i].push_back(g);
            
            geographicCoordinates.push_back(g->getLatitude());
            geographicCoordinates.push_back(g->getLongitude());
        }
    }


    geographicDistances.resize(numEpochs * numAreas * numAreas, 0.0);
    geographicDistancePowers.resize(numEpochs * numAreas * numAreas, 0.0);
    
    for (unsigned i = 0; i < numEpochs; i++)
    {
        unsigned iOffset = i*epochOffset;
        for (unsigned j = 0; j < numAreas; j++)
        {
            unsigned jOffset = j*areaOffset;
            for (unsigned k = j; k < numAreas; k++)
            {
//                std::cout << "geoDist " << epochOffset*i + areaOffset*j + k << "\n";
//                unsigned l = epochOffset*i + areaOffset*j + k;
                
                geographicDistances[epochOffset*i + areaOffset*j + k] = computePairwiseDistances(i, j, k);
                geographicDistances[epochOffset*i + areaOffset*k + j] = geographicDistances[epochOffset*i + areaOffset*j + k];
            }
        }
    }
    
//    std::cout << "\n";
//    for (size_t i = 0; i < numEpochs; i++)
//    {
//        unsigned iOffset = i*epochOffset;
//        for (size_t j = 0; j < numAreas; j++)
//        {
//            unsigned jOffset = j*areaOffset;
//            for (size_t k = 0; k < numAreas; k++)
//            {
//                double d = geographicDistances[ iOffset + jOffset + k ];
//                std::cout << std::fixed << std::setprecision(3) << std::setw(4);
//                std::cout <<  d << " ";
//            }
//            std::cout << "\n";
//        }
//        std::cout << "----\n";
//        
//    }
    
    //computeAllPairwiseDistanceOrder();
    
    setDistancePower(dp, true);
    update();
}

GeographicDistanceRateModifier::GeographicDistanceRateModifier(const GeographicDistanceRateModifier& g)
{
    
    if (&g != this)
    {
        atlas = g.atlas;
        index = g.index;
        epochs = g.epochs;
        areas = g.areas;
        
        distanceType = g.distanceType;
        geographicCoordinates = g.geographicCoordinates;
        geographicDistances = g.geographicDistances;
        geographicDistancePowers = g.geographicDistancePowers;
        geographicDistanceOrder = g.geographicDistanceOrder;
        
        numAreas = g.numAreas;
        numEpochs = g.numEpochs;
        areaOffset = g.areaOffset;
        epochOffset = g.epochOffset;
        threshhold = g.threshhold;
        distancePower = g.distancePower;
        
        computeAllPairwiseDistanceOrder();
        setDistancePower(1.0, true);
        update();
    }
}

double GeographicDistanceRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    
    if (newState->getState() == 0)
        return 1.0;
    
    unsigned epochIndex = getEpochIndex(age);
    
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
    std::set<CharacterEvent*>::iterator it_p;
    std::set<CharacterEvent*>::iterator it_a;
    for (it_p = present.begin(); it_p != present.end(); it_p++)
    {
        size_t idx_p = (*it_p)->getIndex();
        
        for (it_a = absent.begin(); it_a != absent.end(); it_a++)
        {
            size_t idx_a = (*it_a)->getIndex();

//            double d = geographicDistancePowers[idx_p][idx_a];
            double d = geographicDistancePowers[ epochIndex*epochOffset + idx_p*areaOffset + idx_a  ];
            
            sum += d;
            
            if (idx_a == newState->getIndex())
                rate += d;
        }
    }
    
    // get sum-normalized rate-modifier
    double r = absent.size() * rate / sum;
    
    //std::cout << "drm   " << r << " = " << absent.size() << " * " << rate << " / " << sum <<  "\n";
    
    return r;
}

double GeographicDistanceRateModifier::computeRateModifier(const TopologyNode& node, std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    return computeRateModifier(currState, newState, age);
}

double GeographicDistanceRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    return computeRateModifier(currState, newState, 0.0);
}


GeographicDistanceRateModifier* GeographicDistanceRateModifier::clone(void) const
{
    return new GeographicDistanceRateModifier(*this);
}



void GeographicDistanceRateModifier::update(void)
{
    for (size_t i = 0; i < numEpochs; i++)
    {
        unsigned iOffset = i*epochOffset;
        for (size_t j = 0; j < numAreas; j++)
        {
            unsigned jOffset = j*areaOffset;
            for (size_t k = 0; k < j; k++)
            {
                //double d = exp(-distancePower * geographicDistances[ iOffset + jOffset + k ]); //, -distancePower);
                double d = pow(geographicDistances[ iOffset + jOffset + k ], -distancePower);

                geographicDistancePowers[ iOffset + jOffset + k ] = d;
                geographicDistancePowers[ iOffset + k*areaOffset + j ] = d;
            }
        }
    }
    
//    for (size_t i = 0; i < numEpochs; i++)
//    {
//        unsigned iOffset = i*epochOffset;
//        for (size_t j = 0; j < numAreas; j++)
//        {
//            unsigned jOffset = j*areaOffset;
//            for (size_t k = 0; k < numAreas; k++)
//            {
//                double d = geographicDistancePowers[ iOffset + jOffset + k ];
//                std::cout << std::fixed << std::setw(4);
//                std::cout <<  d << " ";
//            }
//            std::cout << "\n";
//        }
//        std::cout << "----\n";
//        
//    }
    
}


void GeographicDistanceRateModifier::setDistancePower(double dp, bool upd)
{
    
    bool changed = (distancePower != dp);
    
    //std::cout << "setDistancePower " << dp << "\n";
    distancePower = dp;
    if (upd && changed)
        update();
}

void GeographicDistanceRateModifier::setGeographicDistancePowers(const std::vector<double>& dp)
{
    geographicDistancePowers = dp;
}

const std::vector<double>& GeographicDistanceRateModifier::getGeographicDistancePowers(void) const
{
    return geographicDistancePowers;
}

unsigned GeographicDistanceRateModifier::getEpochIndex(double age)
{
    unsigned index = 0;
    while (age <= epochs[index] && index < numEpochs)
    {
        //std::cout << age << " " <<  epochs[index] << " " << index << "\n";
        index++;
    };
    return index;
}

double GeographicDistanceRateModifier::computePairwiseDistances(int i, int j, int k)
{
    double d = 0.0;
    double rad = RbConstants::PI/180;
    
//    double lat0 = geographicCoordinates[i][0] * rad;
//    double lat1 = geographicCoordinates[j][0] * rad;
//    double lon0 = geographicCoordinates[i][1] * rad;
//    double lon1 = geographicCoordinates[j][1] * rad;

//    std::cout << j << " : " << geographicCoordinates[i*epochOffset + j*2 + 0] << ", " << geographicCoordinates[i*epochOffset + j*2 + 0] << "\n";
//    std::cout << k << " : " << geographicCoordinates[i*epochOffset + k*2 + 0] << ", " << geographicCoordinates[i*epochOffset + k*2 + 0] << "\n";
    unsigned idx0 = i*numAreas*2 + j*2;
    unsigned idx1 = i*numAreas*2 + k*2;
    //std::cout << i << " " << j << " " << k << " " << idx0 << " " << idx1 << "\n";
    
    double lat0 = geographicCoordinates[ idx0 + 0] * rad;
    double lat1 = geographicCoordinates[ idx1 + 0] * rad;
    double lon0 = geographicCoordinates[ idx0 + 1] * rad;
    double lon1 = geographicCoordinates[ idx1 + 1] * rad;

    
    if (distanceType == "haversine")
    {
        //double d = 0.0;
        double dlat = lat0 - lat1;
        double dlon = lon0 - lon1;
        double sindlat = sin(dlat/2);
        double sindlon = sin(dlon/2);
        double a = sqrt(sindlat * sindlat + cos(lat0) * cos(lat1) * sindlon * sindlon);
        if (a < 0.9995)
            d = 2 * asin(a) * EARTHRADIUSKM;
        else
            d = 2 * asin(1.0) * EARTHRADIUSKM;
    }
    else if (distanceType == "euclidean")
    {
        double dlat = lat0 - lat1;
        double dlon = lon0 - lon1;
        d = pow(dlat*dlat+dlon*dlon, 0.5);
    }
    
    return d;
}

void GeographicDistanceRateModifier::computeAllPairwiseDistanceOrder(void)
{
//    //std::vector<std::vector<size_t> > geographicDistanceOrder;
//    //double minDist = 0;
//    
//    geographicDistanceOrder.resize(numEpochs*numAreas);
//    for (size_t i = 0; i < numAreas; i++)
//    {
//        //orderedDistanceIdx[i].resize(numAreas,0);
//        for (size_t j = 0; j < numAreas; j++)
//        {
//            double d = geographicDistances[i][j];
//            
//            size_t k = 0;
//            for (k = 0; k < geographicDistanceOrder[i].size(); k++)
//            {
//                if (d < geographicDistances[i][geographicDistanceOrder[i][k]])
//                    break;
//            }
//            std::vector<size_t>::iterator it = geographicDistanceOrder[i].begin();
//            geographicDistanceOrder[i].insert(it + k, j);
//        }
//    }
}

void GeographicDistanceRateModifier::print(std::vector<std::vector<double> > m)
{
//    std::vector<std::vector<double> >::iterator it0;
//    std::vector<double>::iterator it1;
//    
//    for (it0 = m.begin(); it0 != m.end(); it0++)
//    {
//        for (it1 = it0->begin(); it1 != it0->end(); it1++)
//        {
//            std::cout << *it1 << " ";
//        }
//        std::cout << "\n";
//    }
}

void GeographicDistanceRateModifier::printAll(void)
{
//    std::cout << "\ngc\n";
//    print(geographicCoordinates);
//        std::cout << "\ngd\n";
//    print(geographicDistances);
//        std::cout << "\ngdp\n";
//    print(geographicDistancePowers);
//    
}
