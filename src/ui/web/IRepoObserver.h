/* 
 * File:   IRepoObserver.h
 * Author: johadunf
 *
 * Created on May 21, 2014, 7:57 PM
 */

#ifndef IREPOOBSERVER_H
#define	IREPOOBSERVER_H

#include <string>
#include <vector>

#include "RepositoryInfo.h"
#include "HttpClient.h"

typedef std::vector<std::string> StringVector;

class IRepoObserver{
public:
    // basic
    virtual void notifyError(std::string error) = 0;
    
    // repo operations
    virtual void notifyGetIndexComplete(HttpResponse httpResponse, RepositoryInfo revRepository) = 0;
    virtual void notifyGetFileComplete(HttpResponse httpResponse, RepositoryInfo revRepository) = 0;
};

#endif	/* IREPOOBSERVER_H */

