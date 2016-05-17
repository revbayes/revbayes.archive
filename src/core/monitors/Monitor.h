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
        virtual void                                addFileExtension(const std::string &s, bool dir);
        virtual void                                addVariable(DagNode *n);
        virtual void                                closeStream(void);                                                  //!< Close stream after finish writing
        virtual void                                combineReplicates(size_t n);                                        //!< Combine results from several replicate analyses
        virtual void                                disable(void);                                                      //!< Disable this monitor (momentarily)
        virtual void                                enable(void);                                                       //!< Enable this monitor
        virtual bool                                isEnabled(void) const;                                              //!< Is the monitor currently enabled?
        virtual bool                                isScreenMonitor(void) const;                                        //!< Is this a screen monitor?
        virtual void                                openStream(bool reopen);                                            //!< Open the stream for writing
        virtual void                                printHeader(void);                                                  //!< Print header
        virtual void                                setModel(Model* m);
        virtual void                                swapNode(DagNode *oldN, DagNode *newN);
        virtual void                                removeVariable(DagNode *n);
        virtual void                                reset(size_t numCycles);                                            //!< Reset the monitor for a new start.

        // getters and setters
        const std::vector<DagNode *>&               getDagNodes(void) const;                                            //!< Get the nodes vector
//        void                                        setDagNodes(const std::set<DagNode *>& args);
        void                                        setDagNodes(const std::vector<DagNode *>& args);
        void                                        setMcmc(Mcmc* m);

    protected:
    
        void                                        sortNodesByName(void);                                              //!< Sort the nodes by name
        
        // parameters
        bool                                        enabled;
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

