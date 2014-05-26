
#ifndef RBCLIENT_H
#define	RBCLIENT_H

#include "IHelp.h"
#include "Options.h"
#include "Configuration.h"
#include "EditorMachineObserver.h"
#include "IRepoObserver.h"

typedef std::vector<std::string> StringVector;

class RbClient : public EditorMachineObserver, public IRepoObserver {
public:
    void startInterpretor(IHelp *help, Options *options, Configuration *configuration);
    
    // EditorMachineObserver
    void eventStateChanged(EditorState *state, EditorStateChangeType type);
    
    //IRepoObserver
    void notifyError(std::string error);
    void notifyGetIndexComplete(HttpResponse httpResponse, RepositoryInfo revRepository);
    void notifyGetFileComplete(HttpResponse httpResponse, RepositoryInfo revRepository);
private:

};

#endif	/* RBCLIENT_H */

