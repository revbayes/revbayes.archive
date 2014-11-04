
//  BrownianMotionAdmixtureGraph.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "BrownianMotionAdmixtureGraph.h"
#include "DistributionExponential.h"
#include "DistributionNormal.h"
#include "DynamicNode.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathFunctions.h"
#include "RbStatisticsHelper.h"
#include "SnpData.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "TopologyNode.h"
#include "TreeChangeEventListener.h"
#include <sstream>
#include <list>
#include <algorithm>
#include <iomanip>


//#include "opencv2/core/core.hpp"
//#include <armadillo>
//#include <Eigen/Dense>
//#include <Eigen/LU>
//#include <gsl/gsl_matrix.h>
//#include <gsl/gsl_linalg.h>
//#include <gsl/gsl_cblas.h>
//#include <gsl/gsl_blas.h>

using namespace RevBayesCore;

BrownianMotionAdmixtureGraph::BrownianMotionAdmixtureGraph(const TypedDagNode<AdmixtureTree> *t, const TypedDagNode<double> *dr, const TypedDagNode<double> *ar, const TypedDagNode< RbVector< double > >* br, SnpData* s, bool uw, bool uc, bool ub, bool dnpdm, int bs, double ls) :
TypedDistribution<ContinuousCharacterData >( new ContinuousCharacterData() ),
snps(s),
tau(t),
diffusionRate(dr),
admixtureRate(ar),
branchRates(br),
rbCovariance(s->getNumPopulations(),s->getNumPopulations()),
rbCovarianceInverse(s->getNumPopulations(),s->getNumPopulations()),
rbCovarianceEigensystem(&rbCovariance),
rbMeanSampleCovariance(s->getNumPopulations(),s->getNumPopulations()),
rbMeanSampleCovarianceEigensystem(&rbMeanSampleCovariance),
regularizationFactor(1e-9),
numSites(s->getNumSnps()),
numNodes(2 * s->getNumPopulations() -1),
numTaxa(s->getNumPopulations()),
blockSize(bs), // humans == 500
useWishart(uw),
useContrasts(uc),
useBias(ub),
discardNonPosDefMtx(dnpdm),
likelihoodScaler(ls),
lnZWishart(0.0),
lnDetX(0.0),
//numBlocks(numSites/blockSize),
//tipData(snps->getSnpFrequencies()),
cloned(false)

{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( diffusionRate );
    addParameter( admixtureRate );
    addParameter( branchRates );
    
    numBlocks = numSites / blockSize;
    
    std::cout << "numTaxa\t" << numTaxa << "\n";
    std::cout << "numSites\t" << numSites << "\n";
    std::cout << "numBlocks\t" << numBlocks << "\n";
    std::cout << "blockSize\t" << blockSize << "\n";
    
    //delete this->value;
    //this->value = simulate(this->tau->getValue().getRoot());
    
//    rbCovariance = MatrixReal(numTaxa,numTaxa,0.0);
    //rbCovarianceInverse = MatrixReal(numTaxa,numTaxa,0.0);
    //rbCovarianceEigensystem = EigenSystem(&rbCovariance);
    
    // initializeTipData();
    // testWishart();
}


BrownianMotionAdmixtureGraph::BrownianMotionAdmixtureGraph(const BrownianMotionAdmixtureGraph &n) :
TypedDistribution<ContinuousCharacterData > ( new ContinuousCharacterData() ),
snps(n.snps),
pathsToRoot(n.pathsToRoot),
tipNodesByIndex(n.tipNodesByIndex),
tau(n.tau),
diffusionRate(n.diffusionRate),
admixtureRate(n.admixtureRate),
branchRates(n.branchRates),
rbCovariance(n.rbCovariance),
rbCovarianceInverse(n.rbCovarianceInverse),
rbCovarianceEigensystem(n.rbCovarianceEigensystem),
rbMeanSampleCovariance(n.rbMeanSampleCovariance),
rbMeanSampleCovarianceEigensystem(n.rbMeanSampleCovarianceEigensystem),
regularizationFactor(n.regularizationFactor),
//residualsFn(n.residualsFn),
numSites(n.numSites),
numNodes(n.numNodes),
numTaxa(n.numTaxa),
numBlocks(n.numBlocks),
blockSize(n.blockSize),
useWishart(n.useWishart),
useContrasts(n.useContrasts),
useBias(n.useBias),
discardNonPosDefMtx(n.discardNonPosDefMtx),
likelihoodScaler(n.likelihoodScaler),
lnZWishart(n.lnZWishart),
lnDetX(n.lnDetX),
cloned(n.cloned)
//usePopulationSizeLimitedWeights(n.usePopulationSizeLimitedWeights)
{
    // calling this method again is required despite the constructor initializer -- interacts poorly w/ vector<MatrixReal>, I reckon
    
    // dirty hack -- only initialize when object is cloned for Mcmc... will ask SH about how to do this elegantly
    if (cloned)
    {
        
        initializeTipNodesByIndex();
        initializeData();
        initializeMissingDataCorrection();
        initializeSampleMeans();
        initializeSampleCovarianceBias();
        initializeSampleCovarianceEstimator();
        initializeMeanSampleCovarianceEstimator();
        initializeCompositeCovariance();
        initializeCovariance();
        initializeSampleCovariance();
        initializeResiduals();
        
        // Wishart pdf constants | data, dimensions
        if (useWishart)
        {
            computeLnZWishart();
            computeLnDetX();
        }
        
        if (useContrasts)
        {
            updateDagTraversal();
            initializeNodeToIndexContrastData();
        }

        updateTipPathsToRoot();
        updateAllNodePathsToRoot();
        updateCovariance();
        updateSampleCovariance();
        updateResiduals();
        updateRbCovarianceEigensystem();
    }
    
    cloned = true;
}

BrownianMotionAdmixtureGraph::~BrownianMotionAdmixtureGraph(void)
{
    
}

BrownianMotionAdmixtureGraph* BrownianMotionAdmixtureGraph::clone(void) const
{
    return new BrownianMotionAdmixtureGraph( *this );
}

/** Swap a parameter of the distribution */
void BrownianMotionAdmixtureGraph::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode< AdmixtureTree >* >( newP );
        //std::cout << "swapParameter() for Tau\n";
    }
    else if (oldP == diffusionRate)
    {
        diffusionRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == admixtureRate)
    {
        admixtureRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == branchRates)
    {
        branchRates = static_cast<const TypedDagNode< RbVector< double > >* >(newP);
    }
    else
    {
        ;
    }
}

void BrownianMotionAdmixtureGraph::setValue(ContinuousCharacterData *v)
{
    
}



double BrownianMotionAdmixtureGraph::computeLnProbability(void)
{
    if (useWishart == true)
        return likelihoodScaler * computeLnProbWishart();
    else if (useContrasts == true)
        return likelihoodScaler * computeLnProbContrasts();
    else
        return likelihoodScaler * computeLnProbComposite();
}

void BrownianMotionAdmixtureGraph::keepSpecialization(DagNode *affecter)
{
    /*
     
     // flags for nodes
     for (std::vector<bool>::iterator it = this->dirtyNodes.begin(); it != this->dirtyNodes.end(); ++it) {
        (*it) = false;
     }
     for (std::vector<bool>::iterator it = this->changedNodes.begin(); it != this->changedNodes.end(); ++it) {
        (*it) = false;
     }
     
     */
}

void BrownianMotionAdmixtureGraph::restoreSpecialization(DagNode *restorer)
{
    // hmm...
    
}

void BrownianMotionAdmixtureGraph::updateParameters(DagNode* toucher)
{
    if (toucher == tau || toucher == NULL)
    {
        initializeTipNodesByIndex();
        
        if (useContrasts == true)
        {
            updateAllNodePathsToRoot();
            updateDagTraversal();
            updateNodeToIndexContrastData();
        }
        else
        {
            updateTipPathsToRoot();
            //std::cout << toucher << "\n";
            //true;
        }
    }
    
    // return;
    if (toucher == diffusionRate || toucher == branchRates || toucher == tau || toucher == NULL) // toucher == admixtureRate ||
    {
        updateCovariance();
        updateSampleCovariance();
        if (useWishart == true)
            updateRbCovarianceEigensystem();
        updateResiduals();
    }

}

