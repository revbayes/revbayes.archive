/**
 * @Model
 * This Model contains the declaration of Monitor, used to save information
 * to a Model about the monitoring of a variable DAG node.
 *
 * @brief Declaration of Monitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2013-05-27 21:06:55 +0200 (Mon, 27 May 2013) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: ModelMonitor.h 2041 2013-05-27 19:06:55Z michaellandis $
 */

#ifndef ModelMonitor_H
#define ModelMonitor_H

#include "Monitor.h"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class ModelMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        ModelMonitor(int g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false);                                                                //!< Constructor with single DAG node
        ModelMonitor(const ModelMonitor& f);
        
        // basic methods
        ModelMonitor*                       clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(long gen);                                                  //!< Monitor at generation gen
        
        // ModelMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
        void                                setModel(Model *m);
    
    private:
        std::fstream                        outStream;
        
        // parameters
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        
    };
    
}

#endif

