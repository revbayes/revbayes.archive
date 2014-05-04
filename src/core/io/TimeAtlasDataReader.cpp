//
//  TimeAtlasDataReader.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 12/3/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TimeAtlasDataReader.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <iostream>
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
     
     // maybe something like this??
     
     {{
     name:'epoch1',
     start_age:55,
     end_age:20,
     areas:
     [
     { name:'area0', index:0, latitude:-20, longitude:20, altitude:1000, adjacent:[1,2], size:200 },
     { name:'area1', index:1, latitude:-30, longitude:30, altitude:2000, adjacent:[0,2], size:300 },
     { name:'area2', index:2, latitude:-40, longitude:40, altitude:3000, adjacent:[0,1], size:400 }
     ]
     },
     {
     name:'epoch2',
     start_age:20,
     end_age:0,
     areas:
     [
     { name:'area0', index:0, latitude:-20, longitude:20, altitude:1000, adjacent:[2], size:200 },
     { name:'area1', index:1, latitude:-30, longitude:30, altitude:1500, adjacent:[2], size:200 },
     { name:'area2', index:2, latitude:-40, longitude:40, altitude:2000, adjacent:[0,1], size:200 }
     ]
     }};
     */
    
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

void TimeAtlasDataReader::readJson(void)
{
    std::stringstream ss;
    //ss << "{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }";
    ss << "{ \"maps\": { \"array\": [ { \"values\": [ \"val\":1, \"val\":2, \"val\":3, \"val\":4, \"val\":5 ], \"name\":\"what\" }, { \"values\":[ \"val\":2,\"val\":3,\"val\":4], \"name\":\"yeah\" } ] } }";
    std::cout << ss.str() << "\n";
    
    
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);
   // boost::property_tree::read_json(this->filename, pt);

//    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("maps.array"))
//    {
//        const boost::property_tree::ptree &pt2 = v.second;
//        BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, pt2.get_child("values"))
//        {
//            const boost::property_tree::ptree &pt3 = v2.second;
//            std::string s = pt3.get<std::string>("");
//        
//            std::cout << s << "\n";
//        }
//    }

//    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("maps.array"))
//    {
//        std::cout << "b\n";
//        
////        unsigned long uiPlaylistId = playlist.second.get<unsigned long>("id");
//        BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.second.get_child("values."))
//        {
//            std::cout << "a\n";
//            std::string strVal = v2.second.get<std::string>("val");
//            
//            std::cout << strVal << "\n";
//        }
//        
//        std::cout << "\n";
//    }
    
//    std::cout << ss.str() << "\n";
    
    ;
}

std::vector<double> TimeAtlasDataReader::getTimes(void)
{
    return times;
}

std::vector<std::vector<GeographicArea*> > TimeAtlasDataReader::getAreas(void)
{
    return areas;
}
