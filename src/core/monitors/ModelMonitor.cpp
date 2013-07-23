/**
 * @Model
 * This Model contains the implementation of ModelMonitor, used to save
 * information to Model about the monitoring of a variable DAG node.
 *
 * @brief Implementation of ModelMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2013-05-27 21:06:55 +0200 (Mon, 27 May 2013) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-21, version 1.0
 *
 * $Id: ModelMonitor.cpp 2041 2013-05-27 19:06:55Z michaellandis $
 */


#include "ModelMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "BranchLengthTree.h"
#include "StochasticNode.h"

using namespace RevBayesCore;

/* Constructor */
ModelMonitor::ModelMonitor(int g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g), outStream(), filename( fname ), separator( del ), posterior( pp ), likelihood( l ), prior( pr ), append(ap) {
    
}



ModelMonitor::ModelMonitor(const ModelMonitor &f) : Monitor( f ), outStream() {
    
    filename    = f.filename;
    separator   = f.separator;
    prior       = f.prior;
    posterior   = f.posterior;
    likelihood  = f.likelihood;
    append      = f.append;
}


/* Clone the object */
ModelMonitor* ModelMonitor::clone(void) const {
    
    return new ModelMonitor(*this);
}


void ModelMonitor::closeStream() {
    outStream.close();
}


/** Monitor value at generation gen */
void ModelMonitor::monitor(long gen) {
    
    // get the printing frequency
    int samplingFrequency = printgen;
    
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
        
        for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
            // add a separator before every new element
            outStream << separator;
            
            // get the node
            DagNode *node = *i;
            
            // print the value
            node->printValue(outStream,separator);
        }
        
        outStream << std::endl;
        
    }
}


/** open the Model stream for printing */
void ModelMonitor::openStream(void) {
    
    // open the stream to the Model
    if (append)
        outStream.open( filename.c_str(), std::fstream::out | std::fstream::app);
    else
        outStream.open( filename.c_str(), std::fstream::out);    
}

/** Print header for monitored values */
void ModelMonitor::printHeader() {
    
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
    
    for (std::set<DagNode *>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element
        outStream << separator;
        
        const DagNode* theNode = *it;
        
        // print the header
        if (theNode->getName() != "")
        {
            // print the name
            theNode->printName(outStream,separator);
        }
        else
        {
            outStream << "Unnamed";
        }
    }
    
    outStream << std::endl;
}


void ModelMonitor::setModel(Model *m)
{
    
    // delegate call to super class
    // model = m;
    Monitor::setModel(m);
    
    const std::vector<DagNode*> &n = model->getDagNodes();
    for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) 
    {
        if ( (*it)->isStochastic() && !(*it)->isClamped() )
        {
            bool tree = dynamic_cast< StochasticNode<BranchLengthTree>* >(*it) != NULL || dynamic_cast< StochasticNode<TimeTree>* >(*it) != NULL;
            if ( tree ) 
            {
                // nothing to do (at least not yet)
            } 
            else 
            {
                nodes.insert( *it );
            }
        }
        
    }
    
}


