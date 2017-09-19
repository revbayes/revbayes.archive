//
//  JointConditionalCharacterMappingMonitor.h
//  RevBayes_development_branch
//
//  Created by will freyman on 12/13/16.
//  Copyright (c) 2016 will freyman. All rights reserved.
//


#ifndef RevBayes_development_branch_JointConditionalCharacterMappingMonitor_h
#define RevBayes_development_branch_JointConditionalCharacterMappingMonitor_h


#include "AbstractHomologousDiscreteCharacterData.h"
#include "StateDependentSpeciationExtinctionProcess.h"
#include "Monitor.h"
#include "TypedDagNode.h"
#include "StochasticNode.h"

#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>


namespace RevBayesCore {
    
    /**
     * @brief Declaration and implementation of the JointConditionalCharacterMappingMonitor class.
     *
     * @file
     * Declaration and implementation of the JointConditionalCharacterMappingMonitor class which
     * monitors samples of character histories drawn from the character dependent birth death process
     * and prints their value into a file.
     *
     */
    class JointConditionalCharacterMappingMonitor : public Monitor {
        
    public:
        
        // Constructors and Destructors
        JointConditionalCharacterMappingMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, bool is, const std::string &del);
        JointConditionalCharacterMappingMonitor(const JointConditionalCharacterMappingMonitor &m);
        virtual ~JointConditionalCharacterMappingMonitor(void);
        
        JointConditionalCharacterMappingMonitor*        clone(void) const;                                                  //!< Clone the object
        
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
        std::fstream                                    outStream;
        
        // parameters
        bool                                            include_simmaps;                                                    //!< Should we print out SIMMAP/phytools compatible character histories?
        std::string                                     filename;                                                           //!< Filename to which we print the values
        std::string                                     separator;                                                          //!< Seperator between monitored values (between columns)
        bool                                            append;                                                             //!< Flag if to append to existing file
        TypedDagNode<Tree>*                             tree;
        StochasticNode<Tree>*                           cdbdp;                                                              //!< The character dependent birth death process we are monitoring

    };
    
}

#endif
