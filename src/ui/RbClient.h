
#ifndef RBCLIENT_H
#define	RBCLIENT_H

#include "IHelp.h"
#include "Options.h"
#include "Configuration.h"
#include "EditorMachineObserver.h"

class RbClient : public EditorMachineObserver{
public:
    void startInterpretor(IHelp *help, Options options, Configuration configuration);
    void eventStateChanged();
private:

};

#endif	/* RBCLIENT_H */

