#ifndef FileMonitor_H
#define FileMonitor_H

#include "AbstractFileMonitor.h"

#include <string>
#include <vector>

namespace RevBayesCore {

    class FileMonitor : public AbstractFileMonitor {

    public:
        // Constructors and Destructors
        FileMonitor(DagNode *n, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool wv=true);                                                                //!< Constructor with single DAG node
        FileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool wv=true);                                              //!< Constructor with vector of DAG node
        
        // basic methods
        FileMonitor*                        clone(void) const;                                                  //!< Clone the object
        
    private:
        
    };
    
}

#endif

