/**
 * @file
 * This file contains the implementation of ScreenMonitor, used to save
 * information to the screen about the monitoring of a variable DAG node.
 *
 * @brief Implementation of ScreenMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-11-15 16:03:33 +0100 (Thu, 15 Nov 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-21, version 1.0
 *
 * $Id: FileMonitor.cpp 1833 2012-11-15 15:03:33Z hoehna $
 */


#include "ScreenMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"

using namespace RevBayesCore;

/* Constructor */
ScreenMonitor::ScreenMonitor(DagNode *n, int g, const std::string &del, bool pp, bool l, bool pr) : Monitor(g,n), separator( del ), posterior( pp ), prior( pr ), likelihood( l ) {
    
}


/* Constructor */
ScreenMonitor::ScreenMonitor(const std::set<DagNode *> &n, int g, const std::string &del, bool pp, bool l, bool pr) : Monitor(g,n), separator( del ), posterior( pp ), prior( pr ), likelihood( l ) {
    
}


ScreenMonitor::ScreenMonitor(const std::vector<DagNode *> &n, int g, const std::string &del, bool pp, bool l, bool pr) : Monitor(g,n), separator( del ), posterior( pp ), prior( pr ), likelihood( l ) {
    
}

ScreenMonitor::ScreenMonitor(const ScreenMonitor &f) : Monitor( f ) {
    
    separator   = f.separator;
    prior       = f.prior;
    posterior   = f.posterior;
    likelihood  = f.likelihood;
}


/* Clone the object */
ScreenMonitor* ScreenMonitor::clone(void) const {
    
    return new ScreenMonitor(*this);
}



/** Monitor value at generation gen */
void ScreenMonitor::monitor(long gen) {
    
    // get the printing frequency
    int samplingFrequency = printgen;
    
    if (gen % samplingFrequency == 0) {
        // print the iteration number first
        std::cout << gen;
        
        if ( posterior ) {
            // add a separator before every new element
            std::cout << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                pp += (*it)->getLnProbability();
            }
            std::cout << pp;
        }
        
        if ( likelihood ) {
            // add a separator before every new element
            std::cout << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( (*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            std::cout << pp;
        }
        
        if ( prior ) {
            // add a separator before every new element
            std::cout << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( !(*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            std::cout << pp;
        }
        
        for (std::vector<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
            // add a separator before every new element
            std::cout << separator;
            
            // get the node
            DagNode *node = *i;
            
            // print the value
            node->printValue(std::cout, separator);
        }
        
        std::cout << std::endl;
        
    }
}


/** Print header for monitored values */
void ScreenMonitor::printHeader() 
{
    
    // print empty line first
    std::cout << std::endl;
    
    // print one column for the iteration number
    std::cout << "Iteration";
    
    if ( posterior ) 
    {
        // add a separator before every new element
        std::cout << separator;
        std::cout << "Posterior";
    }
    
    if ( likelihood ) 
    {
        // add a separator before every new element
        std::cout << separator;
        std::cout << "Likelihood";
    }
    
    if ( prior ) 
    {
        // add a separator before every new element
        std::cout << separator;
        std::cout << "Prior";
    }
    
    for (std::vector<DagNode *>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) 
    {
        // add a separator before every new element
        std::cout << separator;
        
        const DagNode* theNode = *it;
        
        // print the header
        if (theNode->getName() != "")
        {
            theNode->printName(std::cout,separator);
        }
        else
        {
            std::cout << "Unnamed";
        }
    }
    
    std::cout << std::endl;
}


