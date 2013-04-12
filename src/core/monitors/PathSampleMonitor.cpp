/**
 * @file
 * This file contains the implementation of FileMonitor, used to save
 * information to file about the monitoring of a variable DAG node.
 *
 * @brief Implementation of FileMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-07 18:36:49 +0200 (Tue, 07 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-21, version 1.0
 *
 * $Id: FileMonitor.cpp 1753 2012-08-07 16:36:49Z hoehna $
 */


#include "PathSampleMonitor.h"
#include "DagNode.h"
#include "Monitor.h"
#include "TypedDagNode.h"

using namespace RevBayesCore;

/* Constructor */
PathSampleMonitor::PathSampleMonitor(TypedDagNode<UnivariateFunction> *n, unsigned int g, double l, double u, int k, const std::string &fname, const std::string &del) : Monitor(g,n), outStream(), filename( fname ), separator( del ), lower( l ), upper( u ), blocks( k ), theNode( n ) {
    
}


PathSampleMonitor::PathSampleMonitor(const PathSampleMonitor &f) : Monitor( f ), outStream(), theNode( f.theNode ) {
    
    filename    = f.filename;
    separator   = f.separator;
    lower       = f.lower;
    upper       = f.upper;
    blocks      = f.blocks;
}


/* Clone the object */
PathSampleMonitor* PathSampleMonitor::clone(void) const {
    
    return new PathSampleMonitor(*this);
}


void PathSampleMonitor::closeStream() {
    outStream.close();
}


/** Monitor value at generation gen */
void PathSampleMonitor::monitor(long gen) {
    
    // get the printing frequency
    int samplingFrequency = printgen;
    
    if (gen % samplingFrequency == 0) {
        // print the iteration number first
        outStream << gen;
        
        // add a separator before every new element
        outStream << separator;
            
        double size = upper - lower;
            
        // get the function
        const UnivariateFunction& f = theNode->getValue();
        // print the value
        for (int i = 0; i <= blocks; ++i) {
            outStream << f.evaluate( lower + (i/double(blocks)) * size ) << separator;
        }
        outStream << f.evaluate( upper );
        
        outStream << std::endl;
        
        outStream.flush();
        
    }
}


/** open the file stream for printing */
void PathSampleMonitor::openStream(void) {
    
    // open the stream to the file
    outStream.open( filename.c_str(), std::fstream::out );
    
}

/** Print header for monitored values */
void PathSampleMonitor::printHeader() {
    
    // print one column for the iteration number
    outStream << "Iteration";
        
    // print the header
    for (int i = 0; i <= blocks; ++i) {
        // add a separator before every new element
        outStream << separator;
        double size = upper - lower;
    
        if (theNode->getName() != "") {
//            outStream << theNode->getName() << "(" << lower + (i/double(blocks)) * size << ")";
            outStream << theNode->getName() << i+1;
        }
        else
            outStream << "f(" << lower + (i/double(blocks)) * size << ")";
    }
    
    outStream << std::endl;
}


void PathSampleMonitor::swapNode(DagNode *oldN, DagNode *newN) {
    Monitor::swapNode(oldN, newN);
    
    theNode = static_cast<TypedDagNode<UnivariateFunction>*>(newN);
}


