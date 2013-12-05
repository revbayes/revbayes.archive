//
//  TimeAtlas.h
//  rb_mlandis
//
//  Created by Michael Landis on 12/3/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TimeAtlas__
#define __rb_mlandis__TimeAtlas__

#include "Cloneable.h"
#include "GeographicArea.h"
#include "TimeAtlasDataReader.h"

namespace RevBayesCore {
    class TimeAtlas : public Cloneable
    {
        
    public:
        TimeAtlas(TimeAtlasDataReader* tadr);
        TimeAtlas(const TimeAtlas& a);
        TimeAtlas& operator=(const TimeAtlas& a);
        virtual TimeAtlas* clone(void) const;
        
        std::vector<double> getTimes(void);
        std::vector<std::vector<GeographicArea*> > getAreas(void);
        
    protected:
        
        std::vector<std::vector<GeographicArea*> > areas;
        std::vector<double> times;
        
    };
}


#endif /* defined(__rb_mlandis__TimeAtlas__) */