void BrownianMotionAdmixtureGraph::touchSpecialization(DagNode *toucher)
{
  /*
    //std::cout << "BMAG: touched by " << toucher->getName() << "\n";
    // only need to update Mrca for topology changes (FPNR, NNI, admixture edge moves)
    if (toucher == tau || toucher == NULL)
    {
        initializeTipNodesByIndex();
        
        if (useContrasts == true)
        {
            updateAllNodePathsToRoot();
            updateDagTraversal();
            updateNodeToIndexContrastData();
        }
        else
        {
            updateTipPathsToRoot();
        }
    }
    
   // return;
    if (toucher == diffusionRate || toucher == admixtureRate || toucher == branchRates || toucher == tau || toucher == NULL)
    {
        updateCovariance();
        updateSampleCovariance();
        if (useWishart == true)
            updateRbCovarianceEigensystem();
        updateResiduals();
    }
*/
    updateParameters(toucher);
    
    std::set<DagNode*> s = this->dagNode->getChildren();
    for (std::set<DagNode*>::iterator it = s.begin(); it != s.end(); it++)
    {
        (*it)->touch();
    }

}

void BrownianMotionAdmixtureGraph::redrawValue(void)
{
    ;
}

void BrownianMotionAdmixtureGraph::initializeData(void)
{
    // MJL: verified 03/20/13
    data.clear();
    data.resize(blockSize*numBlocks);
    
    // make sure tipData is matched by index, using getSnpFrequencies(name,idx)
    // std::vector<TopologyNode*> nodesByIndex = tau->getValue().getNodesByIndex();
    
    
//    for (int i = 0; i < numTaxa; i++)
//        std::cout << "check match " << snps->getPopulationNames(i) << " " << tau->getValue().getNode(i).getName() << "\n";
    
    for (unsigned i = 0; i < blockSize*numBlocks; i++)
    {
        data[i].resize(numTaxa,0.0);
        for (unsigned j = 0; j < numTaxa; j++)
        {
            double v = snps->getSnpFrequencies(j,i);
            data[i][j] = v;
        }
    }
    
    //std::cout << tau->getValue().getNewickRepresentation() << "\n";
    // std::cout << "dims\t" << data.size() << "\t" << data[0].size() << "\n";
    //std::cout << "data\n";
    //printR(data);
}

void BrownianMotionAdmixtureGraph::initializeMissingDataCorrection(void)
{
    missingDataCorrection.clear();
    missingDataCorrection.resize(numTaxa);
    
    for (size_t i = 0; i < numTaxa; i++)
        missingDataCorrection[i].resize(numTaxa,0.0);
    
    
    for (unsigned i = 0; i < blockSize*numBlocks; i++)
    {
        for (unsigned j = 0; j < numTaxa; j++)
        {
            if (snps->getNumSamples(j,i) == 0)
                continue;
            
            for (unsigned k = 0; k < numTaxa; k++)
            {
                if (snps->getNumSamples(k,i) != 0)
                    missingDataCorrection[j][k] += 1.0;
            }
        }
    }
    //std::cout << "pre-missingDataCorrection\n";
    //print(missingDataCorrection);

    
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = i; j < numTaxa; j++)
        {
            missingDataCorrection[i][j] = (double)(blockSize*numBlocks) / missingDataCorrection[i][j];
            missingDataCorrection[j][i] = missingDataCorrection[i][j];
        }
    }
    //std::cout << "missingDataCorrection\n";
    //print(missingDataCorrection);

}

// mu
void BrownianMotionAdmixtureGraph::initializeSampleMeans(void)
{
    // MJL: verified 03/20/13
    
    sampleMeans.clear();
    sampleMeans.resize(blockSize*numBlocks,0.0);
    
    for (size_t i = 0; i < blockSize*numBlocks; i++)
    {
        int nt = 0;
        for (size_t j = 0; j < numTaxa; j++)
        {
            //if (snps->getNumSamples(j,i) > 0)
            {
                sampleMeans[i] += data[i][j];
                nt++;
            }
            //std::cout << data[i][j] << "\t";
        }
        sampleMeans[i] /= nt;
        //sampleMeans[i] /= numTaxa;
        //std::cout << sampleMeans[i] << "\n";
    }
}

// B'
void BrownianMotionAdmixtureGraph::initializeSampleCovarianceBias(void)
{
    
    // compute mean bias per population
    std::vector<double> sampleMeanBias(numTaxa,0.0);
    
    //std::cout << "sample mean bias:   ";
    // use sample size for N_i,j, and mean N_i,j for bias
    std::vector<double> meanNumSamples(numTaxa,0.0);
    for (unsigned i =0; i < numTaxa; i++)
    {
        int ns = 0;
        for (unsigned j = 0; j < blockSize*numBlocks; j++)
        {
            double mns = (double)snps->getNumSamples(i,j);
            if (mns > 0)
                ns++;
            meanNumSamples[i] += mns;
        }
        //meanNumSamples[i] /= (blockSize*numBlocks);
        meanNumSamples[i] /= ns;
        //std::cout << meanNumSamples[i] << "\n";
    }
    
    for (unsigned i = 0; i < numTaxa; i++)
    {
        int ns = 0;
        // compute bias
        for (unsigned j = 0; j < blockSize*numBlocks; j++)
        {
            // exclude missing data
            if (snps->getNumSamples(i,j) > 0)
            {
                double N_i = snps->getNumSamples(i,j) / 2.0; // assume diploidy
                if (N_i == 0.5)
                {
                    std::cerr << "ERROR: " << snps->getPopulationNames(i) << " contains haploid data at locus " << j << "\n";
                    std::exit(0);
                }
                double Z_i = N_i * (2.0 * N_i - 1.0);
                double n_ik = 2.0 * N_i * data[j][i]; // convert allele frequency to allele count
                sampleMeanBias[i] += n_ik * (2.0 * N_i - n_ik) / Z_i;
                ns++;
                //std::cout << N_i << " " << Z_i << " " << n_ik << " " << sampleMeanBias[i] << " " << data[j][i] << "\n";
            }
            
        }
//        sampleMeanBias[i] /= (blockSize * numBlocks);
        sampleMeanBias[i] /= ns;
        sampleMeanBias[i] /= 2 * meanNumSamples[i]; // was 4....
    }
    //std::cout << "\n";
    
    // compute sample covariance estimator bias
    sampleCovarianceBias.clear();
    sampleCovarianceBias.resize(numTaxa);
    for (size_t i = 0; i < numTaxa; i++)
        for (size_t j = 0; j < numTaxa; j++)
            sampleCovarianceBias[i].resize(numTaxa,0.0);
    
    double b = 0.0;
    for (size_t k = 0; k < numTaxa; k++)
        b += sampleMeanBias[k];
    b /= (numTaxa * numTaxa);
    
    for (size_t i = 0; i < numTaxa; i++)
    {
        sampleCovarianceBias[i][i] += sampleMeanBias[i];
        
        for (size_t j = i; j < numTaxa; j++)
        {
            sampleCovarianceBias[i][j] -= ((sampleMeanBias[i] + sampleMeanBias[j]) / numTaxa);
            sampleCovarianceBias[i][j] += b;
            
            if (i != j)
                sampleCovarianceBias[j][i] = sampleCovarianceBias[i][j];
        }
    }
    
//    std::cout << "sampleCovarianceBias\n";
//    print(sampleCovarianceBias);
    ;
}

// W^
void BrownianMotionAdmixtureGraph::initializeSampleCovarianceEstimator(void)
{
    sampleCovarianceEstimator.clear();
    sampleCovarianceEstimator.resize(numBlocks);
    
    for (unsigned k = 0; k < numBlocks; k++)
    {
        sampleCovarianceEstimator[k].resize(numTaxa);
        for (size_t m = 0; m < numTaxa; m++)
            sampleCovarianceEstimator[k][m].resize(numTaxa,0.0);
        
        for (unsigned m = 0; m < numTaxa; m++)
        {
            for (unsigned n = m; n < numTaxa; n++)
            {
                double v = 0.0;
                int ns = 0;
                for (unsigned j = (k * blockSize); j < (k+1) * blockSize; j++)
                {
                    if (snps->getNumSamples(n,j) > 0 && snps->getNumSamples(m,j) > 0)
                    {
                        double v0 = (data[j][m] - sampleMeans[j]) * (data[j][n] - sampleMeans[j]);
                        v += v0;
                        ns++;
                    }
                   
                    //std::cout << k << "\t" << m << "\t" << n << "\t" << j << "\t" << data[j][m] << "\t" << data[j][n] << "\t" << sampleMeans[j] << "\t" << v0 << "\t" << v << "\n";
                }
                //std::cout << blockSize << " " << ns << "\n";
                //sampleCovarianceEstimator[k][m][n] = (v / blockSize);
                sampleCovarianceEstimator[k][m][n] = v / ns;
                sampleCovarianceEstimator[k][n][m] = v / ns;
            }
        }
        
        //std::cout << "sampleCovarianceEstimator[" << k << "]\n";
        //print(sampleCovarianceEstimator[k]);
    }
    
    // free memory
    std::vector<std::vector<double> > wipe;
    data.swap(wipe);
}

