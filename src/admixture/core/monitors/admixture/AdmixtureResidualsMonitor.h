//
//  AdmixtureResidualsMonitor.h
//  rb_mlandis
//
//  Created by Michael Landis on 3/10/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureResidualsMonitor__
#define __rb_mlandis__AdmixtureResidualsMonitor__

#include "Monitor.h"
#include "TypedDagNode.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>


namespace RevBayesCore
{
  
    class AdmixtureResidualsMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        AdmixtureResidualsMonitor(TypedDagNode< RbVector<double> > *r, std::vector<std::string> tn, int g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false);
        AdmixtureResidualsMonitor(const AdmixtureResidualsMonitor& f);
        
        // basic methods
        AdmixtureResidualsMonitor*        clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                                  //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        
        void                                initializeResiduals(void);
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        TypedDagNode< RbVector< double > >* residuals;
        
        std::set<DagNode *>                 nodeVariables;
        std::vector<std::string>            taxonNames;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureResidualsMonitor__) */
