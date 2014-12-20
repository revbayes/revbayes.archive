#ifndef ModelMonitor_H
#define ModelMonitor_H

#include "Monitor.h"

#include <fstream>
#include <sstream>
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
    class ModelMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        ModelMonitor(unsigned long g, const std::string &fname, const std::string &del);                                  //!< Constructor
        ModelMonitor(const ModelMonitor &m);
        virtual ~ModelMonitor(void);
        
        
        
        // basic methods
        ModelMonitor*                       clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                         //!< Monitor at generation gen
        
        // ModelMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
        // getters and setters
        void                                setAppend(bool tf);                                                 //!< Set if the monitor should append to an existing file
        void                                setModel(Model *m);                                                 //!< Set the model for which the monitor should print values
        void                                setPrintLikelihood(bool tf);                                        //!< Set flag whether to print the likelihood
        void                                setPrintPosterior(bool tf);                                         //!< Set flag whether to print the posterior probability
        void                                setPrintPrior(bool tf);                                             //!< Set flag whether to print the prior probability
        void                                setStochasticNodesOnly(bool tf);                                    //!< Set if only stochastic nodes should be monitored
    
    private:
        // helper methods
        void                                resetDagNodes(void);                                                //!< Extract the variable to be monitored again.
        
        // members
//#ifdef RB_MPI
//        std::stringstream                   outStream;
//#else
        std::fstream                        outStream;
//#endif
        
        // parameters
        std::string                         filename;                                                           //!< Filename to which we print the values
        std::string                         separator;                                                          //!< Seperator between monitored values (between columns)
        bool                                posterior;                                                          //!< Flag if to print the posterior probability
        bool                                likelihood;                                                         //!< Flag if to print the likelihood
        bool                                prior;                                                              //!< Flag if to print the prior probability
        bool                                append;                                                             //!< Flag if to append to existing file
        bool                                stochasticNodesOnly;                                                //!< Flag if only stochastic nodes should be printed
        
#ifdef RB_MPI
        MPI_File                            file;
#endif
        
    };
    
}

#endif

