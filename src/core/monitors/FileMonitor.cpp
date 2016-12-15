#include "FileMonitor.h"
#include "DagNode.h"
#include "Monitor.h"
#include "RbFileManager.h"

using namespace RevBayesCore;

/* Constructor */
FileMonitor::FileMonitor(DagNode *n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap, bool wv) : AbstractFileMonitor(n,g,fname,del,pp,l,pr,ap,wv)
{
    
}


/* Constructor */
FileMonitor::FileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap, bool wv) : AbstractFileMonitor(n,g,fname,del,pp,l,pr,ap,wv)
{
    
}


/* Clone the object */
FileMonitor* FileMonitor::clone(void) const
{

    return new FileMonitor(*this);
}


