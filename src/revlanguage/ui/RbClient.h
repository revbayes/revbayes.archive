
#ifndef RBCLIENT_H
#define	RBCLIENT_H

#include "IHelp.h"
#include "Options.h"
#include "Configuration.h"
#include "EditorMachineObserver.h"
#include "IRepoObserver.h"
#include <string>

typedef std::vector<std::string> StringVector;

struct tabCompletionInfo{
        int startPos;
        unsigned int specialMatchType;
        StringVector completions;
        StringVector matchingCompletions;
        std::string compMatch;
    };

class RbClient : public IRepoObserver {
public:
    void setTabCompletionInfo(const char *buf);
    
    void startInterpretor(IHelp *help, Options *options, Configuration *configuration);
    tabCompletionInfo getTabCompleteInfo(const char *buf);
    
    // EditorMachineObserver
//    void eventStateChanged(EditorState *state, EditorStateChangeType type);
    
    //IRepoObserver
    void notifyError(std::string error);
    void notifyGetIndexComplete(HttpResponse httpResponse, RepositoryInfo revRepository);
    void notifyGetFileComplete(HttpResponse httpResponse, RepositoryInfo revRepository);
    
    
private:
    

};

#endif	/* RBCLIENT_H */

