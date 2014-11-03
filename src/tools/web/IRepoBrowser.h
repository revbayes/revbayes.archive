/* 
 * File:   RepoBrowser.h
 * Author: johadunf
 *
 * Created on May 20, 2014, 6:31 AM
 */

#ifndef REPOBROWSER_H
#define	REPOBROWSER_H

#include <string>
#include <vector>

#include "HttpClient.h"
#include "RepositoryInfo.h"

typedef std::vector<std::string> StringVector;

class IRepoBrowser {
    
public:
    
    virtual HttpResponse getIndex() = 0;
    virtual HttpResponse getFile(std::string filename) = 0;

};

#endif	/* REPOBROWSER_H */

