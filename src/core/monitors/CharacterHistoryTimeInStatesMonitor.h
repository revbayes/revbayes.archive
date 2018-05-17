//
//  CharacterHistoryTimeInStatesMonitor.h
//  RevBayes_development_branch
//
//  Created by will freyman on 4/25/18.
//  Copyright (c) 2018 will freyman. All rights reserved.
//

#ifndef RevBayes_development_branch_CharacterHistoryTimeInStatesMonitor_h
#define RevBayes_development_branch_CharacterHistoryTimeInStatesMonitor_h

#include "StateDependentSpeciationExtinctionProcess.h"
#include "AbstractFileMonitor.h"
#include "TypedDagNode.h"
#include "StochasticNode.h"

#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>


namespace RevBayesCore {
    
    /**
     * Monitor to print out the time spent in each state during an SSE stochastically mapped character history.
     */
    class CharacterHistoryTimeInStatesMonitor : public AbstractFileMonitor {
        
    public:
        
        // Constructors and Destructors
        CharacterHistoryTimeInStatesMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, const std::string &del);
        CharacterHistoryTimeInStatesMonitor(const CharacterHistoryTimeInStatesMonitor &m);
        virtual ~CharacterHistoryTimeInStatesMonitor(void);
        
        CharacterHistoryTimeInStatesMonitor*                clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                                monitorVariables(unsigned long gen);                                 //!< Monitor at generation gen
        void                                                printFileHeader(void);                                              //!< Print header
        
        // getters and setters
        void                                                swapNode(DagNode *oldN, DagNode *newN);
        
    private:
        
        // members
        TypedDagNode<Tree>*                                 tree;
        StochasticNode<Tree>*                               cdbdp;                                                              //!< The character dependent birth death process we are monitoring
    };
    
}

#endif
