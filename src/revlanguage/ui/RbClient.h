
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

class RbClient : public EditorMachineObserver {
public:
    void setTabCompletionInfo(const char *buf);
    
    void startInterpretor();
    tabCompletionInfo getTabCompleteInfo(const char *buf);
    
    // EditorMachineObserver
    void eventStateChanged(EditorState *state, EditorStateChangeType type);
    
private:
    

};

#endif	/* RBCLIENT_H */

