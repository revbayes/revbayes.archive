/* 
 * File:   IHelp.h
 * Author: johadunf
 *
 * Created on May 7, 2014, 2:59 PM
 */

#ifndef IHELP_H
#define	IHELP_H

class IHelp {
public:
    IHelp();
    IHelp(std::string helpDir){
        this->helpDir = helpDir;
    }
    virtual std::string getHelp(std::string query, int columnWidth) = 0;
    
    void setHelpDir(std::string helpDir){
        this->helpDir = helpDir;
    }
    
private:
    std::string helpDir;
};

#endif	/* IHELP_H */

