/* 
 * File:   IHelp.h
 * Author: johadunf
 *
 * Created on May 7, 2014, 2:59 PM
 */

#ifndef IHELP_H
#define	IHELP_H


#include <string>
#include "RbHelpEntry.h"

class IHelp {
public:   
    virtual std::string getHelp(std::string query, size_t columnWidth) = 0;
    virtual bool isHelpAvailableForQuery(std::string query) = 0;
    
    void setRbHelpEntry(RbHelpEntry rbHelp) {
        this->rbHelpEntry = rbHelp;
    }

    RbHelpEntry getRbHelpEntry() const {
        return rbHelpEntry;
    }
  
private:
    RbHelpEntry rbHelpEntry;
};

#endif	/* IHELP_H */