void BrownianMotionAdmixtureGraph::initializeMeanSampleCovarianceEstimator(void)
{
    meanSampleCovarianceEstimator.clear();
    meanSampleCovarianceEstimator.resize(numTaxa);
    
    for (size_t i = 0; i < numTaxa; i++)
    {
        meanSampleCovarianceEstimator[i].resize(numTaxa,0.0);
        for (size_t j = 0; j < numTaxa; j++)
        {
            for (size_t k = 0; k < numBlocks; k++)
            {
                meanSampleCovarianceEstimator[i][j] += sampleCovarianceEstimator[k][i][j];
            }
            meanSampleCovarianceEstimator[i][j] /= numBlocks;
            rbMeanSampleCovariance[i][j] = meanSampleCovarianceEstimator[i][j];
        }
    }

//    std::cout << "pre-bias meanSampleCovarianceEstimator\n";
//    print(meanSampleCovarianceEstimator);    
    
    /*
    if (useBias)
    {
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = 0; j < numTaxa; j++)
            {
                meanSampleCovarianceEstimator[i][j] -= sampleCovarianceBias[i][j];
            }
        }
    }
    */

    rbMeanSampleCovariance *= (numBlocks * blockSize);

//    std::cout << "meanSampleCovarianceEstimator\n";
//    print(meanSampleCovarianceEstimator);

}



// sigma^
void BrownianMotionAdmixtureGraph::initializeCompositeCovariance(void)
{
    compositeCovariance.clear();
    compositeCovariance.resize(numTaxa);
    
    for (size_t i = 0; i < numTaxa; i++)
    {
        compositeCovariance[i].resize(numTaxa,0.0);
        for (size_t j = 0; j < numTaxa; j++)
        {
            double v = 0.0;
            for (size_t k = 0; k < numBlocks; k++)
            {
                double w = meanSampleCovarianceEstimator[i][j] - sampleCovarianceEstimator[k][i][j];
                v += w * w;
            }
            compositeCovariance[i][j] = pow(v / (numBlocks * (numBlocks - 1)), 0.5);
        }
    }
    
    // free memory
    std::vector<std::vector<std::vector<double> > > wipe;
    sampleCovarianceEstimator.swap(wipe);
    
//    std::cout << "compositeCovariance\n";
//    print(compositeCovariance);
}

void BrownianMotionAdmixtureGraph::initializeSampleCovariance(void)
{
    sampleCovariance.clear();
    sampleCovariance.resize(numTaxa);
    
    for (size_t i = 0; i < numTaxa; i++)
        sampleCovariance[i].resize(numTaxa,0.0);
    
}

void BrownianMotionAdmixtureGraph::initializeCovariance(void)
{
    covariance.clear();
    covariance.resize(numTaxa);
    
    for (size_t i = 0; i < numTaxa; i++)
        covariance[i].resize(numTaxa,0.0);
    
    //print(covariance);
}

void BrownianMotionAdmixtureGraph::initializeResiduals(void)
{
    residuals.clear();
    residuals.resize(numTaxa);
    
    for (size_t i = 0; i < numTaxa; i++)
        residuals[i].resize(numTaxa,0.0);
}

void BrownianMotionAdmixtureGraph::initializeRbCovarianceEigensystem(void)
{
    ;
}

// V
void BrownianMotionAdmixtureGraph::updateCovariance(void)
{
    
    // get diffusion rate
    double sigma = diffusionRate->getValue(); // x(1-x)
    //double sigma_2 = sigma * sigma;
    
    // compute covariance (depends on pathsToRoot being current)
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = i; j < numTaxa; j++)
        {
            //double v = sqrt(findCovariance(tipNodesByIndex[i],tipNodesByIndex[j])) * sigma;
            double v = findCovariance(tipNodesByIndex[i],tipNodesByIndex[j]) * sigma;
            // MJL readd, change model VCV bias, not data directly...
            v += sampleCovarianceBias[i][j];
            covariance[i][j] = v;
            if (i != j)
                covariance[j][i] = v;
        }
    }
    
//    std::cout << "updateCovariance\n";
//    print(covariance);
    
    
}

// W
void BrownianMotionAdmixtureGraph::updateSampleCovariance(void)
{
    
    // (1/m^2) * sum(sum(V_k,k')) is a constant wrt any V_i,j
    double m_2 = 0.0;
    for (size_t i = 0; i < numTaxa; i++)
        for (size_t j = 0; j < numTaxa; j++)
            m_2 += covariance[i][j];
    m_2 /= (numTaxa * numTaxa);
  
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            double m = 0.0;
            for (size_t k = 0; k < numTaxa; k++)
                m -= (covariance[i][k] + covariance[j][k]);
            m /= numTaxa;
            double v = covariance[i][j] + m + m_2;
            sampleCovariance[i][j] = v;
        }
    }
    
    /*
    std::cout << "updateSampleCovariance\n";
    print(sampleCovariance);
     */
}

// R
void BrownianMotionAdmixtureGraph::updateResiduals(void)
{
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (useWishart == false)
                residuals[i][j] = meanSampleCovarianceEstimator[i][j] - sampleCovariance[i][j];
            else
                residuals[i][j] = meanSampleCovarianceEstimator[i][j] - sampleCovariance[i][j];
        }
    }
    //std::cout << "updateResiduals\n";
    //print(residuals);
}

void BrownianMotionAdmixtureGraph::initializeTipNodesByIndex(void)
{
//    std::cout << "init\n";
    tipNodesByIndex.clear();
    std::vector<TopologyNode*> n = tau->getValue().getNodes();
    size_t numTaxa = snps->getNumPopulations();
    for (size_t i = 0; i < numTaxa; i++)
        tipNodesByIndex.push_back(static_cast<AdmixtureNode*>(n[i]));
}

double BrownianMotionAdmixtureGraph::computeLnProbComposite(void)
{
    
    // RV is meanSampleCovarianceEstimator
    // mean is sampleCovariance
    // covariance is compositeCovariance
    
    double lnP = 0.0;
    
    double lnZ1 = -0.5 * log(2 * 3.14159265359);
    
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = i; j < numTaxa; j++)
        {
            // normalization constant
            double sigma = compositeCovariance[i][j];
            double lnZ2 = -log(sigma);
            
            // potential fn
            double x = (meanSampleCovarianceEstimator[i][j] - sampleCovariance[i][j]) / sigma;
            double lnX = -0.5 * x * x;
            
            lnP += lnZ1 + lnZ2 + lnX;
            
            // verify results with R
            // std::cout << "dnorm(" << meanSampleCovarianceEstimator[i][j] << "," << sampleCovariance[i][j] << "," << sigma << ",log=TRUE)\n" << lnX + lnZ1 + lnZ2 << "\n";
        }
    }
    //std::cout << "lnP\t" << lnP << "\n";
    
    return lnP;
}

double BrownianMotionAdmixtureGraph::computeLnDeterminant(EigenSystem* e)
{
    e->update();
    std::vector<double> ev = e->getRealEigenvalues();
    
    std::cout << "computeLnDeterminant\t";
    double lnDet = 1.0;
    for (size_t i = 0; i < ev.size(); i++)
    {
        std::cout << ev[i] << "\t";
        lnDet *= ev[i];
    }
    std::cout << "\n";
    return log(lnDet);
}



void BrownianMotionAdmixtureGraph::computeLnZWishart(void)
{
    // precompute, remains constant
    double lnZ1 = 0.5 * numBlocks * blockSize  * numTaxa * RbConstants::LN2;
    double lnZ2 = computeLnMvnGammaFn(numBlocks * blockSize/2, (int)numTaxa);
     
    // normalization constant (except for V)
    lnZWishart = lnZ1 + lnZ2;
}

double BrownianMotionAdmixtureGraph::computeLnMvnGammaFn(double a, int p)
{
    double lnZ1 = 0.25*p*(p-1)*log(RbConstants::PI);
    double lnZ2 = 0.0;
    for (int i = 1; i <= p; i++)
        lnZ2 += RbMath::lnGamma(a + (1.0 - i)/2.0);
    
    return lnZ1 + lnZ2;
}

#ifdef USE_LIB_ARMADILLO

