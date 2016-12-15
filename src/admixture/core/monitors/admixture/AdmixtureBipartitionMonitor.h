//
//  AdmixtureBipartitionMonitor.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureBipartitionMonitor__
#define __rb_mlandis__AdmixtureBipartitionMonitor__

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
    struct AdmixtureBipartitionCompare {
        
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
    
    class AdmixtureBipartitionMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        AdmixtureBipartitionMonitor(TypedDagNode<AdmixtureTree> *t, TypedDagNode<double>* dr, TypedDagNode< RbVector< double > >* br, size_t ntr, size_t nar, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap = false, bool ci=false, bool ch=false);
        AdmixtureBipartitionMonitor(const AdmixtureBipartitionMonitor& f);
        
        // basic methods
        AdmixtureBipartitionMonitor*        clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                         //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(bool reopen);                                            //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        
        void                                initializeTaxonBipartition(void);
        void                                findTaxonBipartition(std::vector<bool>& b, AdmixtureNode* p);
        void                                flipTaxonBipartitionToMinor(std::vector<bool>& b);
        //void                                updateBipartitions(void);
        std::string                         buildBipartitionString(void);
        //std::string                         buildAdmixtureBipartitionString(void);
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        TypedDagNode<AdmixtureTree>*        tree;
        TypedDagNode< RbVector< double > >* branchRates;
        TypedDagNode<double>*               driftRate;
        
        std::set<DagNode *>                 nodeVariables;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        bool                                chainIdx;
        bool                                chainHeat;
        
        std::map<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >                              treeBipartitions;
        std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureEdgeRecord> > > AdmixtureBipartitions;
        
        
        
        size_t numTaxa;
        size_t numSamples;
        size_t numTreeResults;
        size_t numAdmixtureResults;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureNode*> > >& x);
    
}

#endif /* defined(__rb_mlandis__AdmixtureBipartition__) */
