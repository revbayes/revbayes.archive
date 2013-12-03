//
//  TimeAtlasDataReader.h
//  rb_mlandis
//
//  Created by Michael Landis on 12/3/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TimeAtlasDataReader__
#define __rb_mlandis__TimeAtlasDataReader__

#include "DelimitedDataReader.h"
#include "GeographicArea.h"

#include <vector>

namespace RevBayesCore {
    class TimeAtlasDataReader : public DelimitedDataReader
    {
    public:
        
        TimeAtlasDataReader(std::string fn, char d='\t');
        TimeAtlasDataReader(const TimeAtlasDataReader& tadr);
        
        std::vector<double> getTimes(void);
        std::vector<std::vector<GeographicArea*> > getAreas(void);
        
    protected:
        
        void setTimes(void);
        void setAreas(void);
        
        std::vector<double> times;
        std::vector<std::vector<GeographicArea*> > areas;
        
    };
}

#endif /* defined(__rb_mlandis__TimeAtlasDataReader__) */
