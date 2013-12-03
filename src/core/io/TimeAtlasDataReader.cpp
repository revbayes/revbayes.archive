//
//  TimeAtlasDataReader.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 12/3/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TimeAtlasDataReader.h"

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <set>

using namespace RevBayesCore;

TimeAtlasDataReader::TimeAtlasDataReader(std::string fn, char d) : DelimitedDataReader(fn, d)
{
    
    setTimes();
    setAreas();
}

TimeAtlasDataReader::TimeAtlasDataReader(const TimeAtlasDataReader& tadr) : DelimitedDataReader(tadr)
{
    ;
}

void TimeAtlasDataReader::setTimes(void)
{
    for (size_t i = 0; i < chars.size(); i++)
    {
        std::string s = chars[i][0];
        times.push_back(std::strtod(s.c_str(),0));
    }
}

void TimeAtlasDataReader::setAreas(void)
{
    // lat,lon,%_land,adj_1,...,adj_n
    
    std::vector<std::set<int> > tmpAdjAreas;
    std::vector<int> tmpLandAreas;
    std::vector<double> tmpLat, tmpLon;
    
    std::vector<GeographicArea*> tmpAreas;
    
    for (size_t i = 0; i < chars.size(); i++)
    {
        for (size_t j = 1; j < chars[i].size(); j++)
        {
            std::set<int> tmpAdj;
            
            int pos = 0;
            double land = 0.0;
            double lat = 0.0;
            double lon = 0.0;

            std::stringstream ss(chars[i][j]);
            std::string field;
    
            while (std::getline(ss,field,','))
            {
                if (pos == 0)
                    lat = std::strtod(field.c_str(),0);
                else if (pos == 1)
                    lon = std::strtod(field.c_str(),0);
                else if (pos == 2)
                    land = std::strtod(field.c_str(),0);
                else if (pos > 2 && field != "")
                    tmpAdj.insert(std::strtod(field.c_str(),0));
                pos++;
            }
            
            GeographicArea* g  = new GeographicArea(j-1, lat, lon, "", land);
            tmpAreas.push_back(g);
            tmpAdjAreas.push_back(tmpAdj);
        }
        
        // update adjacencies
        for (size_t j = 0; j < tmpAreas.size(); j++)
        {
            for (std::set<int>::iterator it = tmpAdjAreas[j].begin(); it != tmpAdjAreas[j].end(); it++)
            {
                tmpAreas[j]->addAdjacentArea(tmpAreas[*it]);
            }
        }
        
        areas.push_back(tmpAreas);
        tmpAdjAreas.clear();
        tmpAreas.clear();
    }
    
    /*
    for (size_t i = 0; i < areas.size(); i++)
    {
        for (size_t j = 0; j < areas[i].size(); j++)
        {
            std::cout << areas[i][j]->getStr();
            std::cout << "\t";
        }
        std::cout << "\n";
    }
    */
}

std::vector<double> TimeAtlasDataReader::getTimes(void)
{
    return times;
}

std::vector<std::vector<GeographicArea*> > TimeAtlasDataReader::getAreas(void)
{
    return areas;
}
