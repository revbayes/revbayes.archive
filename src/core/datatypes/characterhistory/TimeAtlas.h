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
        
        std::vector<double> getEpochs(void);
        std::vector<std::vector<GeographicArea*> > getAreas(void);
        
    protected:
        std::vector<std::vector<GeographicArea*> > areas;
        std::vector<double> epochs;
        
    private:
        unsigned numAreas;
        unsigned numEpochs;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const TimeAtlas& x);
}


#endif /* defined(__rb_mlandis__TimeAtlas__) */
