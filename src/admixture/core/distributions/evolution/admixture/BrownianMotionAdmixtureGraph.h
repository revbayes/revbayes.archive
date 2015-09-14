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

#include "ContinuousCharacterData.h"
#include "MatrixReal.h"
#include "EigenSystem.h"
#include "TypedDistribution.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class SnpData;
    
    class BrownianMotionAdmixtureGraph : public TypedDistribution< ContinuousCharacterData > {
        
    public:
        BrownianMotionAdmixtureGraph(const TypedDagNode<AdmixtureTree> *t, const TypedDagNode<double> *dr, const TypedDagNode<double> *ar, const TypedDagNode< RbVector< double > > *br, SnpData* s, bool uw, bool uc, bool ub, bool dnpdm, int bs, double ls=1.0);
        BrownianMotionAdmixtureGraph(const BrownianMotionAdmixtureGraph &n);
        virtual                                     ~BrownianMotionAdmixtureGraph(void);
        
        // inherited pure virtual functions
        BrownianMotionAdmixtureGraph*               clone(void) const;
        double                                      computeLnProbability(void);
        void                                        redrawValue(void);
        void                                        setValue(ContinuousCharacterData* v);
        void                                        keepSpecialization(DagNode* affecter);
        void                                        restoreSpecialization(DagNode* restorer);
        void                                        touchSpecialization(DagNode* toucher);
        void                                        updateParameters(DagNode* toucher);
        std::vector<std::vector<double> >           getResiduals(void) const;
        double                                      getResiduals(int i, int j) const;
 		
    protected:
        // Parameter management functions
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        double                                      computeLnProbComposite(void);
        double                                      computeLnProbWishart(void);
        double                                      computeLnProbWishart2(void);
        void                                        computeLnZWishart(void);
        void                                        computeLnDetX(void);
        double                                      computeLnDeterminant(EigenSystem* e);
        double                                      computeLnMvnGammaFn(double a, int p);
        
        void                                        fillPruningOrder(AdmixtureNode* p, std::vector<AdmixtureNode*>& pruningOrder);
        
        
        void                                        initializeData(void);                           // X^, fixed
        void                                        initializeSampleMeans(void);                    // mu, fixed
        void                                        initializeSampleCovarianceBias(void);           // B', fixed
        void                                        initializeSampleCovarianceEstimator(void);      // W^ (per block), fixed
        void                                        initializeMeanSampleCovarianceEstimator(void);  // W^ (averaged), fixed
        void                                        initializeCompositeCovariance(void);          // Sigma, fixed
        void                                        initializeSampleCovariance(void);               // V
        void                                        initializeCovariance(void);                     // W
        void                                        initializeResiduals(void);
        void                                        initializeTipNodesByIndex(void);
        void                                        initializeRbCovarianceEigensystem(void);
        void                                        initializeMissingDataCorrection(void);
        
        void                                        updateResiduals(void);                          // R, updates
        void                                        updateCovariance(void);                         // V, updates
        void                                        updateCovarianceWithDiffusionRate(void);
        void                                        updateSampleCovariance(void);                   // W, updates
        void                                        updateCovarianceWithoutAdmixture(void);
        void                                        updateAllNodePathsToRoot(void);
        void                                        updateTipPathsToRoot(void);
        void                                        updateRbCovarianceEigensystem(void);

        
        void                                        findAllDagPathsToRoot(AdmixtureNode* node, std::list<AdmixtureNode*> path, std::set<std::list<AdmixtureNode*> >& pathSet);
        double                                      findCovariance(AdmixtureNode* p, AdmixtureNode* q);

#ifdef USE_LIB_ARMADILLO
        int                                         findFirstDuplicateIndex(arma::mat A);
        void                                        svd(arma::mat& S, arma::mat& X, int x);
        void                                        eigenTest(arma::mat W);
#endif
        
        void                                        testGsl(void);
        void                                        testArma(void);
        void                                        testEigen(void);
        void                                        testOpenCv(void);
        void                                        testWishart(void);

        
        void                                        print(const std::vector<std::vector<double> >& x);
        void                                        printR(const std::vector<std::vector<double> >& x);
        
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
        std::vector<std::vector<double> >                               missingDataCorrection;
        
        // population graph
        std::map<AdmixtureNode*,std::set<std::list<AdmixtureNode*> > >  pathsToRoot;
        std::vector<AdmixtureNode*>                                     tipNodesByIndex;
        
        // graph nodes
        const TypedDagNode<AdmixtureTree>*                              tau;
        const TypedDagNode<double>*                                     diffusionRate;
        const TypedDagNode<double>*                                     admixtureRate;
        const TypedDagNode< RbVector< double > >*                    branchRates;

        // RevBayes eigensystem for Wishart dist
        MatrixReal                                                      rbCovariance;
        MatrixReal                                                      rbCovarianceInverse;
        EigenSystem                                                     rbCovarianceEigensystem;
        MatrixReal                                                      rbMeanSampleCovariance;
        EigenSystem                                                     rbMeanSampleCovarianceEigensystem;
        double                                                          regularizationFactor;
        
        // data dimensions
        unsigned int numSites;
        unsigned int numNodes;
        unsigned int numTaxa;
        unsigned int numBlocks;
        unsigned int blockSize;
        
        // model settings
        bool useWishart;
        bool useContrasts;
        bool useBias;
        bool discardNonPosDefMtx;
        
        double likelihoodScaler;            // default 1.0, flattens likelihood...
        double lnProbMvn;
        double lnZWishart;
        double lnDetX;
        
        // contrasts data structures
        double                              computeLnProbContrasts(void);
        void                                updateDagTraversal(void);
        void                                updateNodeToIndexContrastData(void);
        void                                initializeNodeToIndexContrastData(void);
        void                                passDagTraversal(AdmixtureNode* p);
        std::vector<AdmixtureNode*>         dagTraversalOrder;
        std::map<AdmixtureNode*,int>        nodeToIndex;
        std::vector<std::vector<double> >   contrastData;
        std::vector<double>                 contrastVar;
        
        double storedLnL;
        bool cloned;
    };
}

#endif /* defined(__revbayes_mlandis__BrownianMotionAdmixtureGraph__) */
