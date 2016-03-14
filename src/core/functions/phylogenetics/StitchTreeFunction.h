//
//  StitchTreeFunction.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/2/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef StitchTreeFunction_h
#define StitchTreeFunction_h

#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <map>
#include <set>

namespace RevBayesCore {
    
    class StitchTreeFunction : public TypedFunction<Tree> {
        
    public:
        StitchTreeFunction(const TypedDagNode<Tree> *bt, const TypedDagNode<RbVector<Tree> >* pc, RbVector<RbVector<Taxon> > pt);
        virtual                                            ~StitchTreeFunction(void);                                                         //!< Virtual destructor
        
        // public member functions
        StitchTreeFunction*                                 clone(void) const;                                                                  //!< Create an independent clone
        void                                                keep(DagNode* affecter);
        void                                                restore(DagNode *restorer);
        void                                                reInitialized(void);                                                                //!< The arguments have been re-initialized
        void                                                touch(DagNode *toucher );
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    private:
        void                                                updateStitchTree(void);
        void                                                initTaxonGroups(void);
        void                                                recursivelyStitchPatchClades(TopologyNode* node);
        void                                                recursivelyCleanPatchClade(TopologyNode* node, TopologyNode*& newRoot, std::set<Taxon>& remainingTaxa);
//        void                                                stitchPatchClade(TopologyNode* node, const Tree& patchClade);
        
        // parameters
        const TypedDagNode<Tree>*                           backboneTree;
        const TypedDagNode<RbVector<Tree> >*                patchClades;
        RbVector<RbVector<Taxon> >                          patchTaxa;
        RbVector<Taxon>                                     stitchTaxon;
        std::vector<std::set<Taxon> >                       prunedTaxa;
        size_t                                              numPatches;
        std::map<Taxon, int>                                taxonToIndex;
        
    };
    
}

#endif /* StitchTreeFunction_h */
