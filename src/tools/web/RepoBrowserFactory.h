/* 
 * File:   RepoBrowserFactory.h
 * Author: johadunf
 *
 * Created on May 20, 2014, 7:47 PM
 */

#ifndef REPOBROWSERFACTORY_H
#define	REPOBROWSERFACTORY_H

#include "SourceForgeRepoBrowser.h"
#include "IRepoBrowser.h"
#include "RepositoryInfo.h"

class RepoBrowserFactory{
public:
    static IRepoBrowser *getRepoBrowser(RepositoryInfo revRepository){
        if(revRepository.GetType() == "SourceForge"){
            return new SourceForgeRepoBrowser(revRepository);
        }
        
        return NULL;
    }
};

#endif	/* REPOBROWSERFACTORY_H */

