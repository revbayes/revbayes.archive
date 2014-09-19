#ifndef AncestralStateMonitor_H
#define AncestralStateMonitor_H

#include "Monitor.h"

#include <fstream>
#include <string>
#include <vector>

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
    class AncestralStateMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        AncestralStateMonitor(unsigned long g, const std::string &fname, const std::string &del);                                  //!< Constructor
        AncestralStateMonitor(const AncestralStateMonitor &m);
        virtual ~AncestralStateMonitor(void);
        
        
        
        // basic methods
        AncestralStateMonitor*              clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                         //!< Monitor at generation gen
        
        // AncestralStateMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
        // getters and setters
        void                                setAppend(bool tf);                                                 //!< Set if the monitor should append to an existing file
        void                                setSeqObject(AncestralState *m);                                                 //!< Set the model for which the monitor should print values
		
    private:
        // helper methods
        void                                resetDagNodes(void);                                                //!< Extract the variable to be monitored again.
        
        // members
        std::fstream                        outStream;
        
        // parameters
        std::string                         filename;                                                           //!< Filename to which we print the values
        std::string                         separator;                                                          //!< Seperator between monitored values (between columns)
        bool                                append;                                                             //!< Flag if to append to existing file
    };
    
}

#endif

