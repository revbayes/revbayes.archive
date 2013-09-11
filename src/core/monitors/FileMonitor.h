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

#include "Monitor.h"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace RevBayesCore {

    class FileMonitor : public Monitor {

    public:
        // Constructors and Destructors
        FileMonitor(DagNode *n, int g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool ci=false, bool ch=false);                                                                //!< Constructor with single DAG node
        FileMonitor(const std::set<DagNode *> &n, int g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool ci=false, bool ch=false);                                              //!< Constructor with set of DAG node
        FileMonitor(const std::vector<DagNode *> &n, int g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool ci=false, bool ch=false);                                              //!< Constructor with vector of DAG node
        FileMonitor(const FileMonitor& f);

        // basic methods
        FileMonitor*                        clone(void) const;                                                  //!< Clone the object
    
        // Monitor functions
        void                                monitor(long gen);                                                  //!< Monitor at generation gen
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
    
    private:
        std::fstream                        outStream;

        // parameters
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        bool                                chainIdx;
        bool                                chainHeat;
    
    };
    
}

#endif

