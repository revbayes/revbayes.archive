//
//  PruneTreeFunction.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/2/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef PruneTreeFunction_h
#define PruneTreeFunction_h

#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <map>
#include <set>

namespace RevBayesCore {
    
    class PruneTreeFunction : public TypedFunction<Tree> {
        
    public:
        PruneTreeFunction(const TypedDagNode<Tree> *t, std::set<Taxon> pt, bool pf);
        virtual                                            ~PruneTreeFunction(void);                                                         //!< Virtual destructor
        
        // public member functions
        PruneTreeFunction*                                  clone(void) const;                                                                  //!< Create an independent clone
        void                                                keep(DagNode* affecter);
        void                                                restore(DagNode *restorer);
        void                                                reInitialized(void);                                                                //!< The arguments have been re-initialized
        void                                                touch(DagNode *toucher );
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    private:
        void                                                setRetainedTaxa(void);
        int                                                 recursivelyRetainTaxa(TopologyNode* node);
        
        // members
        std::map<TopologyNode*, int>                        pruneCount;
        bool                                                pruneFossils;
        std::set<Taxon>                                     prunedTaxa;
        std::map<Taxon, int>                                retainedIndices;
        std::set<Taxon>                                     retainedTaxa;

        // parameters
        const TypedDagNode<Tree>*                           tau;

    };
    
}

#endif /* PruneTreeFunction_h */