void BrownianMotionAdmixtureGraph::eigenTest(arma::mat W)
{
    
    // check for positive definite
    arma::vec evalx, evalw;
    arma::mat evecx, evecw;
//    double eps = 1e-9;
    
    int p = W.n_rows;
    
    for (int i = 1; i < p; i++)
    {
        arma::mat X = W.submat( 0, 0, i, i );
        arma::eig_sym(evalx,evecx,X);
        for (int j = 0; j < i; j++)
        {
            if (evalx[j] < 0.0)
            {
                std::cout << i << " " << j << " " << evalx[j] << "\n\n";
                std::cout << W.submat(0,0,i,i) << "\n\n";
                ;
            }
        }
    }
}

double BrownianMotionAdmixtureGraph::computeLnProbWishart(void)
{
    //std::cout << "computeLnProbWishart\n";
    //return 0.0;

    int n = (int)(numBlocks*blockSize);
    int p = (int)numTaxa;
    
    arma::mat W(p,p);
    arma::mat X(p,p);
    //std::cout << W << "\n";
    
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < p; j++)
        {
            W.at(i,j) = sampleCovariance[i][j];
            X.at(i,j) = rbMeanSampleCovariance[i][j];
        }
        W.at(i,i) += regularizationFactor;
        X.at(i,i) += regularizationFactor;
        
        // prior on max pop size
        // W.at(i,i) += (1.0 / 1e3);
    }
    
//    std::cout << W << "\n";
//    std::cout << X << "\n";
    
    // drop smallest singular value using SVD
    if (true)
    {
        arma::mat Uprime(p,p);
        arma::mat U(p,p);
        arma::mat V(p,p);
        arma::vec s(p);
        
        arma::svd(U,s,V,X);
        Uprime = U;
        Uprime.shed_row(p-1);
        X = Uprime * X * arma::trans(Uprime);
        W = Uprime * W * arma::trans(Uprime);
        p -= 1;
    }
    
    
    // check for positive definite
    arma::vec evalx, evalw;
    arma::mat evecx, evecw;
    arma::eig_sym(evalx,evecx,X);
    arma::eig_sym(evalw,evecw,W);
    double eps = 1e-9;
    
    //std::cout << "evalx" << "\n" << evalx << "\n";
    //std::cout << "evecx" << "\n" << evecx << "\n";
    
    
    // round off small negative values, fail if proposed matrix is not positive definite (i.e. has any negative eigenvalue < -eps)
    evalx += eps;
    evalw += eps;
    
    for (unsigned i = 0; i < (unsigned)p; i++)
    {
        if (evalx.at(i) < 0.0)
        {
            std::cout << "evalx\t" << i << "\t" << evalx.at(i) << "\n";
            std::cout << "evalx\t" << evalx << "\n";
            return RbConstants::Double::neginf;
        }
        
        if (evalw.at(i) < 0.0)
        {
            
            //eigenTest(W);

            std::cout << "W\n" << W << "\n\n";
            //std::cout << "U\n" << U << "\n\n";
            //std::cout << "Uprime\n" << Uprime << "\n\n";
            
            std::cout << "evalw" << "\n" << evalw << "\n";
            std::cout << "evecw" << "\n" << evecw << "\n";
            std::cout << "bad evalw\t" << i << "\t" << evalw.at(i) << "\n";
            ;
            
            if (discardNonPosDefMtx)
                return RbConstants::Double::neginf;
            else
                evalw.at(i) = eps;
        }
        
        //    if (evalx.at(i) < 0.0 && evalx.at(i) > eps) evalx.at(i) = -eps;
        //    else if (evalx.at(i) < 0.0 && evalx.at(i) < eps) return RbConstants::Double::neginf;
        //    if (evalw.at(i) < 0.0 && evalw.at(i) > eps) evalw.at(i) = -eps;
        //    else if (evalw.at(i) < 0.0 && evalw.at(i) < eps) return RbConstants::Double::neginf;
    }
    
    W = evecw * arma::diagmat(evalw) * arma::trans(evecw);
    X = evecx * arma::diagmat(evalx) * arma::trans(evecx);
    
    // linear algebra
    double ldx, ldw;
    double signx, signw;
    arma::log_det(ldx,signx,X);
    arma::log_det(ldw,signw,W);
    
    //return 0.0;
    
    if (signw < 0) return RbConstants::Double::neginf;
    arma::mat Wi = arma::pinv(W);
    arma::mat WiX= Wi * X;
    arma::mat WiW = Wi * W;
    
    
    /*
     std::cout << "Wi * W check\t" << arma::sum(arma::sum(WiW)) / p << "\n\n";
     std::cout << "WiW\n" << WiW << "\n\n";
     std::cout << "Wi\n" << Wi << "\n\n";
     std::cout << "W\n" << W << "\n\n";
     std::cout << "X\n" << X << "\n\n";
     std::cout << "WiX\n" << WiX << "\n";
     std::cout << "ldx  " << ldx << " signx  " << signx << "\n";
     std::cout << "ldw  " << ldw << " signw  " << signw << "\n";
     */
    
    /*
     std::cout << "Needs[\"MultivariateStatistics`\"]\n";
     std::cout << "XX={";
     for (size_t i = 0; i < numTaxa; i++)
     {
     if (i != 0)
     std::cout << ",";
     std::cout << "{";
     for (size_t j = 0; j < numTaxa; j++)
     {
     if (j != 0)
     std::cout << ",";
     std::cout << X.at(i,j);
     }
     std::cout << "}";
     }
     std::cout << "}\n";
     
     std::cout << "WW={";
     for (size_t i = 0; i < numTaxa; i++)
     {
     if (i != 0)
     std::cout << ",";
     std::cout << "{";
     for (size_t j = 0; j < numTaxa; j++)
     {
     if (j != 0)
     std::cout << ",";
     std::cout << W.at(i,j);
     }
     std::cout << "}";
     }
     std::cout << "}\n";
     */
    
    // get lnP
    double lnZ1 = -0.5 * n * p * RbConstants::LN2; // OK
    double lnZ2 = -computeLnMvnGammaFn(.5 * n, p); // OK
    double lnDetW = -0.5 * n * ldw;
    double lnDetX2 = 0.5 * (n - p - 1) * ldx;
    double lnTr = -0.5 * arma::trace(WiX);
    double lnP = lnZ1 + lnZ2 + lnDetW + lnDetX2 + lnTr;
    
    
    /*
     std::cout << "--------------\n";
     std::cout << "n\t" << n << "\tp\t" << p << "\n";
     std::cout << "lnZ1\t" << lnZ1 << "\n";
     std::cout << "lnZ2\t" << lnZ2 << "\n";
     std::cout << "lnDetV\t" << lnDetW << "\n";
     std::cout << "lnDetX\t" << lnDetX2 << "\n";
     std::cout << "lnTr\t" << lnTr << "\n";
     std::cout << "lnP\t" << lnP << "\n";
     std::cout << "--------------\n";
     */
    
    /*
    double scaleFactorConstant = 2000; // make this smaller to flatten the likelihood surface
    double scaleFactor = numBlocks * blockSize / scaleFactorConstant;
    scaleFactor = 1;
    / scaleFactor;
    */
    return lnP;
    
}



void BrownianMotionAdmixtureGraph::computeLnDetX(void)
{
    // nS ~ Wishart(n-1,Sigma)
    //rbMeanSampleCovariance *= (blockSize * numBlocks);
    
    //for (int i = 0; i < numTaxa; i++)
    //    rbMeanSampleCovariance[i][i] += regularizationFactor;
    int p = (int)numTaxa;
    
    arma::mat tmp(p,p);
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
            tmp.at(i,j) = rbMeanSampleCovariance[i][j];
    double v = 0;
    double sgn = 0;
    arma::log_det(v,sgn,tmp);
    //std::cout << "logdet\t" << v << "\n";
    //std::cout << "det\t" << arma::det(tmp) << "\n";
    
    /*
     arma::vec armaEigenvalues(numTaxa);
     arma::mat armaEigenvectors(numTaxa,numTaxa);
     arma::eig_sym(armaEigenvalues, armaEigenvectors, tmp, "standard");
     std::cout << "eigenvalues\n";
     for (size_t i = 0; i < numTaxa; i++)
     std::cout << armaEigenvalues.at(i) << "\t";
     std::cout << "\n";
     
     
     std::cout << "before\n";
     for (size_t i = 0; i < numTaxa; i++)
     {
     for (size_t j = 0; j < numTaxa; j++)
     {
     std::cout << rbMeanSampleCovariance[i][j] << "\t";
     }
     std::cout << "\n";
     }
     */
    EigenSystem es(&rbMeanSampleCovariance);
    es.setRateMatrixPtr(&rbMeanSampleCovariance);
    es.update();
    double v2 = es.getDeterminant();
    
    /*
     std::cout << "after\n";
     for (size_t i = 0; i < numTaxa; i++)
     {
     for (size_t j = 0; j < numTaxa; j++)
     {
     std::cout << rbMeanSampleCovariance[i][j] << "\t";
     }
     std::cout << "\n";
     }
     
     std::cout << v2 << "\n";
     */
    lnDetX = log(v2);
}

