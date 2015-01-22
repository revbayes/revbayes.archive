/**
 * @file
 * This file contains the declaration of PriorMonitor, used to save the Prior
 * value of a model.
 *
 * @brief Declaration of PriorMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-11-15 16:03:33 +0100 (Thu, 15 Nov 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: PriorMonitor.h 1833 2012-11-15 15:03:33Z boussau $
 */

#ifndef PriorMonitor_H
#define PriorMonitor_H

#include "Monitor.h"


#include <iostream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class PriorMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        PriorMonitor(DagNode *n, int g);                                                                //!< Constructor with single DAG node
        PriorMonitor(const std::vector<DagNode *> &n, int g);                                           //!< Constructor with vector of DAG node
        PriorMonitor(const PriorMonitor &p);                                                            //!< Copy constructor

        // basic methods
        PriorMonitor*                       clone(void) const;                                          //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                 //!< Monitor at generation gen
        
        // PriorMonitor functions
        std::vector<double>                 getValues();                                                //!< Get the vector of values
        
    private:
        
        // parameters
        std::vector< double >               values;
    };
    
}

#endif

