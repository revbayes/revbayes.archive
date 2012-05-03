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

#include "InferenceMonitor.h"
#include "RbValue.h"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class FileMonitor : public InferenceMonitor {

public:
    // Constructors and Destructors
    FileMonitor();                                                                                          //!< Default Constructor
    FileMonitor(const FileMonitor& f);

    // Basic utility functions
    FileMonitor*                        clone(void) const;                                                  //!< Clone object
    void                                setAttribute(const std::string &name, const RbValue<void*> &a);

    // Monitor functions
    void                                monitor(void);                                                      //!< Monitor unconditionally
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
    
};

#endif

