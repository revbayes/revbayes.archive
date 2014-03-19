//
//  GeographicArea.h
//  rb_mlandis
//
//  Created by Michael Landis on 11/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__GeographicArea__
#define __rb_mlandis__GeographicArea__

#include "Cloneable.h"
#include <set>

namespace RevBayesCore {

    class GeographicArea : public Cloneable
    {
    public:
        GeographicArea(size_t idx, double lat, double lon, std::string n="", double alt=1.0, std::set<GeographicArea*> aa=std::set<GeographicArea*>(), std::set<GeographicArea*> ra=std::set<GeographicArea*>());
        GeographicArea(const GeographicArea& g);
        GeographicArea& operator=(const GeographicArea& g);
        virtual GeographicArea* clone(void) const;
        std::string getName(void);
        size_t getIndex(void);
        double getLatitude(void);
        double getLongitude(void);
        double getAltitude(void);
        const std::set<GeographicArea*>& getAdjacentAreas(void);
        const std::set<GeographicArea*>& getReachableAreas(void);
        void addAdjacentArea(GeographicArea* a);
        void removeAdjacentArea(GeographicArea* a);
        void swapAdjacentAreas(GeographicArea* oldArea, GeographicArea* newArea);
        void swapReachableAreas(GeographicArea* oldArea, GeographicArea* newArea);
        std::string getStr(void);
        
    protected:
        size_t index;
        std::string name;
        double latitude;
        double longitude;
        double altitude;
        std::set<GeographicArea*> adjacentAreas;
        std::set<GeographicArea*> reachableAreas;
    };

}

#endif /* defined(__rb_mlandis__GeographicArea__) */
