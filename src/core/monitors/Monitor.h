/**
 * @file
 * This file contains the declaration of the monitor base class, used to hold information
 * about the monitoring of a variable DAG node.
 *
 * @brief Declaration of the monitor base class
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-19, version 1.0
 *
 * $Id$
 */

#ifndef Monitor_H
#define Monitor_H

#include "Cloneable.h"

#include <ostream>
#include <set>
#include <vector>

namespace RevBayesCore {

    class DagNode;
    class Mcmc;
    class Model;

    class Monitor : public Cloneable {
    
    public:
        // Constructors and Destructors
        Monitor(unsigned long printgen);                                                                                          //!< Default Constructor
        Monitor(unsigned long printgen, DagNode *n);                                                                              //!< Default Constructor
        Monitor(unsigned long printgen, const std::vector<DagNode *> &n);                                                         //!< Default Constructor
        Monitor(const Monitor &x);                                                                                      //!< Copy Constructor
        virtual ~Monitor(void);                                                                                         //!< Destructor

        // overloaded operators
        Monitor&                                    operator=(const Monitor &i);
            
        // pure virtual member functions
        virtual Monitor*                            clone(void) const = 0;
        virtual void                                monitor(unsigned long gen) = 0;                                     //!< InferenceMonitor at generation gen

        // methods you may want to overwrite
        virtual void                                swapNode(DagNode *oldN, DagNode *newN);
        virtual void                                closeStream(void);                                                  //!< Close stream after finish writing
        virtual void                                openStream(void);                                                   //!< Open the stream for writing
        virtual void                                printHeader(void);                                                  //!< Print header
        virtual void                                setModel(Model* m);
        virtual void                                setReplicateIndex(size_t idx);                                      //!< Set the index of the replicate here.
        virtual void                                reset(size_t numCycles);                                            //!< Reset the monitor for a new start.

        // getters and setters
        const std::vector<DagNode *>&               getDagNodes(void) const;                                            //!< Get the nodes vector
        void                                        setDagNodes(const std::set<DagNode *>& args);
        void                                        setDagNodes(const std::vector<DagNode *>& args);
        void                                        setMcmc(Mcmc* m);

    protected:
    
        // parameters
        unsigned long                               printgen;
        Mcmc*                                       mcmc;
        std::vector<DagNode *>                      nodes;
        const Model*                                model;
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Monitor& x);                                //!< Overloaded output operator

    
}



// Global functions using the class
std::ostream&                       operator<<(std::ostream& o, const RevBayesCore::Monitor& x);   

#endif

