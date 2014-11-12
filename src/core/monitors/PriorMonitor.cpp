/**
 * @file
 * This file contains the implementation of PriorMonitor, used to save
 * the prior of the model.
 *
 * @brief Implementation of PriorMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-11-15 16:03:33 +0100 (Thu, 15 Nov 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-21, version 1.0
 *
 * $Id: PriorMonitor.cpp 1833 2012-11-15 15:03:33Z boussau $
 */


#include "PriorMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "StringUtilities.h"

#include <cmath>
#include <iomanip>
#include <sstream>

using namespace RevBayesCore;

/* Constructor */
PriorMonitor::PriorMonitor(DagNode *n, int g) : Monitor(g,n), values(0,0.0)
{
    
}


/* Constructor */
PriorMonitor::PriorMonitor(const std::set<DagNode *> &n, int g) : Monitor(g,n), values(0,0.0)
{
    
}


/* Constructor */
PriorMonitor::PriorMonitor(const std::vector<DagNode *> &n, int g) : Monitor(g,n), values(0,0.0)
{
    
}


/* Copy constructor */
PriorMonitor::PriorMonitor(const PriorMonitor &p) : Monitor(p), values(p.values)
{
    
}


/* Clone the object */
PriorMonitor* PriorMonitor::clone(void) const
{
    
    return new PriorMonitor(*this);
}



/** Monitor value at generation gen */
void PriorMonitor::monitor(unsigned long gen)
{
    
    // get the printing frequency
    unsigned long samplingFrequency = printgen;
    
    
    if (gen % samplingFrequency == 0)
    {
        /*
        if ( posterior )
        {
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                pp += (*it)->getLnProbability();
            }
            
            ss << pp;
            s = ss.str();
            StringUtilities::fillWithSpaces( s, columnWidth, false );
            std::cout << prefixSeparator << s << suffixSeparator;
            ss.str("");
        }
        
        if ( likelihood )
        {
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                if ( (*it)->isClamped() )
                {
                    pp += (*it)->getLnProbability();
                }
            }
            
            ss << pp;
            s = ss.str();
            StringUtilities::fillWithSpaces( s, columnWidth, false );
            std::cout << prefixSeparator << s << suffixSeparator;
            ss.str("");
        }*/
        

            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                if ( !(*it)->isClamped() )
                {
                    pp += (*it)->getLnProbability();
                }
            }
            //We now have the prior pp
        values.push_back(pp);

        
    }
}


/** Get the vector of values */
std::vector<double> PriorMonitor::getValues() {
    return values;
    
}


