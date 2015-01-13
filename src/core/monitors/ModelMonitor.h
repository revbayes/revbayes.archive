#ifndef ModelMonitor_H
#define ModelMonitor_H

#include "AbstractFileMonitor.h"

#include <string>
#include <vector>


#ifdef RB_MPI
#include <mpi.h>
#endif

namespace RevBayesCore {
    
    /**
     * @brief A monitor class that monitors all variables of a model and prints their value into a file.
     *
     * @file
     * The model monitor is a convenience monitor that simply monitors all variables of a model
     * instead of a pre-selected set. Thus, one only needs to specify the model and this monitor
     * extracts all variables that can be monitored.
     * The values will be printed into a file.
     *
     * Note that the copy constructor is necessary because streams need to be handled in a particular way.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-06-21, version 1.0
     *
     */
    class ModelMonitor : public AbstractFileMonitor {
        
    public:
        // Constructors and Destructors
        ModelMonitor(unsigned long g, const std::string &fname, const std::string &del);                                  //!< Constructor
        virtual ~ModelMonitor(void);
        
        
        
        // basic methods
        ModelMonitor*                       clone(void) const;                                                  //!< Clone the object
        
        // getters and setters
        void                                setModel(Model* m);
        void                                setStochasticNodesOnly(bool tf);                                    //!< Set if only stochastic nodes should be monitored
    
    private:
        // helper methods
        void                                resetDagNodes(void);                                                //!< Extract the variable to be monitored again.
        
        // members
        bool                                stochasticNodesOnly;                                                //!< Flag if only stochastic nodes should be printed
                
    };
    
}

#endif

