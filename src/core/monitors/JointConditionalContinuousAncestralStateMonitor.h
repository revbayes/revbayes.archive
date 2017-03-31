#ifndef JointConditionalContinuousAncestralStateMonitor_H
#define JointConditionalContinuousAncestralStateMonitor_H

#include "ContinuousCharacterData.h"
#include "Monitor.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "StochasticNode.h"
#include "DnaState.h"

#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>

namespace RevBayesCore {
    
    /**
     * @brief Declaration and implementation of the JointConditionalContinuousAncestralStateMonitor class.
     *
     * @file
	 * Declaration and implementation of the JointConditionalContinuousAncestralStateMonitor class which
	 * monitors the ancestral states of each internal node and
	 * prints their value into a file.
     *
     * @copyright Copyright 2014-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @version 1.0
     *
     */
	class JointConditionalContinuousAncestralStateMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
		JointConditionalContinuousAncestralStateMonitor(TypedDagNode<Tree> *t, StochasticNode<ContinuousCharacterData>* ch, unsigned long g, const std::string &fname, const std::string &del, bool wt, bool wss);                                  //!< Constructor
        JointConditionalContinuousAncestralStateMonitor(const JointConditionalContinuousAncestralStateMonitor &m);
        virtual ~JointConditionalContinuousAncestralStateMonitor(void);
        
        JointConditionalContinuousAncestralStateMonitor*          clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                            monitor(unsigned long gen);                                         //!< Monitor at generation gen
        void                                            closeStream(void);                                                  //!< Close stream after finish writing
        void                                            openStream(bool reopen);                                            //!< Open the stream for writing
        void                                            printHeader(void);                                                  //!< Print header
        
        // getters and setters
        void                                            setAppend(bool tf);                                                 //!< Set if the monitor should append to an existing file
		void                                            swapNode(DagNode *oldN, DagNode *newN);
		
    private:
        
        // members
        std::fstream                                outStream;
        
        // parameters
        std::string                                 filename;                                                           //!< Filename to which we print the values
        std::string                                 separator;                                                          //!< Seperator between monitored values (between columns)
		bool                                        append;                                                             //!< Flag if to append to existing file
		TypedDagNode<Tree>*                         tree;
		StochasticNode<ContinuousCharacterData>*    ccpm;
		bool                                        stochasticNodesOnly;
        bool                                        withTips;
        bool                                        withStartStates;
    };
    
}

#endif /* defined(__revbayes_proj__JointConditionalContinuousAncestralStateMonitor__) */
