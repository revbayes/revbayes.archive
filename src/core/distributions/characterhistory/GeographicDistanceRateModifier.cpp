//
//  GeographicDistanceRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <cmath>
#include "CharacterEVent.h"
#include "GeographicDistanceRateModifier.h"
#include "RbConstants.h"
#define EARTHRADIUSKM 6371

using namespace RevBayesCore;

GeographicDistanceRateModifier::GeographicDistanceRateModifier(void)
{

    geographicCoordinates.resize(numAreas);
    for (size_t i = 0; i < numAreas; i++)
    {
        // get these somehow
        double latitude = 0.0;
        double longitude = 0.0;
        
        geographicCoordinates[i].push_back(latitude);
        geographicCoordinates[i].push_back(longitude);
    }
}

double GeographicDistanceRateModifier::computeRateModifier(std::vector<CharacterEvent*> curState, std::set<CharacterEvent*> newState)
{
    
}

double GeographicDistanceRateModifier::computePairwiseDistances(int i, int j)
{
    double d = 0.0;
    double lat0 = geographicCoordinates[i][0];
    double lat1 = geographicCoordinates[j][0];
    double lon0 = geographicCoordinates[i][1];
    double lon1 = geographicCoordinates[j][1];
    
    // check for International Time Line
    if (lon0 - lon1 > 180)
        lon0 -= 180;
    else if (lon1 - lon0 > 180)
        lon1 -= 180;
    
    if (distanceType == "haversine")
    {
        double r = 0.0;
        
        // MJL 090913: wrong??
        double dlat = fabs(lat0 - lat1);
        double dlon = fabs(lon0 - lon1);
        if (dlon > 180)
            dlon -= 180;
        //else if (dlon < -180)
        //    dlon += 180;
        
        dlat *= RbConstants::PI/180;
        dlon *= RbConstants::PI/180;
        
        double sindlat = sin(dlat/2);
        double sindlon = sin(dlon/2);
        double a = sqrt(sindlat * sindlat + cos(lat0*RbConstants::PI/180) * cos(lat1*RbConstants::PI/180) * sindlon * sindlon);
        if (a < 1.0)
            r = 2 * asin(a) * EARTHRADIUSKM;
        else
            r = 2 * asin(1.0) * EARTHRADIUSKM;

    }
    else if (distanceType == "euclidean")
    {
        double dlat = lat0 - lat1;
        double dlon = lon0 - lon1;
        d = pow(dlat*dlat+dlon*dlon, 0.5);
    }
}

std::string distanceType;
std::vector<std::vector<double> > geographicCoordinates;
std::vector<std::vector<double> > geographicDistances;

