//
//  GeographicArea.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 11/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "GeographicArea.h"

#include <sstream>

using namespace RevBayesCore;

GeographicArea::GeographicArea(size_t idx, double lat, double lon, std::string n, double alt, std::set<GeographicArea*> aa, std::set<GeographicArea*> ra)
//: index(idx), name(n), latitude(lat), longitude(lon), altitude(alt), adjacentAreas(aa), reachableAreas(ra)
{
    index = idx;
    name = n;
    latitude = lat;
    longitude = lon;
    altitude = alt;
    adjacentAreas = aa;
    reachableAreas = ra;
}

GeographicArea::GeographicArea(const GeographicArea& g)
{
    if (this != &g) {
        index = g.index;
        name = g.name;
        latitude = g.latitude;
        longitude = g.longitude;
        altitude = g.altitude;
        
        std::set<GeographicArea*>::iterator it;
        for (it = adjacentAreas.begin(); it != adjacentAreas.end(); it++)
            (*it)->swapAdjacentAreas(this, const_cast<GeographicArea*>(&g));
        
        for (it = reachableAreas.begin(); it != reachableAreas.end(); it++)
            (*it)->swapReachableAreas(this, const_cast<GeographicArea*>(&g));

    }
        
}

GeographicArea& GeographicArea::operator=(const GeographicArea &g)
{
    if (this != &g) {
        index = g.index;
        name = g.name;
        latitude = g.latitude;
        longitude = g.longitude;
        altitude = g.altitude;
        
        std::set<GeographicArea*>::iterator it;
        for (it = adjacentAreas.begin(); it != adjacentAreas.end(); it++)
            (*it)->swapAdjacentAreas(this, const_cast<GeographicArea*>(&g));
        
        for (it = reachableAreas.begin(); it != reachableAreas.end(); it++)
            (*it)->swapReachableAreas(this, const_cast<GeographicArea*>(&g));

    }
    
    return *this;
}

GeographicArea* GeographicArea::clone(void) const
{
    return new GeographicArea( *this );
}

std::string GeographicArea::getName(void)
{
    return name;
}

size_t GeographicArea::getIndex(void)
{
    return index;
}

double GeographicArea::getLatitude(void)
{
    return latitude;
}

double GeographicArea::getLongitude(void)
{
    return longitude;
}

double GeographicArea::getAltitude(void)
{
    return altitude;
}

void GeographicArea::addAdjacentArea(GeographicArea* a)
{
    adjacentAreas.insert(a);
}

void GeographicArea::removeAdjacentArea(GeographicArea* a)
{
    adjacentAreas.erase(a);
}

const std::set<GeographicArea*>& GeographicArea::getAdjacentAreas(void)
{
    return adjacentAreas;
}

const std::set<GeographicArea*>& GeographicArea::getReachableAreas(void)
{
    return reachableAreas;
}

std::string GeographicArea::getStr(void)
{
    std::stringstream ss;
    ss << latitude << ",";
    ss << longitude << ",";
    ss << altitude;
    for (std::set<GeographicArea*>::iterator it = adjacentAreas.begin(); it != adjacentAreas.end(); it++)
        ss << "," << (*it)->getIndex();
    
    return ss.str();
}

void GeographicArea::swapAdjacentAreas(GeographicArea* oldArea, GeographicArea* newArea)
{
    std::set<GeographicArea*>::iterator it = adjacentAreas.find(oldArea);
    if (it != adjacentAreas.end())
        adjacentAreas.erase(it);
    adjacentAreas.insert(newArea);
}

void GeographicArea::swapReachableAreas(GeographicArea* oldArea, GeographicArea* newArea)
{
    std::set<GeographicArea*>::iterator it = reachableAreas.find(oldArea);
    if (it != reachableAreas.end())
        reachableAreas.erase(it);
    reachableAreas.insert(newArea);
}
