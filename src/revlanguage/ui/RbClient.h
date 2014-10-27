
#ifndef RBCLIENT_H
#define	RBCLIENT_H

#include "EditorMachineObserver.h"
#include <string>

typedef std::vector<std::string> StringVector;

struct tabCompletionInfo{
        int startPos;
        unsigned int specialMatchType;
        StringVector completions;
        StringVector matchingCompletions;
        std::string compMatch;
    };

class RbClient {
public:
    void setTabCompletionInfo(const char *buf);
    
    void startInterpretor(void);
    tabCompletionInfo getTabCompleteInfo(const char *buf);
    
    
private:
    

};

#endif	/* RBCLIENT_H */

