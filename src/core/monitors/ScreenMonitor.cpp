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
#include "StringUtilities.h"

#include <sstream>

using namespace RevBayesCore;

/* Constructor */
ScreenMonitor::ScreenMonitor(DagNode *n, int g, bool pp, bool l, bool pr) : Monitor(g,n),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    separator("   |   "),
    headerPrintingInterval( 50 )
{
    
}


/* Constructor */
ScreenMonitor::ScreenMonitor(const std::set<DagNode *> &n, int g, bool pp, bool l, bool pr) : Monitor(g,n),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    separator("   |   "),
    headerPrintingInterval( 50 )
{
    
}


ScreenMonitor::ScreenMonitor(const std::vector<DagNode *> &n, int g, bool pp, bool l, bool pr) : Monitor(g,n),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    separator("   |   "),
    headerPrintingInterval( 50 )
{
    
}


/* Clone the object */
ScreenMonitor* ScreenMonitor::clone(void) const
{
    
    return new ScreenMonitor(*this);
}



/** Monitor value at generation gen */
void ScreenMonitor::monitor(unsigned long gen)
{
    
    // get the printing frequency
    unsigned long samplingFrequency = printgen;
    
    if ( gen > 0 && gen % (headerPrintingInterval*samplingFrequency) == 0 )
    {
        std::cout << std::endl;
        printHeader();
    }
    
    
    if (gen % samplingFrequency == 0)
    {
        // print the iteration number first
        std::stringstream ss;
        ss << gen;
        std::string s = ss.str();
        StringUtilities::fillWithSpaces(s, 10, true);
        std::cout << s;
        
        if ( posterior )
        {
            // add a separator before every new element
            std::cout << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                pp += (*it)->getLnProbability();
            }
            std::stringstream ss;
            ss << pp;
            std::string s = ss.str();
            StringUtilities::fillWithSpaces(s, 12, false);
            std::cout << s;
        }
        
        if ( likelihood )
        {
            // add a separator before every new element
            std::cout << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                if ( (*it)->isClamped() )
                {
                    pp += (*it)->getLnProbability();
                }
            }
            std::stringstream ss;
            ss << pp;
            std::string s = ss.str();
            StringUtilities::fillWithSpaces(s, 12, false);
            std::cout << s;
        }
        
        if ( prior )
        {
            // add a separator before every new element
            std::cout << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                if ( !(*it)->isClamped() )
                {
                    pp += (*it)->getLnProbability();
                }
            }
            std::stringstream ss;
            ss << pp;
            std::string s = ss.str();
            StringUtilities::fillWithSpaces(s, 12, false);
            std::cout << s;
        }
        
        for (std::vector<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
        {
            // add a separator before every new element
            std::cout << separator;
            
            // get the node
            DagNode *node = *i;
            
            // print the value
            node->printValue(std::cout, separator, 12, false);
        }
        
        std::cout << std::endl;
        std::cout.flush();
        
    }
}


/** Print header for monitored values */
void ScreenMonitor::printHeader() 
{
    
    // print empty line first
    std::cout << std::endl;
    
    // print one column for the iteration number
    std::string itString = "Iteration";
    StringUtilities::fillWithSpaces(itString,10,true);
    std::cout << itString;
    
    if ( posterior ) 
    {
        // add a separator before every new element
        std::cout << separator;
        std::string poString = "Posterior";
        StringUtilities::fillWithSpaces(poString,12,false);
        std::cout << poString;
    }
    
    if ( likelihood ) 
    {
        // add a separator before every new element
        std::cout << separator;
        std::string liString = "Likelihood";
        StringUtilities::fillWithSpaces(liString,12,false);
        std::cout << liString;
    }
    
    if ( prior ) 
    {
        // add a separator before every new element
        std::cout << separator;
        std::string prString = "Prior";
        StringUtilities::fillWithSpaces(prString,12,false);
        std::cout << prString;
    }
    
    for (std::vector<DagNode *>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) 
    {
        // add a separator before every new element
        std::cout << separator;
        
        const DagNode* theNode = *it;
        
        // print the header
        if (theNode->getName() != "")
        {
            theNode->printName(std::cout,separator,12,false);
        }
        else
        {
            std::string nString = "Unnamed";
            StringUtilities::fillWithSpaces(nString,12,false);
            std::cout << nString;
        }
    }
    
    std::cout << std::endl;
}


