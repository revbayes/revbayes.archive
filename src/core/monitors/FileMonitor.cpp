#include "FileMonitor.h"
#include "DagNode.h"
#include "Monitor.h"
#include "RbFileManager.h"

using namespace RevBayesCore;

/* Constructor */
FileMonitor::FileMonitor(DagNode *n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : AbstractFileMonitor(n,g,fname,del,pp,l,pr,ap)
{
    
}


/* Constructor */
FileMonitor::FileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : AbstractFileMonitor(n,g,fname,del,pp,l,pr,ap)
{
    
}


/* Clone the object */
FileMonitor* FileMonitor::clone(void) const
{

    return new FileMonitor(*this);
}


