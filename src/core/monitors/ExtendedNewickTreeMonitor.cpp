/**
 * @file
 * This file contains the implementation of ExtendedNewickTreeMonitor, used to save
 * information to file about the tree and additional variables at nodes, e.g. branch rates, 
 * in extended Newick format.
 *
 * @brief Implementation of ExtendedNewickTreeMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-11-26 05:34:51 -0800 (Mon, 26 Nov 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-21, version 1.0
 *
 * $Id: FileMonitor.cpp 1867 2012-11-26 13:34:51Z hoehna $
 */


#include "ExtendedNewickTreeMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbException.h"

using namespace RevBayesCore;

/* Constructor */
ExtendedNewickTreeMonitor::ExtendedNewickTreeMonitor(TypedDagNode<TimeTree> *t, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,t),
    outStream(),
    tree( t ),
    filename( fname ),
    separator( del ),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    append(ap)
{
    
}


/* Constructor */
ExtendedNewickTreeMonitor::ExtendedNewickTreeMonitor(TypedDagNode<TimeTree> *t, const std::set<TypedDagNode< RbVector<double> > *> &n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,t),
    outStream(),
    tree( t ),
    nodeVariables( n ),
    filename( fname ),
    separator( del ),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    append(ap)
{
//    this->nodes.insert( tree );
    
    for (std::set<TypedDagNode< RbVector<double> > *>::iterator it = nodeVariables.begin(); it != nodeVariables.end(); ++it) {
        this->nodes.push_back( *it );
    }
}


ExtendedNewickTreeMonitor::ExtendedNewickTreeMonitor(const ExtendedNewickTreeMonitor &m) : Monitor( m ), outStream(), tree( m.tree ), nodeVariables( m.nodeVariables ) {
    
    filename    = m.filename;
    separator   = m.separator;
    prior       = m.prior;
    posterior   = m.posterior;
    likelihood  = m.likelihood;
    append      = m.append;
    
    if (m.outStream.is_open())
        openStream();
}


/* Clone the object */
ExtendedNewickTreeMonitor* ExtendedNewickTreeMonitor::clone(void) const {
    
    return new ExtendedNewickTreeMonitor(*this);
}


void ExtendedNewickTreeMonitor::closeStream() {
    outStream.close();
}


/** Monitor value at generation gen */
void ExtendedNewickTreeMonitor::monitor(unsigned long gen) {
    
    // get the printing frequency
    unsigned long samplingFrequency = printgen;
    
    if (gen % samplingFrequency == 0) {
        // print the iteration number first
        outStream << gen;
        
        if ( posterior ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                pp += (*it)->getLnProbability();
            }
            outStream << pp;
        }
        
        if ( likelihood ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( (*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        if ( prior ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( !(*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        // add a separator before the tree
        outStream << separator;
        
        tree->getValue().clearBranchParameters();
        for (std::set<TypedDagNode< RbVector<double> > *>::iterator it = nodeVariables.begin(); it != nodeVariables.end(); ++it) {
            tree->getValue().addBranchParameter((*it)->getName(), (*it)->getValue(), false);
        }
            
        outStream << tree->getValue();
        
        outStream << std::endl;
        
    }
}


/** open the file stream for printing */
void ExtendedNewickTreeMonitor::openStream(void) {
    
    // open the stream to the file
    if (append)
        outStream.open( filename.c_str(), std::fstream::out | std::fstream::app);
    else
        outStream.open( filename.c_str(), std::fstream::out);
}

/** Print header for monitored values */
void ExtendedNewickTreeMonitor::printHeader() {
    
    // print one column for the iteration number
    outStream << "Iteration";
    
    if ( posterior ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Posterior";
    }
    
    if ( likelihood ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Likelihood";
    }
    
    if ( prior ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Prior";
    }
    
    // add a separator tree
    outStream << separator << "Tree";
        
    // end line of stream
    outStream << std::endl;
}


void ExtendedNewickTreeMonitor::swapNode(DagNode *oldN, DagNode *newN) {
    
    TypedDagNode< RbVector<double> >* nodeVar = dynamic_cast< TypedDagNode< RbVector<double> > *>(oldN);
    if ( oldN == tree )
    {
        tree = static_cast< TypedDagNode< TimeTree > *>( newN );
    }
    else if ( nodeVar != NULL )
    {
        // error catching
        if ( nodeVariables.find(nodeVar) == nodeVariables.end() ) {
            throw RbException("Cannot replace DAG node with name\"" + oldN->getName() + "\" in this extended newick monitor because the monitor doesn't hold this DAG node.");
        }
        
        nodeVariables.erase( nodeVar );
        nodeVariables.insert( static_cast< TypedDagNode< RbVector<double> > *>(newN) );
    }
    
    // delegate to base class
    Monitor::swapNode(oldN, newN);
}


