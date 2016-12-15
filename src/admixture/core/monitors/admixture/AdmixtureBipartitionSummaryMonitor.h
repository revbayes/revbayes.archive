//
//  AdmixtureBipartitionSummaryMonitor.h
//  rb_mlandis
//
//  Created by Michael Landis on 3/10/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureBipartitionSummaryMonitor__
#define __rb_mlandis__AdmixtureBipartitionSummaryMonitor__

#include "Monitor.h"
#include "AdmixtureTree.h"
#include "RbVector.h"
#include "TypedDagNode.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace RevBayesCore {
    
    // store results
    struct AdmixtureEdgeRecord
    {
        int cycle;
        bool isAdmixtureEvent;
        double weight;
        double age;
        double brlen;
        double rate;
        
        AdmixtureEdgeRecord(int c, bool tf, double w, double a, double b, double r)
        {
            cycle = c;
            isAdmixtureEvent = tf;
            weight = w;
            age = a;
            brlen = b;
            rate = r;
        }
    };
    
    // sort results
    struct AdmixtureBipartitionSummaryCompare {
        
        bool operator() (const std::pair<std::pair<std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureEdgeRecord> >& lhs, const std::pair<std::pair<std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureEdgeRecord> >& rhs) const
        {
            return lhs.second.size() > rhs.second.size();
        }
    };
   
    struct TreeBipartitionCompare {
        bool operator() (const std::pair<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >& lhs, const std::pair<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >& rhs) const
        {
            return lhs.second.size() > rhs.second.size();
        }
    };
     
    class AdmixtureBipartitionSummaryMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        AdmixtureBipartitionSummaryMonitor(TypedDagNode<AdmixtureTree> *t, TypedDagNode< RbVector< double > >* br, TypedDagNode<int>* dt, int ntr, int nar, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap = false);
        AdmixtureBipartitionSummaryMonitor(const AdmixtureBipartitionSummaryMonitor& f);
        
        // basic methods
        AdmixtureBipartitionSummaryMonitor*        clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                                  //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(bool reopen);                                            //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:

        void                                initializeTaxonBipartition(void);
        void                                findTaxonBipartition(std::vector<bool>& b, AdmixtureNode* p);
        void                                flipTaxonBipartitionToMinor(std::vector<bool>& b);
        void                                updateBipartitions(void);
        std::string                         buildTreeBipartitionString(void);
        std::string                         buildAdmixtureBipartitionSummaryString(void);
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        TypedDagNode<AdmixtureTree>*        tree;
        TypedDagNode< RbVector< double > >* branchRates;
        TypedDagNode<int>* delayTimer;
        
        std::set<DagNode *>                 nodeVariables;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        
        std::map<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >                              treeBipartitions;
        std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureEdgeRecord> > > AdmixtureBipartitionSummarys;
        
        
        
        size_t numTaxa;
        size_t numSamples;
        size_t numTreeResults;
        size_t numAdmixtureResults;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureNode*> > >& x);
    
}

#endif /* defined(__rb_mlandis__AdmixtureBipartitionSummaryMonitor__) */