void BrownianMotionAdmixtureGraph::svd(arma::mat& A, arma::mat& B, int idx)
{
    
    int p = (int)numTaxa;
    
    arma::mat Uprime(p,p);
    arma::mat U(p,p);
    arma::mat V(p,p);
    arma::vec s(p);
    
    arma::svd(U,s,V,A);
    Uprime = U;
    Uprime.shed_row(p-1);
    arma::mat Aprime = Uprime * A * arma::trans(Uprime);

    arma::svd(U,s,V,B);
    Uprime = U;
    Uprime.shed_row(p-1);
    arma::mat Bprime = Uprime * B * arma::trans(Uprime);
    
    
    //std::cout << "Uprime\n" << Uprime << "\n";
    std::cout << "Aprime\n" << Aprime << "\n";
    std::cout << arma::det(Aprime) << "\n";
    std::cout << "Bprime\n" << Bprime << "\n";
    
    
    /*
    arma::mat sol = U*arma::diagmat(s)*arma::trans(V);
    std::cout << "A\n" << A << "\n";
    std::cout << "U\n" << U << "\n";
    std::cout << "s\n" << s << "\n";
    std::cout << "V\n" << V << "\n";
    std::cout << "UsV^T\n" << sol << "\n";
     */
    
}


int BrownianMotionAdmixtureGraph::findFirstDuplicateIndex(arma::mat A)
{
    double eps = 1e-5;
    for (unsigned i = 0; i < numTaxa-1; i++)
    {
        for (unsigned j = i+1; j < numTaxa; j++)
        {
            bool duplicate = true;
            int ki = 0;
            int kj = 0;
            for (unsigned k = 0; k < numTaxa-1; k++)
            {
                if (k == i) ki = 1;
                if (k == j) kj = 1;
                
                //std::cout << i << "\t" << j << "\t" << k << "\t" << ki << "\t" << kj << "\t";
                // if one element differs, then the columns are not duplicates
                if (fabs(A.at(i,k+ki) - A.at(j,k+kj)) > eps)
                {
                    //  std::cout << "!d\n";
                    duplicate = false;
                    break;
                }
                //                std::cout << "\n";
            }
            if (duplicate == true)
                return i;
        }
    }
    return -1;
}


#else

// USE_LIB_ARMADILLO disabled
void BrownianMotionAdmixtureGraph::computeLnDetX(void) { ; }
double BrownianMotionAdmixtureGraph::computeLnProbWishart(void) { return 0.0; }

#endif

void BrownianMotionAdmixtureGraph::updateRbCovarianceEigensystem(void)
{
    //std::cout << "updating covariance eigensystem\n";
    // regularize
    
    //for (size_t i = 0; i < numTaxa; i++)
     //   rbCovariance[i][i] += regularizationFactor;
    
    // get the precision matrix
    rbCovarianceEigensystem.setRateMatrixPtr(&rbCovariance);
    rbCovarianceEigensystem.update();
    MatrixReal Q = rbCovarianceEigensystem.getEigenvectors();
    MatrixReal Qinv = rbCovarianceEigensystem.getInverseEigenvectors();
    std::vector<double> lambda = rbCovarianceEigensystem.getRealEigenvalues();
    MatrixReal Linv(numTaxa,numTaxa,0.0);
    for (size_t i = 0; i < numTaxa; i++)
    {
        //std::cout << lambda[i] << "\t";
        Linv[i][i] = 1.0 / lambda[i];
    }
//    std::cout << "\n";
    rbCovarianceInverse = Q * Linv * Qinv;
    
}

void BrownianMotionAdmixtureGraph::updateTipPathsToRoot(void)
{
    for (size_t i = 0; i < numTaxa; i++)
    {
        AdmixtureNode* p = tipNodesByIndex[i];
        std::list<AdmixtureNode*> path;
        std::set<std::list<AdmixtureNode*> > pathSet;
        findAllDagPathsToRoot(p, path, pathSet);
        pathsToRoot[p] = pathSet;
    }
}

void BrownianMotionAdmixtureGraph::updateAllNodePathsToRoot(void)
{
    pathsToRoot.clear();
    AdmixtureTree t = tau->getValue();
    
    for (size_t i = 0; i < t.getNumberOfNodes(); i++)
    {
        AdmixtureNode* p = const_cast<AdmixtureNode*>(&t.getNode(i));
        std::list<AdmixtureNode*> path;
        std::set<std::list<AdmixtureNode*> > pathSet;
        findAllDagPathsToRoot(p, path, pathSet);
        pathsToRoot[p] = pathSet;
    }
}


void BrownianMotionAdmixtureGraph::findAllDagPathsToRoot(AdmixtureNode* node, std::list<AdmixtureNode*> path, std::set<std::list<AdmixtureNode*> >& pathSet)
{
    
    // for (std::list<AdmixtureNode*>::iterator it = path.begin(); it != path.end(); it++)
    //     std::cout << *it << "\t";
    // std::cout << "\n";
    
    // if not at root, continue recording path towards root
    if (node != NULL)
    {
        path.push_front(node);
        
        // if at an admixture node, fork a new path to root
        AdmixtureNode* ap = &node->getAdmixtureParent();
        if (ap != NULL)
        {
            std::list<AdmixtureNode*> newPath(path);
            findAllDagPathsToRoot(ap, newPath, pathSet);
        }
        
        // if at a topology node, continue normally
        findAllDagPathsToRoot(&node->getParent(), path, pathSet);
    }
    
    // if at root, insert the path to the pathset
    else
        pathSet.insert(path);
}

double BrownianMotionAdmixtureGraph::findCovariance(AdmixtureNode* p, AdmixtureNode* q)
{
    
    // Step 1. get all dag paths to root
    std::set<std::list<AdmixtureNode*> > pPathSet = pathsToRoot[p];
    std::set<std::list<AdmixtureNode*> > qPathSet = pathsToRoot[q];
    
    std::set<std::list<AdmixtureNode*> >::const_iterator itp_path, itq_path;
    std::list<AdmixtureNode*>::const_iterator itp_step, itq_step, itp_prev, itq_prev;
    
    
    // Step 2. compute covariance structure

    //std::cout << tau->getValue().getRoot().getAge() << "\t" << tau->getValue().getTreeHeight() << "\n";
    double unitTreeScaler = 1.0; // / tau->getValue().getRoot().getAge();
    double vpq = 0.0;
    
    //std::cout << "paths\t" << p->getIndex() << " (" << pPathSet.size() << ")\t" << q->getIndex() << " (" << qPathSet.size() << ")\n";
    // compare all pairs of paths from root to tips of interest
    std::vector<double> br_val = branchRates->getValue();
    
    for (itp_path = pPathSet.begin(); itp_path != pPathSet.end(); itp_path++)
    {
        for (itq_path = qPathSet.begin(); itq_path != qPathSet.end(); itq_path++)
        {
            double wp = 1.0;
            double wq = 1.0;
            double u = 0.0;
            
            //std::cout << "\tsteps\n";
            // for each step in a path to p
            for (itp_step = itp_path->begin(), itp_prev = itp_path->end(); itp_step != itp_path->end(); itp_prev = itp_step, ++itp_step)
            {
                // branch rate (population size) parameter index
                size_t brIdx = 0;
                
                // skip first step (root node)
                if (itp_step == itp_path->begin())
                    continue;
                
                // this step leads to an admixture parent
                if ( &(*itp_step)->getAdmixtureChild() != NULL)
                {
                    brIdx = (*itp_step)->getTopologyChild(0).getIndex();
                }
                
                // this step leads to a divergence node
                else if ( &(*itp_step)->getAdmixtureParent() == NULL)
                {
                   // std::cout << "A\n";
                    brIdx = (*itp_step)->getIndex();
                }
                
                // this step leads to an admixture child, and the previous path step is its parent
                else if ( &(*itp_step)->getAdmixtureParent() == (*itp_prev) )
                {
                    //std::cout << "B\n";
                    wp *= (*itp_step)->getWeight();
                    brIdx = (*itp_step)->getTopologyChild(0).getIndex();
                }
                
                // this step is an admixture event, but the previous path step is a divergence node
                else if ( &(*itp_step)->getAdmixtureParent() != NULL )
                {
                    //std::cout << "C\n";
                    wp *= 1.0 - (*itp_step)->getWeight();
                    //brIdx = (*itp_step)->getIndex();
                    brIdx = (*itp_step)->getTopologyChild(0).getIndex();
                }
                //std::cout << "\tbrIdx\t" << brIdx << "\n";
                
                // accumulate unit divergence for paths shared by p and q
                if (find( itq_path->begin(), itq_path->end(), *itp_step) != itq_path->end())
                {
                    u += ((*itp_prev)->getAge() - (*itp_step)->getAge()) * unitTreeScaler * br_val[brIdx];
                    // std::cout << branchRates->getValue()[brIdx] << "\n";
                }
                
                //std::cout << "\t\t" << u << "\n";
                
            }
            
            for (itq_step = itq_path->begin(), itq_prev = itq_path->end(); itq_step != itq_path->end(); itq_prev = itq_step, ++itq_step)
            {
                // skip first step (root node)
                if (itq_step == itq_path->begin())
                    continue;
                
                // the previous step on this path is the admixture parent
                if ( &(*itq_step)->getAdmixtureParent() == (*itq_prev) )
                    wq *= (*itq_step)->getWeight();
                
                // the previous step on this path is the standing population
                else if ( &(*itq_step)->getAdmixtureParent() != NULL )
                    wq *= 1.0 - (*itq_step)->getWeight();

            }
            //std::cout << "\tstep sum\t" << u << "\n";
            //std::cout << "\tfactors\t" << wp << "\t" << wq << "\t" << unitTreeScaler << "\n";
            
            // accumuluate weighted mixture components
            vpq += u * wp * wq;
        }
        //std::cout << "path sum\t" << vpq << "\n\n";
    }
    
    return vpq;
    
}

