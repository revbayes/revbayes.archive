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


#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class AbstractFileMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        AbstractFileMonitor(DagNode *n, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false);                                                                //!< Constructor with single DAG node
        AbstractFileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false);                                              //!< Constructor with vector of DAG node
        AbstractFileMonitor(const AbstractFileMonitor& f);
        
        // basic methods
        virtual AbstractFileMonitor*        clone(void) const = 0;                                              //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                         //!< Monitor at generation gen
        void                                printHeader(void);                                                  //!< Print header
        void                                setReplicateIndex(size_t idx);
        void                                setStoneIndex(size_t idx);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                setAppend(bool tf);                                                 //!< Set if the monitor should append to an existing file
        void                                setPrintLikelihood(bool tf);                                        //!< Set flag whether to print the likelihood
        void                                setPrintPosterior(bool tf);                                         //!< Set flag whether to print the posterior probability
        void                                setPrintPrior(bool tf);                                             //!< Set flag whether to print the prior probability

        // functions you may want to overwrite
        virtual void                        monitorVariables(unsigned long gen);                                //!< Monitor at generation gen
        virtual void                        printFileHeader(void);                                              //!< Print header
    
    protected:
        std::fstream                        outStream;
        
        // parameters
        std::string                         filename;
        std::string                         workingFileName;
        size_t                              replicateIndex;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        bool                                flatten;
        
    };
    
}

#endif

