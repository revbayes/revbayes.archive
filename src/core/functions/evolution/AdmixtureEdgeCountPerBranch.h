//
//  AdmixtureEdgeCountPerBranch.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/11/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureEdgeCountPerBranch__
#define __revbayes_mlandis__AdmixtureEdgeCountPerBranch__

#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "AdmixtureNode.h"
#include "AdmixtureTree.h"
#include <vector>
#include <map>

namespace RevBayesCore {
    
    class AdmixtureEdgeCountPerBranch : public TypedFunction< std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureNode*> > > >
    {
    public:
        AdmixtureEdgeCountPerBranch(const TypedDagNode<AdmixtureTree>* t, const TypedDagNode<int>* d, size_t nr);
        AdmixtureEdgeCountPerBranch(const AdmixtureEdgeCountPerBranch& n);
        virtual ~AdmixtureEdgeCountPerBranch(void);
        
        AdmixtureEdgeCountPerBranch*               clone(void) const;  //!< Create an independent clone
        void                                       update(void);
        std::string                                getSummary(void);
        size_t                                     getNumSamples(void);

    protected:
        void                                       swapParameterInternal(const DagNode *oldP, const DagNode *newP);
        
    private:
        void                                       initializeTaxonBipartition(void);
        void                                       findTaxonBipartition(std::vector<bool>& b, AdmixtureNode* p);
        void                                       flipTaxonBipartitionToMinor(std::vector<bool>& b);
        
        const TypedDagNode<AdmixtureTree>* tau;
        const TypedDagNode<int>* delayTimer;
        //std::vector<AdmixtureNode*> nodesByIndex;
        //std::vector<bool> taxonBipartitionKey;
        size_t numTaxa;
        //        size_t numNodes;
        size_t numSamples;
        size_t numResults;

    };
    
    // used to summarize results
    struct moreEdgesCompare {
        bool operator() (const std::pair<std::pair<std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureNode*> >& lhs, const std::pair<std::pair<std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureNode*> >& rhs) const
        {
            return lhs.second.size() > rhs.second.size();
        }
    };
    
    std::ostream& operator<<(std::ostream& o, const std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureNode*> > >& x);


}

#endif /* defined(__revbayes_mlandis__AdmixtureEdgeCountPerBranch__) */
