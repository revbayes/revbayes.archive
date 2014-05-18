//
//  TimeAtlasDataReader.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 12/3/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TimeAtlasDataReader.h"
#include "RbFileManager.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "RbConstants.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <set>
#include <string>

using namespace RevBayesCore;

TimeAtlasDataReader::TimeAtlasDataReader(std::string fn, char d) : DelimitedDataReader(fn, d)
{
    readJson();
    //setTimes();
    //setAreas();
}

TimeAtlasDataReader::TimeAtlasDataReader(const TimeAtlasDataReader& tadr) : DelimitedDataReader(tadr)
{
    ;
}

//void TimeAtlasDataReader::setTimes(void)
//{
//    for (size_t i = 0; i < chars.size(); i++)
//    {
//        std::string s = chars[i][0];
//        times.push_back(std::strtod(s.c_str(),0));
//    }
//}
//
//void TimeAtlasDataReader::setAreas(void)
//{
//    // lat,lon,%_land,adj_1,...,adj_n
//    
//    std::vector<std::set<int> > tmpAdjAreas;
//    std::vector<int> tmpLandAreas;
//    std::vector<double> tmpLat, tmpLon;
//    
//    std::vector<GeographicArea*> tmpAreas;
//    
//    for (size_t i = 0; i < chars.size(); i++)
//    {
//        for (size_t j = 1; j < chars[i].size(); j++)
//        {
//            std::set<int> tmpAdj;
//            
//            int pos = 0;
//            double land = 0.0;
//            double lat = 0.0;
//            double lon = 0.0;
//
//            std::stringstream ss(chars[i][j]);
//            std::string field;
//    
//            while (std::getline(ss,field,','))
//            {
//                if (pos == 0)
//                    lat = std::strtod(field.c_str(),0);
//                else if (pos == 1)
//                    lon = std::strtod(field.c_str(),0);
//                else if (pos == 2)
//                    land = std::strtod(field.c_str(),0);
//                else if (pos > 2 && field != "")
//                    tmpAdj.insert(std::strtod(field.c_str(),0));
//                pos++;
//            }
//            
//            GeographicArea* g  = new GeographicArea(j-1, lat, lon, "", land);
//            tmpAreas.push_back(g);
//            tmpAdjAreas.push_back(tmpAdj);
//        }
//        
//        // update adjacencies
//        for (size_t j = 0; j < tmpAreas.size(); j++)
//        {
//            for (std::set<int>::iterator it = tmpAdjAreas[j].begin(); it != tmpAdjAreas[j].end(); it++)
//            {
//                tmpAreas[j]->addAdjacentArea(tmpAreas[*it]);
//            }
//        }
//        
//        areas.push_back(tmpAreas);
//        tmpAdjAreas.clear();
//        tmpAreas.clear();
//    }
//}

void TimeAtlasDataReader::readJson(void)
{
//    std::stringstream ss;
//    ss << "{\"money\":\"billz\"}";
    std::ifstream readStream;
    RbFileManager* f = new RbFileManager(this->filename);
    if (!f->openFile(readStream))
        std::cout << "ERROR: Could not open file " << this->filename << "\n";

    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(readStream, pt);
        
        unsigned epochIndex = 0;
        BOOST_FOREACH(boost::property_tree::ptree::value_type &node, pt.get_child("epochs"))
        {
            std::vector<GeographicArea*> tmpAreas;
            
            std::string epochName = "";
            if (node.second.find("name") != node.second.not_found())
                epochName = node.second.get<std::string>("name");
            
            double epochStartAge = RbConstants::Double::neginf;
            if (node.second.find("start_age") != node.second.not_found())
                epochStartAge = node.second.get<double>("start_age");
            
            double epochEndAge = 0.0;
            if (node.second.find("end_age") != node.second.not_found())
                epochEndAge = node.second.get<double>("end_age");
            
            
            unsigned areaIndex = 0;
            boost::property_tree::ptree pt_areas = node.second;
            BOOST_FOREACH(boost::property_tree::ptree::value_type &node_areas, pt_areas.get_child("areas"))
            {
            
                std::string areaName = "";
                if (node.second.find("altitude") != node.second.not_found())
                    areaName = node_areas.second.get<std::string>("name");
                
                double areaLatitude = 0.0;
                if (node.second.find("latitude") != node.second.not_found())
                    areaLatitude = node_areas.second.get<double>("latitude");
                
                double areaLongitude = 0.0;
                if (node.second.find("longitude") != node.second.not_found())
                    areaLongitude =  node_areas.second.get<double>("longitude");
                
                double areaAltitude = 0.0;
                if (node.second.find("altitude") != node.second.not_found())
                    areaAltitude = node_areas.second.get<double>("altitude");
                
                unsigned areaState = 0;
                if (node.second.find("state") != node.second.not_found())
                    areaState = node_areas.second.get<unsigned>("state");
                
                GeographicArea* g  = new GeographicArea(areaIndex, areaLatitude, areaLongitude, areaName, areaState);
                areaIndex++;
                tmpAreas.push_back(g);
            }
            
            epochIndex++;
            areas.push_back(tmpAreas);
            times.push_back(epochEndAge);
        }
        sortEpochs();

        
        //fillData(pt);
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void TimeAtlasDataReader::sortEpochs(void)
{
    std::vector<AgeIndexPair> v;
    for (size_t i = 0; i < times.size(); i++)
        v.push_back(AgeIndexPair(times[i],i));
    
    std::sort(v.begin(), v.end(), AgeCompare());
    
    std::vector<double> tmpTimes = times;
    std::vector<std::vector<GeographicArea*> > tmpAreas = areas;
    for (size_t i = 0; i < v.size(); i++)
    {
        areas[i] = tmpAreas[v[i].index];
        times[i] = tmpTimes[v[i].index];
        std::cout << times[i] << "\n";
    }

}


void TimeAtlasDataReader::printJson(boost::property_tree::ptree const& pt)
{
    using boost::property_tree::ptree;
    ptree::const_iterator end = pt.end();
    for (ptree::const_iterator it = pt.begin(); it != end; ++it)
    {
        std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
        printJson(it->second);
    }
}

std::vector<double> TimeAtlasDataReader::getTimes(void)
{
    return times;
}

std::vector<std::vector<GeographicArea*> > TimeAtlasDataReader::getAreas(void)
{
    return areas;
}