///////////////////////////



double BrownianMotionAdmixtureGraph::computeLnProbContrasts(void)
{
    double lnP = 0.0;
    
    // fill pruning order for tree, ignoring admixture nodes
    AdmixtureTree t = tau->getValue();
    //AdmixtureNode* root = &t.getRoot();
    
    // pruning contrasts
    for (size_t i = 0; i < dagTraversalOrder.size(); i++)
    {
        
        // store tip data if needed
        AdmixtureNode* p = dagTraversalOrder[i];
        size_t idx_p = nodeToIndex[p];
        
        // skip tip nodes
        if (p->isTip())
            continue;

        // admixture parent, weight mean, pass variance
        else if (&p->getAdmixtureChild() != NULL)
        {
            AdmixtureNode* m = &p->getChild(0);
            AdmixtureNode* n = &p->getAdmixtureChild();
            double w = n->getWeight();
            
            size_t idx_m = m->getIndex();
            size_t idx_n = n->getIndex();
            double cv_mn = findCovariance(m,n);
            double v_m = findCovariance(m,m) - cv_mn;
            double v_n = findCovariance(n,n) - cv_mn;
            
            for (size_t j = 0; j < numSites; j++)
            {
                double x_n = contrastData[idx_n][j];
                double x_m = contrastData[idx_m][j];

                // first guess
                contrastData[idx_p][j] = (w*v_m*x_n + (1.0-w)*v_n*x_m) / (w*v_m + (1.0-w)*v_n);
            }
            
            // first guess
            contrastVar[idx_p] = ((w*v_m*(1.0-w)*v_n) / (w*v_m + (1.0-w)*v_n) + cv_mn);
        }
        // admixture child, pass mean, pass variance
        else if (&p->getAdmixtureParent() != NULL)
        {
            AdmixtureNode* m = &p->getTopologyChild(0);
            size_t idx_m = m->getIndex();
            contrastData[idx_p] = contrastData[idx_m];
            contrastVar[idx_p] = findCovariance(p, p);
        }
        // topology, weight mean, weight variance
        else
        {
            AdmixtureNode* m = &p->getTopologyChild(0);
            AdmixtureNode* n = &p->getTopologyChild(1);
            
            size_t idx_m = m->getIndex();
            size_t idx_n = n->getIndex();
            double cv_mn = findCovariance(m,n);
            double v_m = findCovariance(m,m) - cv_mn;
            double v_n = findCovariance(n,n) - cv_mn;
            // get delta, variance term for pruned node...
            
            // parallelize this bit later
            for (size_t j = 0; j < numSites; j++)
            {
                double x_n = contrastData[idx_n][j];
                double x_m = contrastData[idx_m][j];
                
                // compute node mean if bifurcation
                contrastData[idx_p][j] = (v_m*x_n + v_n*x_m) / (v_m + v_n);
                
            }
            contrastVar[idx_p] = ((v_m*v_n) / (v_m + v_n) + cv_mn);
        }
        
        
    }
    
    // pruningwise
    return lnP;
}

void BrownianMotionAdmixtureGraph::updateDagTraversal(void)
{
    dagTraversalOrder.clear();
    AdmixtureTree t = tau->getValue();
    passDagTraversal(&t.getRoot());
}

void BrownianMotionAdmixtureGraph::passDagTraversal(AdmixtureNode* p)
{
    AdmixtureNode* q;
 
    size_t nCh = p->getNumberOfChildren();
    for (size_t i = 0; i < nCh; i++)
    {
        q = &p->getChild(i);
        if (std::find(dagTraversalOrder.begin(),dagTraversalOrder.end(),q) == dagTraversalOrder.end())
            passDagTraversal(&p->getChild(i));
    }
    
    if (&p->getAdmixtureChild() != NULL)
    {
        q = &p->getAdmixtureChild();
        if (std::find(dagTraversalOrder.begin(),dagTraversalOrder.end(),q) == dagTraversalOrder.end())
            passDagTraversal(&p->getAdmixtureChild());
    }
    
    dagTraversalOrder.push_back(p);
}

void BrownianMotionAdmixtureGraph::updateNodeToIndexContrastData(void)
{
    
    // Non-admixture nodes retain AdmixtureNode*->int mapping, since they have a constant index.
    // Admixture nodes need to be updated, since their index changes depending on add/remove proposals.
    // numNodes equals the number of nodes, excluding admixture event nodes.
    // So, we only need to remap AdmixtureNode* -> int for nodes in (numNodes,t.getNumberOfNodes.size()-1).
    
    AdmixtureTree t = tau->getValue();
    for (unsigned i = numNodes; i < t.getNumberOfNodes(); i++)
    {
        AdmixtureNode* p = &t.getNode(i);
        p->setIndex(i); // can I just do this? ...
        nodeToIndex[p] = i;
    }
    
    // allocate more vectors if needed
    while (t.getNumberOfNodes() < contrastData.size())
        contrastData.push_back(std::vector<double>(numSites,0.0));
        
}

void BrownianMotionAdmixtureGraph::initializeNodeToIndexContrastData(void)
{
    // initialize twice as many contrastData vectors as there are topology nodes (pre-allocate memory)...
    contrastData = std::vector<std::vector<double> >(2*numNodes, std::vector<double>(numSites,0.0));
    contrastVar = std::vector<double>(numNodes, 0.0);
    
    AdmixtureTree t = tau->getValue();
    for (size_t i = 0; i < t.getNumberOfNodes(); i++)
    {
        AdmixtureNode* p = &t.getNode(i);
        nodeToIndex[p] = (int)p->getIndex();
        if (p->isTip())
            for (size_t j = 0; j < numSites; j++)
                contrastData[p->getIndex()][j] = data[j][i];
    }
}

////////////////////////////


//std::vector<double> BrownianMotionAdmixtureGraph::getResiduals(void) const
std::vector<std::vector<double> > BrownianMotionAdmixtureGraph::getResiduals(void) const
{
    return residuals;
}

double BrownianMotionAdmixtureGraph::getResiduals(int i, int j) const
{
    return residuals[i][j];
}

