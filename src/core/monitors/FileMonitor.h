/**
 * @file
 * This file contains the declaration of Monitor, used to save information
 * to a file about the monitoring of a variable DAG node.
 *
 * @brief Declaration of Monitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef FileMonitor_H
#define FileMonitor_H

#include "AbstractFileMonitor.h"

#include <string>
#include <vector>

namespace RevBayesCore {

    class FileMonitor : public AbstractFileMonitor {

    public:
        // Constructors and Destructors
        FileMonitor(DagNode *n, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false);                                                                //!< Constructor with single DAG node
        FileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false);                                              //!< Constructor with vector of DAG node
        
        // basic methods
        FileMonitor*                        clone(void) const;                                                  //!< Clone the object
        
    private:
        
    };
    
}

#endif

