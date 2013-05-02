//
//  BrownianMotionAdmixtureGraph.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__BrownianMotionAdmixtureGraph__
#define __revbayes_mlandis__BrownianMotionAdmixtureGraph__


#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>

//#include "AdmixtureHistory.h"

#include "CharacterData.h"
#include "ContinuousCharacterState.h"
#include "EigenSystem.h"
#include "RbVector.h"
#include "TypedDistribution.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class SnpData;
    
    class BrownianMotionAdmixtureGraph : public TypedDistribution<CharacterData<ContinuousCharacterState> > {
        
    public:
        BrownianMotionAdmixtureGraph(const TypedDagNode<AdmixtureTree> *t, const TypedDagNode<double> *dr, const TypedDagNode<double> *ar, const TypedDagNode< std::vector< double > > *br, SnpData* s);
        BrownianMotionAdmixtureGraph(const BrownianMotionAdmixtureGraph &n);
        virtual                                     ~BrownianMotionAdmixtureGraph(void);
        
        // inherited pure virtual functions
        BrownianMotionAdmixtureGraph*               clone(void) const;
        double                                      computeLnProbability(void);
        void                                        redrawValue(void);
        void                                        setValue(CharacterData<ContinuousCharacterState>* v);
        void                                        swapParameter(const DagNode *oldP, const DagNode *newP);
        void                                        keepSpecialization(DagNode* affecter);
        void                                        restoreSpecialization(DagNode* restorer);
        void                                        touchSpecialization(DagNode* toucher);
        std::vector<std::vector<double> >           getResiduals(void) const;
        double                                      getResiduals(int i, int j) const;
 		
    private:
        
        double                                      computeLnProbComposite(void);
        
        void                                        initializeData(void);                           // X^, fixed
        void                                        initializeSampleMeans(void);                    // mu, fixed
        void                                        initializeSampleCovarianceBias(void);           // B', fixed
        void                                        initializeSampleCovarianceEstimator(void);      // W^ (per block), fixed
        void                                        initializeMeanSampleCovarianceEstimator(void);  // W^ (averaged), fixed
        void                                        initializeCompositiveCovariance(void);          // Sigma, fixed
        void                                        initializeSampleCovariance(void);               // V
        void                                        initializeCovariance(void);                     // W
        void                                        initializeResiduals(void);

        void                                        initializeTipNodesByIndex(void);
        
        void                                        updateResiduals(void);                          // R, updates
        void                                        updateCovariance(void);                         // V, updates
        void                                        updateCovarianceWithDiffusionRate(void);
        void                                        updateSampleCovariance(void);                   // W, updates
        void                                        updateCovarianceWithoutAdmixture(void);
        void                                        updatePathsToRoot(void);
        
        void                                        findAllDagPathsToRoot(AdmixtureNode* node, std::list<AdmixtureNode*> path, std::set<std::list<AdmixtureNode*> >& pathSet);
        double                                      findCovariance(AdmixtureNode* p, AdmixtureNode* q);

        void                                        print(const std::vector<std::vector<double> >& x);
        
        // data
        SnpData* snps;
        std::vector<std::vector<double> >                               data;                         // [pop_idx][site]
        
        // parameters
        std::vector<double>                                             sampleMeans;                  // mu
        std::vector<std::vector<double> >                               covariance;                   // V
        std::vector<std::vector<double> >                               sampleCovariance;             // W
        std::vector<std::vector<std::vector<double> > >                 sampleCovarianceEstimator;    // W^ (per block)
        std::vector<std::vector<double> >                               sampleCovarianceBias;         // B'
        std::vector<std::vector<double> >                               meanSampleCovarianceEstimator;// W^ (averaged)
        std::vector<std::vector<double> >                               compositeCovariance;          // Sigma
        std::vector<std::vector<double> >                               residuals;
        //std::vector<double>                                             residuals;                    // R
     //   std::vector<std::vector<AdmixtureNode*> >                       mrca;
     //   std::vector<AdmixtureNode*>                                     dagTraversalOrder;
     //   std::set<AdmixtureNode*>                                        dagTraversalVisited;
     //   std::map<AdmixtureNode*,double>                                 dagVariance;
     //   std::map<AdmixtureNode*,double>                                 dagCovariance;
        
        // population graph
        std::map<AdmixtureNode*,std::set<std::list<AdmixtureNode*> > >  pathsToRoot;
        std::vector<AdmixtureNode*>                                     tipNodesByIndex;
        
        // graph nodes
        const TypedDagNode<AdmixtureTree>*                              tau;
        const TypedDagNode<double>*                                     diffusionRate;
        const TypedDagNode<double>*                                     admixtureRate;
        const TypedDagNode< std::vector< double > >*                    branchRates;

        
               
        // data dimensions
        size_t numSites;
        size_t numNodes;
        size_t numTaxa;
        size_t numBlocks;
        size_t blockSize;
        
        bool cloned;
        
        double lnProbMvn;
        
    };
}

#endif /* defined(__revbayes_mlandis__BrownianMotionAdmixtureGraph__) */