void BrownianMotionAdmixtureGraph::print(const std::vector<std::vector<double> >& x)
{
    for (size_t i = 0; i < x.size(); i++)
    {
        for (size_t j = 0; j < x[i].size(); j++)
        {
            if (j != 0)
                std::cout << "\t";
            std::cout << x[i][j];
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void BrownianMotionAdmixtureGraph::printR(const std::vector<std::vector<double> >& x)
{
    std::cout << "matrix(c(";
    for (size_t i = 0; i < x.size(); i++)
    {
        for (size_t j = 0; j < x[i].size(); j++)
        {
            if (j != 0 || i != 0)
                std::cout << ",";
            std::cout << x[i][j];
        }
    }
    std::cout << "),nrow=" << x[0].size() << ",ncol=" << x.size() << ")\n";
}

////////////////////////////////////////


#if 0

void BrownianMotionAdmixtureGraph::testGsl(void)
{
    // gsl test inverse
    gsl_matrix* gslCovariance = gsl_matrix_alloc (numTaxa, numTaxa);
    gsl_matrix* gslCovarianceInverse = gsl_matrix_alloc (numTaxa, numTaxa);
    gsl_matrix* gslCovarianceIdentity = gsl_matrix_alloc (numTaxa, numTaxa);
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (i == j) gsl_matrix_set(gslCovarianceIdentity, i, j, 1.0);
            else gsl_matrix_set(gslCovarianceIdentity, i, j, 0.0);
            gsl_matrix_set(gslCovariance, i, j, rbCovariance[i][j]);
        }
    }
    //    gsl_matrix_view m = gsl_matrix_view_array (a_data, 4, 4);
    //    gsl_vector_view b = gsl_vector_view_array (b_data, 4);
    //    gsl_vector *x = gsl_vector_alloc (4);
    
    int s;
    gsl_permutation* p = gsl_permutation_alloc (numTaxa);
    gsl_linalg_LU_decomp (gslCovariance, p, &s);
    gsl_linalg_LU_invert(gslCovariance, p, gslCovarianceInverse);
    gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.0, gslCovariance, gslCovarianceInverse, 0.0, gslCovarianceIdentity);
    
    std::stringstream gslc;
    std::cout << "gslCovariance\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                gslc << ",";
            std::cout << gsl_matrix_get(gslCovariance, i, j) << "\t";
            gslc << gsl_matrix_get(gslCovariance, i, j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << gslc.str() << ")\n";
    std::cout << "[" << gslc.str() << "]\n";
    std::cout << "\n";
    
    
    
    std::stringstream gslinv;
    std::cout << "gslCovarianceInverse\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                gslinv << ",";
            std::cout << gsl_matrix_get(gslCovarianceInverse, i, j) << "\t";
            gslinv << gsl_matrix_get(gslCovarianceInverse, i, j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << gslinv.str() << ")\n";
    std::cout << "[" << gslinv.str() << "]\n";
    std::cout << "\n";
    
    
    std::stringstream gslid;
    std::cout << "gslCovarianceIdentity\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                gslid << ",";
            std::cout << gsl_matrix_get(gslCovarianceIdentity, i, j) << "\t";
            gslid << gsl_matrix_get(gslCovarianceIdentity, i, j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << gslid.str() << ")\n";
    std::cout << "[" << gslid.str() << "]\n";
    std::cout << "\n";
    
}

void BrownianMotionAdmixtureGraph::testEigen(void)
{
    Eigen::MatrixXd eiIdentity(numTaxa,numTaxa);
    Eigen::MatrixXd eiCovariance(numTaxa,numTaxa);
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            eiCovariance(i,j) = rbCovariance[i][j];
            eiIdentity(i,j) = 0.0;
        }
    }
    if (!eiCovariance.fullPivLu().isInvertible())
        std::cout << "ERROR: not invertible\n";
    //    Eigen::MatrixXd eiCovarianceInverse = eiCovariance.fullPivLu().solve(eiIdentity);
    //    Eigen::MatrixXd eiCovarianceInverse = eiCovariance.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(eiIdentity);
    Eigen::MatrixXd eiCovarianceInverse = eiCovariance.inverse();
    Eigen::MatrixXd eiCovarianceIdentity = eiCovariance * eiCovarianceInverse;
    
    std::stringstream eic;
    std::cout << "eiCovariance\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                eic << ",";
            std::cout << eiCovariance(i,j) << "\t";
            eic << eiCovariance(i,j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << eic.str() << ")\n";
    std::cout << "[" << eic.str() << "]\n";
    std::cout << "\n";
    
    
    std::stringstream eici;
    std::cout << "eiCovarianceInverse\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                eici << ",";
            std::cout << eiCovarianceInverse(i,j) << "\t";
            eici << eiCovarianceInverse(i,j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << eici.str() << ")\n";
    std::cout << "[" << eici.str() << "]\n";
    std::cout << "\n";
    
    
    std::stringstream eicid;
    std::cout << "eiCovarianceIdentity\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                eicid << ",";
            std::cout << eiCovarianceIdentity(i,j) << "\t";
            eicid << eiCovarianceIdentity(i,j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << eicid.str() << ")\n";
    std::cout << "[" << eicid.str() << "]\n";
    std::cout << "\n";
    
    
    
    
}

void BrownianMotionAdmixtureGraph::testOpenCv(void)
{
    // openCV test inverse
    cv::Mat cvCovariance(numTaxa, numTaxa, cv::DataType<float>::type);
    for (size_t i = 0; i < numTaxa; i++)
        for (size_t j = 0; j < numTaxa; j++)
            cvCovariance.at<float>(i,j) = rbCovariance[i][j];
    cv::Mat cvCovarianceInverse(cvCovariance.inv(cv::DECOMP_SVD));
    cvCovarianceInverse = cv::Mat(numTaxa, numTaxa, cv::DataType<float>::type);
    
    //    cvCovarianceIdentity = cv::Mat::eye(numTaxa, numTaxa, cv::DataType<float>::type);
    //    cv::solve(cvCovariance, cvCovarianceIdentity, cvCovarianceInverse);
    cv::SVD svd(cvCovariance);
    cvCovarianceInverse = svd.vt.t()*cv::Mat::diag(1./svd.w)*svd.u.t();
    cv::Mat cvCovarianceIdentity(cvCovariance*cvCovarianceInverse);
    
    
    std::stringstream cvi;
    std::cout << "cvCovarianceInverse\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                cvi << ",";
            std::cout << cvCovarianceInverse.at<float>(i,j) << "\t";
            cvi << cvCovarianceInverse.at<float>(i,j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << cvi.str() << ")\n";
    std::cout << "[" << cvi.str() << "]\n";
    std::cout << "\n";
    
    
    std::stringstream cvid;
    std::cout << "cvCovarianceIdentity\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                cvid << ",";
            std::cout << cvCovarianceIdentity.at<float>(i,j) << "\t";
            cvid << cvCovarianceIdentity.at<float>(i,j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << cvid.str() << ")\n";
    std::cout << "[" << cvid.str() << "]\n";
    std::cout << "\n";
    
}

void BrownianMotionAdmixtureGraph::testArma(void)
{
    //////////////////////////////////
    
    // armadillo test inverse
    arma::mat armaIdentity2(numTaxa,numTaxa);
    armaIdentity2 = armaIdentity2.eye();
    arma::mat armaCovariance(numTaxa,numTaxa);
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            armaCovariance.at(i,j) = rbCovariance[i][j];
        }
    }
    //arma::mat armaCovarianceInverse = arma::solve(armaCovariance,armaIdentity2,true);
    //arma::mat armaCovarianceInverse = arma::pinv(armaCovariance);//, arma::norm(armaCovariance,2)*arma::datum::eps*numTaxa);
    arma::mat armaCovarianceInverse = arma::inv(armaCovariance, true);
    arma::mat armaIdentity = armaCovariance * armaCovarianceInverse;
    
    arma::mat L(numTaxa,numTaxa);
    arma::mat U(numTaxa,numTaxa);
    arma::lu(L, U, armaCovariance);
    
    
    std::stringstream aciss;
    std::cout << "armaCovarianceInverse\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                aciss << ",";
            std::cout << armaCovarianceInverse.at(i,j) << "\t";
            aciss << armaCovarianceInverse.at(i,j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << aciss.str() << ")\n";
    std::cout << "[" << aciss.str() << "]\n";
    std::cout << "\n";
    
    
    std::stringstream ai;
    std::cout << "armaCovarianceIdentity\tSigma^-1\n";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                ai << ",";
            std::cout << armaIdentity.at(i,j) << "\t";
            ai << armaIdentity.at(i,j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "c(" << ai.str() << ")\n";
    std::cout << "[" << ai.str() << "]\n";
    std::cout << "\n";
    
}


void BrownianMotionAdmixtureGraph::testWishart(void)
{
    
    // verified against MCMCpack's dwish
    
    int n = 5;
    int p = 2;
    
    arma::mat S(p,p); S = "1 .3; .3 1";
    arma::mat X(p,p); X = "6.058958 4.290973; 4.290973 7.885960";
    
    double lnZ1 = -0.5 * n * p * RbConstants::LN2; // OK
    double lnZ2 = -computeLnMvnGammaFn(.5*n, p); // OK
    double lnDetV = -0.5 * n * log(arma::det(S)); // OK
    double lnDetX = 0.5 * (n - p - 1) * log(arma::det(X)); // OK
    double lnTr = -0.5 * arma::trace(arma::pinv(S) * X); // OK
    
    double lnP = lnDetX + lnDetV + lnTr + lnZ1 + lnZ2;
    std::cout << "lnZ1\t" << lnZ1 << "\n";
    std::cout << "lnZ2\t" << lnZ2 << "\n";
    std::cout << "lnDetV\t" << lnDetV << "\n";
    std::cout << "lnDetX\t" << lnDetX << "\n";
    std::cout << "lnTr\t" << lnTr << "\n";
    std::cout << "lnL\t" << lnP << "\n";
    std::cout << "L\t" << exp(lnP) << "\n";
    
}


double BrownianMotionAdmixtureGraph::computeLnProbWishart2(void)
{
    double lnP = 0.0;
    
    for (size_t i = 0; i < numTaxa; i++)
        for (size_t j = 0; j < numTaxa; j++)
            rbCovariance[i][j] = sampleCovariance[i][j];
    
    //testArma();
    //testGsl();
    //testOpenCv();
    //testEigen();
    
    // armadillo test inverse
    arma::mat armaIdentity(numTaxa,numTaxa);
    arma::mat armaIdentitySolver(numTaxa,numTaxa);
    armaIdentitySolver.eye();
    arma::mat armaSampleCovariance(numTaxa,numTaxa);
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            armaSampleCovariance.at(i,j) = sampleCovariance[i][j];
        }
        armaSampleCovariance.at(i,i) += regularizationFactor;
    }
    arma::mat armaSampleCovarianceInverse = arma::solve(armaSampleCovariance,armaIdentitySolver,true);
    //arma::mat armaCovarianceInverse = arma::pinv(armaCovariance);//, arma::norm(armaCovariance,2)*arma::datum::eps*numTaxa);  // NOTE: pinv doesn't seem to work using default settings
    //arma::mat armaCovarianceInverse = arma::inv(armaCovariance, true); // works...
    armaIdentity = armaSampleCovariance * armaSampleCovarianceInverse;
    
    arma::vec armaEigenvalues(numTaxa);
    arma::mat armaEigenvectors(numTaxa,numTaxa);
    arma::eig_sym(armaEigenvalues, armaEigenvectors, armaSampleCovariance, "standard");
    
    
    
    //std::cout << "eigenvalues\n"; for (size_t i = 0; i < numTaxa; i++) std::cout << armaEigenvalues.at(i,i) << "\t"; std::cout << "\n";
    
    std::stringstream mscss;
    std::cout << "rbMeanSampleCovariance\t";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            //std::cout << rbMeanSampleCovariance[i][j] << "\t";
            if (j != 0 || i != 0)
                mscss << ",";
            mscss << rbMeanSampleCovariance[i][j];
        }
        //std::cout << "\n";
    }
    //    std::cout << "\n";
    std::cout << "scatter=matrix(c(" << mscss.str() << "),nrow=" << numTaxa << ")\n";
    //  std::cout << "[" << mscss.str() << "]\n";
    //    std::cout << "\n";
    
    
    std::stringstream aciss;
    //std::cout << "armaSampleCovariance\t";
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0 || i != 0)
                aciss << ",";
            //std::cout << armaSampleCovarianceInverse.at(i,j) << "\t";
            aciss << armaSampleCovariance.at(i,j);
        }
        //        std::cout << "\n";
    }
    //  std::cout << "\n";
    // std::cout << "sigma=matrix(c(" << aciss.str() << "),nrow=" << numTaxa << ")\n";
    // std::cout << "dwish(scatter," << blockSize*numBlocks << ",sigma)\n";
    //    std::cout << "[" << aciss.str() << "]\n";
    //  std::cout << "\n";
    
    /*
     std::stringstream ai;
     std::cout << "armaCovarianceIdentity\tSigma^-1\n";
     for (size_t i = 0; i < numTaxa; i++)
     {
     for (size_t j = 0; j < numTaxa; j++)
     {
     if (j != 0 || i != 0)
     ai << ",";
     std::cout << armaIdentity.at(i,j) << "\t";
     ai << armaIdentity.at(i,j);
     }
     std::cout << "\n";
     }
     std::cout << "\n";
     std::cout << "c(" << ai.str() << ")\n";
     std::cout << "[" << ai.str() << "]\n";
     std::cout << "\n";
     */
    
    
    // (nS/2)*ln(det(V))
    double lnDetV = 0.0;
    //lnDetV = computeLnDeterminant(&rbCovarianceEigensystem);
    //std::cout << "lnDetV\t" << lnDetV << "\n";
    double sign;
    arma::log_det(lnDetV,sign,armaSampleCovariance);
    //lnDetV = log(arma::det(armaCovarianceInverse));
    //    std::cout << "lnDetV\t" << lnDetV << "\t" << sign << "\n";
    double lnV = 0.5 * numBlocks * blockSize * lnDetV;
    // ... verified
    
    // -((nS*nT/2)*ln(2) + ln(gamma_nT(nS/2))
    // stored in lnZWishart
    // ... verified
    
    // ... normalization constant, anyways
    
    // (1/2)*(nS-nT-1)*ln(det(X))
    // double lnX = 0.5 * (numSites - numTaxa - 1) * lnDetX;
    double lnX = 0.0;
    // ... wonky, but a normalization constant, so dropping it should not matter
    
    // tr(V^-1*X)
    double tr = 0.0;
    for (size_t i = 0; i < numTaxa; i++)
        for (size_t j = 0; j < numTaxa; j++)
            tr += armaSampleCovariance.at(i,j) * rbMeanSampleCovariance[j][i];
    //tr += rbCovarianceInverse[i][j] * rbMeanSampleCovariance[j][i];
    
    // ln(e^(-0.5*tr))
    double lnTr = -0.5 * tr;
    // ... verified
    
    // lnP
    // lnP = lnX + lnTr - lnV - lnZWishart;
    lnP = lnX + lnTr - lnV - lnZWishart;
    
    if (true)
    {
        
        std::stringstream mscss;
        std::cout << "rbMeanSampleCovariance\tX\n";
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = 0; j < numTaxa; j++)
            {
                std::cout << rbMeanSampleCovariance[i][j] << "\t";
                if (j != 0 || i != 0)
                    mscss << ",";
                mscss << rbMeanSampleCovariance[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout << "c(" << mscss.str() << ")\n";
        std::cout << "[" << mscss.str() << "]\n";
        std::cout << "\n";
        
        std::stringstream css;
        std::cout << "rbCovariance\tSigma\n";
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = 0; j < numTaxa; j++)
            {
                if (j != 0 || i != 0)
                    css << ",";
                std::cout << rbCovariance[i][j] << "\t";
                css << rbCovariance[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout << "c(" << css.str() << ")\n";
        std::cout << "[" << css.str() << "]\n";
        std::cout << "\n";
        
        std::stringstream ciss;
        std::cout << "rbCovarianceInverse\tSigma^-1\n";
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = 0; j < numTaxa; j++)
            {
                if (j != 0 || i != 0)
                    ciss << ",";
                std::cout << rbCovarianceInverse[i][j] << "\t";
                ciss << rbCovarianceInverse[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout << "c(" << ciss.str() << ")\n";
        std::cout << "[" << ciss.str() << "]\n";
        std::cout << "\n";
        
        std::stringstream cidss;
        MatrixReal rbCovarianceIdentity = rbCovariance * rbCovarianceInverse;
        std::cout << "rbCovarianceIdentity\tSigma*Sigma^-1\n";
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = 0; j < numTaxa; j++)
            {
                if (j != 0 || i != 0)
                    cidss << ",";
                std::cout << rbCovarianceIdentity[i][j] << "\t";
                cidss << rbCovarianceIdentity[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout << "c(" << cidss.str() << ")\n";
        std::cout << "[" << cidss.str() << "]\n";
        std::cout << "\n";
    }
    if (true)
    {
        
        
        
        
        // std::cout << "dwish(matrix(c(" << mscss.str() << "),nrow=" << numTaxa << "),";
        //    std::cout << (numBlocks*blockSize) << ",";
        //   std::cout << "matrix(c(" << css.str() << "),nrow=" << numTaxa << "))\n";
        
        
        /*
         std::cout << "numSites\t" << (blockSize*numBlocks) << "\tnumTaxa\t" << numTaxa << "\n";
         std::cout << "lnZWishart\t" << lnZWishart << "\n";
         std::cout << "lnDetV\t" << lnDetV << "\n";
         std::cout << "lnV\t" << lnV << "\n";
         std::cout << "lnDetX\t" << lnDetX << "\n";
         std::cout << "lnX\t" << lnX << "\n";
         std::cout << "lnTr\t" << lnTr << "\n";
         std::cout << "lnP\t" << lnP << "\n";
         std::cout << "\n\n";
         */
    }
    
    return lnP;
}

#endif
