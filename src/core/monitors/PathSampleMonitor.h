/**
 * @file
 * This file contains the declaration of Monitor, used to save information
 * to a file about the monitoring of a variable DAG node. This special monitor is only for monitoring
 * functions by evaluating the function at several values.
 *
 * @brief Declaration of PathSampleMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-07 18:36:49 +0200 (Tue, 07 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-14, version 1.0
 *
 * $Id: FileMonitor.h 1753 2012-08-07 16:36:49Z hoehna $
 */

#ifndef PathSampleMonitor_H
#define PathSampleMonitor_H

#include "UnivariateFunction.h"
#include "Monitor.h"
#include "TypedDagNode.h"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class PathSampleMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        PathSampleMonitor(TypedDagNode<UnivariateFunction> *n,unsigned int g, double l, double u, int k, const std::string &fname, const std::string &del);                                                                //!< Constructor with single DAG node
        PathSampleMonitor(const PathSampleMonitor& f);
        
        // basic methods
        PathSampleMonitor*                  clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(long gen);                                                  //!< Monitor at generation gen
        virtual void                        swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        std::fstream                        outStream;
        
        // parameters
        std::string                         filename;
        std::string                         separator;
        double                              lower;
        double                              upper;
        int                                 blocks;
        TypedDagNode<UnivariateFunction>*   theNode;
    };
    
}

#endif

