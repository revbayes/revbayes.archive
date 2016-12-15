//
//  ExtendedNewickAdmixtureTreeMonitor.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/28/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__ExtendedNewickAdmixtureTreeMonitor__
#define __revbayes_mlandis__ExtendedNewickAdmixtureTreeMonitor__

#include "Monitor.h"
#include "AdmixtureTree.h"
#include "TypedDagNode.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class ExtendedNewickAdmixtureTreeMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        ExtendedNewickAdmixtureTreeMonitor(TypedDagNode<AdmixtureTree> *t, TypedDagNode< RbVector< double > >* br, bool sm, bool sr, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false);
        ExtendedNewickAdmixtureTreeMonitor(const ExtendedNewickAdmixtureTreeMonitor& f);
        
        // basic methods
        ExtendedNewickAdmixtureTreeMonitor*          clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                                  //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(bool reopen);                                            //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        std::string                         buildExtendedNewick();
        std::string                         buildExtendedNewick(AdmixtureNode* n);
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        TypedDagNode<AdmixtureTree>*        tree;
        TypedDagNode< RbVector< double > >* branchRates;
        std::set<DagNode *>                 nodeVariables;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        bool                                showMetadata;
        bool                                showRates;
        
    };
    
}
#endif /* defined(__revbayes_mlandis__ExtendedNewickAdmixtureTreeMonitor__) */
