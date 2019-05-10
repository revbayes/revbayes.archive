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

#ifndef AbstractFileMonitor_H
#define AbstractFileMonitor_H

#include "Monitor.h"
#include "MonteCarloAnalysisOptions.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class AbstractFileMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        AbstractFileMonitor(DagNode *n, unsigned long g, const std::string &fname, bool ap=false, bool wv=true);                                                                //!< Constructor with single DAG node
        AbstractFileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, bool ap=false, bool wv=true);                                              //!< Constructor with vector of DAG node
        AbstractFileMonitor(const AbstractFileMonitor& f);
        
        virtual ~AbstractFileMonitor(void);
        
        // basic methods
        virtual AbstractFileMonitor*        clone(void) const = 0;                                              //!< Clone the object
        
        // Monitor functions
        void                                addFileExtension(const std::string &s, bool dir);
        virtual void                        monitor(unsigned long gen) = 0;                                                             //!< Monitor at generation gen
        virtual void                        printHeader(void) = 0;                                                                      //!< Print header
        
        // FileMonitor functions
        bool                                isFileMonitor( void ) const;
        void                                openStream(bool reopen);                                                                //!< Open the stream for writing
        void                                setAppend(bool tf);                                                                     //!< Set if the monitor should append to an existing file                                                               //!< Set flag whether to print the prior probability
        void                                setPrintVersion(bool tf);                                                               //!< Set flag whether to print the version

        // functions you may want to overwrite
        virtual void                        closeStream(void);                                                                      //!< Close stream after finish writing
    
    protected:
        std::fstream                        out_stream;
        
        // parameters
        std::string                         filename;
        std::string                         working_file_name;
        bool                                append;
        bool                                flatten;
        bool                                write_version;
        
    };
    
}

#endif

