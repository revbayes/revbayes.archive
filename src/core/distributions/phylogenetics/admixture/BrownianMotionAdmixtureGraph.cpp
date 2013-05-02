
//  BrownianMotionAdmixtureGraph.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "BrownianMotionAdmixtureGraph.h"
#include "DistributionExponential.h"
#include "DynamicNode.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbStatisticsHelper.h"
#include "SnpData.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "TopologyNode.h"
#include "TreeChangeEventListener.h"
#include <list>
#include <algorithm>

using namespace RevBayesCore;

BrownianMotionAdmixtureGraph::BrownianMotionAdmixtureGraph(const TypedDagNode<AdmixtureTree> *t, const TypedDagNode<double> *dr, const TypedDagNode<double> *ar, const TypedDagNode< std::vector< double > >* br, SnpData* s) :
TypedDistribution<CharacterData<ContinuousCharacterState> >( new CharacterData<ContinuousCharacterState>() ),
tau(t),
diffusionRate(dr),
admixtureRate(ar),
branchRates(br),
//residualsFn(res),
snps(s),
numSites(s->getNumSnps()),
numTaxa(s->getNumPopulations()),
numNodes(2 * s->getNumPopulations() -1),
blockSize(500), // humans == 500
//numBlocks(numSites/blockSize),
//tipData(snps->getSnpFrequencies()),
//covarianceMatrix(numTaxa,numTaxa),
//covarianceInverse(numTaxa,numTaxa)
//covarianceBias(numTaxa,numTaxa),
//covarianceEigensystem(&covarianceMatrix),
cloned(false)
{
    numBlocks = numSites / blockSize;
    
    // add model parameters
    this->addParameter( tau );
    this->addParameter( diffusionRate );
    this->addParameter( admixtureRate );
    this->addParameter( branchRates );
    
    std::cout << "numSites\t" << numSites << "\n";
    std::cout << "numBlocks\t" << numBlocks << "\n";
    std::cout << "blockSize\t" << blockSize << "\n";
    
    //delete this->value;
    //this->value = simulate(this->tau->getValue().getRoot());
    
    //covarianceMatrix = MatrixReal(numTaxa,numTaxa,0.0);
    //covarianceInverse = MatrixReal(numTaxa,numTaxa,0.0);
    //covarianceBias = MatrixReal(numTaxa,numTaxa,0.0);
    //covarianceEigensystem = EigenSystem(&covarianceMatrix);
    
    // initializeTipData();
    
}


BrownianMotionAdmixtureGraph::BrownianMotionAdmixtureGraph(const BrownianMotionAdmixtureGraph &n) :
TypedDistribution<CharacterData<ContinuousCharacterState> > ( new CharacterData<ContinuousCharacterState>() ),
tau(n.tau),
diffusionRate(n.diffusionRate),
admixtureRate(n.admixtureRate),
branchRates(n.branchRates),
//residualsFn(n.residualsFn),
snps(n.snps),
numSites(n.numSites),
numTaxa(n.numTaxa),
numNodes(n.numNodes),
blockSize(n.blockSize),
numBlocks(n.numBlocks),
tipNodesByIndex(n.tipNodesByIndex),
pathsToRoot(n.pathsToRoot),
//tipData(n.tipData),
//covarianceMatrix(n.covarianceMatrix),
//covarianceInverse(n.covarianceInverse),
//covarianceBias(n.covarianceBias),
//covarianceEigensystem(n.covarianceEigensystem),
cloned(n.cloned)
{
    // calling this method again is required despite the constructor initializer -- interacts poorly w/ vector<MatrixReal>, I reckon
    
    // dirty hack -- only initialize when object is cloned for Mcmc... will ask SH about how to do this elegantly
    if (cloned)
    {
        // MVN setup
        // initializeTipData();
        
        // composite likelihood setup
        initializeTipNodesByIndex();
        initializeData();
        initializeSampleMeans();
        initializeSampleCovarianceBias();
        initializeSampleCovarianceEstimator();
        initializeMeanSampleCovarianceEstimator();
        initializeCompositiveCovariance();
        initializeCovariance();
        initializeSampleCovariance();
        initializeResiduals();

        
        updatePathsToRoot();
        updateCovariance();
        updateSampleCovariance();
        updateResiduals();
       
        
        // initializeMrca();
        //updateMrca();
        //updateDagTraversal();
        //updateDagVariance();
        
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

void BrownianMotionAdmixtureGraph::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == tau) {
        tau = static_cast<const TypedDagNode< AdmixtureTree >* >( newP );
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
        branchRates = static_cast<const TypedDagNode< std::vector< double > >* >(newP);
    }
    else
    {
        ;
    }
}

void BrownianMotionAdmixtureGraph::setValue(CharacterData<ContinuousCharacterState> *v)
{
    
}



double BrownianMotionAdmixtureGraph::computeLnProbability(void)
{
    
   // updateCovariance();
   // updateSampleCovariance();
    //return 0.0;
    //std::cout << "BMAG::compLnProb\n";
    return computeLnProbComposite();
 
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
    
}

void BrownianMotionAdmixtureGraph::touchSpecialization(DagNode *toucher)
{
    // only need to update Mrca for topology changes (FPNR, NNI, admixture edge moves)
    if (toucher == tau)
    {
        initializeTipNodesByIndex();
        updatePathsToRoot();
    }
    
   // return;
    if (toucher == diffusionRate || toucher == admixtureRate || toucher == branchRates || toucher == tau)
    {
        updateCovariance();
        updateSampleCovariance();
        updateResiduals();
    }

    // touches all children
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
    
    data.clear();
    data.resize(blockSize*numBlocks);
    
    // make sure tipData is matched by index, using getSnpFrequencies(name,idx)
//    std::vector<TopologyNode*> nodesByIndex = tau->getValue().getNodesByIndex();
    
    for (size_t i = 0; i < blockSize*numBlocks; i++)
    {
        data[i].resize(numTaxa,0.0);
        for (size_t j = 0; j < numTaxa; j++)
        {
            data[i][tipNodesByIndex[j]->getIndex()] = snps->getSnpFrequencies(tipNodesByIndex[j]->getName(),i);
            //std::cout << i << "\t" << tipNodesByIndex[j]->getIndex() << "\t" << data[i][tipNodesByIndex[j]->getIndex()] << "\n";
        }
    }
    
    //std::cout << tau->getValue().getNewickRepresentation() << "\n";
    //std::cout << "dims\t" << data.size() << "\t" << data[0].size() << "\n";
}

// mu
void BrownianMotionAdmixtureGraph::initializeSampleMeans(void)
{
    sampleMeans.clear();
    sampleMeans.resize(blockSize*numBlocks,0.0);
    
    for (size_t i = 0; i < blockSize*numBlocks; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            sampleMeans[i] += data[i][j];
            //std::cout << data[i][j] << "\t";
        }
        sampleMeans[i] /= numTaxa;
        //std::cout << sampleMeans[i] << "\n";
    }
}

// B'
void BrownianMotionAdmixtureGraph::initializeSampleCovarianceBias(void)
{
    
    // compute mean bias per population
    std::vector<double> sampleMeanBias(numTaxa,0.0);
    for (size_t i = 0; i < numTaxa; i++)
    {
        // compute bias
        int N_i = snps->getNumIndividuals(i) / 2; // num individuals, not chromosomes...
        int Z_i = N_i * (2 * N_i - 1);
        for (size_t j = 0; j < blockSize*numBlocks; j++)
        {
            double n_ik = 2 * N_i * data[j][i];
            sampleMeanBias[i] += n_ik * (2 * N_i - n_ik) / Z_i;
        }
        sampleMeanBias[i] /= blockSize * numBlocks;
        sampleMeanBias[i] /= 4 * N_i;
    }
    
    // compute sample covariance estimator bias
    sampleCovarianceBias.clear();
    sampleCovarianceBias.resize(numTaxa);
    for (size_t i = 0; i < numTaxa; i++)
        for (size_t j = 0; j < numTaxa; j++)
            sampleCovarianceBias[i].resize(numTaxa,0.0);
    
    for (size_t i = 0; i < numTaxa; i++)
    {
        sampleCovarianceBias[i][i] += sampleMeanBias[i];
        
        for (size_t j = i; j < numTaxa; j++)
        {
            sampleCovarianceBias[i][j] -= (sampleMeanBias[i] + sampleMeanBias[j]) / numTaxa;
            
            double b = 0.0;
            for (size_t k = 0; k < numTaxa; k++)
                b += sampleMeanBias[k];
            b /= (numTaxa * numTaxa);
            
            sampleCovarianceBias[i][j] += b;
            
            if (i != j)
                sampleCovarianceBias[j][i] = sampleCovarianceBias[i][j];
        }
    }
    
    //std::cout << "sampleCovarianceBias\n";
    //print(sampleCovarianceBias);
}

// W^
void BrownianMotionAdmixtureGraph::initializeSampleCovarianceEstimator(void)
{
    sampleCovarianceEstimator.clear();
    sampleCovarianceEstimator.resize(numBlocks);
    
    for (size_t k = 0; k < numBlocks; k++)
    {
        sampleCovarianceEstimator[k].resize(numTaxa);
        for (size_t m = 0; m < numTaxa; m++)
        {
            sampleCovarianceEstimator[k][m].resize(numTaxa,0.0);
            for (size_t n = 0; n < numTaxa; n++)
            {
                double v = 0.0;
                for (size_t j = k * blockSize; j < (k+1) * blockSize; j++)
                {
                    v += (data[j][m] - sampleMeans[j]) * (data[j][n] - sampleMeans[j]);
                    //std::cout << k << "\t" << m << "\t" << n << "\t" << j << "\t" << data[j][m] << "\t" << data[j][n] << "\t" << sampleMeans[j] << "\t" << v << "\n";
                }
                sampleCovarianceEstimator[k][m][n] = (v / blockSize);
            }
        }
        
        //std::cout << "sampleCovarianceEstimator[" << k << "]\n";
        //print(sampleCovarianceEstimator[k]);
    }
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
            meanSampleCovarianceEstimator[i][j] -= sampleCovarianceBias[i][j];
        }
    }
    
    //std::cout << "meanSampleCovarianceEstimator\n";
    //print(meanSampleCovarianceEstimator);
}

// sigma^
void BrownianMotionAdmixtureGraph::initializeCompositiveCovariance(void)
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
    
    //std::cout << "compositeCovariance\n";
    //print(compositeCovariance);
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
    
}

void BrownianMotionAdmixtureGraph::initializeResiduals(void)
{
 
    residuals.clear();
    residuals.resize(numTaxa);
    for (size_t i = 0; i < numTaxa; i++)
        residuals[i].resize(numTaxa,0.0);
    /*
    size_t sz = 0;
    for (size_t i = 0; i < numTaxa; i++)
        sz += i + 1;
    residuals.resize(sz);
     */
}

// V
void BrownianMotionAdmixtureGraph::updateCovariance(void)
{
    
    // get diffusion rate
    double sigma = pow(diffusionRate->getValue(),1.0);
    sigma *= sigma;
    //sigma = 1.0;
    
    // compute covariance (depends on pathsToRoot being current)
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = i; j < numTaxa; j++)
        {
            double v = findCovariance(tipNodesByIndex[i],tipNodesByIndex[j]) * sigma;
            covariance[i][j] = v;
            if (i != j)
                covariance[j][i] = covariance[i][j];
        }
    }
    
    //std::cout << "updateCovariance\n";
    //print(covariance);
    
}

// W
void BrownianMotionAdmixtureGraph::updateSampleCovariance(void)
{
    
    // (1/m^2) * sum(sum(V_k,k')) is a constant wrt any V_i,j
    double m_2 = 0.0;
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            m_2 += covariance[i][j];
        }
    }
    m_2 /= (numTaxa * numTaxa);
  
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            double m = 0.0;
            for (size_t k = 0; k < numTaxa; k++)
                m -= covariance[i][k] + covariance[j][k];
            m /= numTaxa;
            sampleCovariance[i][j] = covariance[i][j] + m + m_2;
        }
    }
    
    //std::cout << "updateSampleCovariance\n";
    //print(sampleCovariance);
}

// R
void BrownianMotionAdmixtureGraph::updateResiduals(void)
{
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            residuals[i][j] = meanSampleCovarianceEstimator[i][j] - covariance[i][j];

        }
    }    
}

void BrownianMotionAdmixtureGraph::initializeTipNodesByIndex(void)
{
//    std::cout << "init\n";
    tipNodesByIndex.clear();
    std::vector<TopologyNode*> n = tau->getValue().getNodesByIndex();
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

void BrownianMotionAdmixtureGraph::updatePathsToRoot(void)
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
    double unitTreeScaler = 1.0 / tau->getValue().getRoot().getAge();
    double vpq = 0.0;
    
    //std::cout << "paths\t" << p->getIndex() << " (" << pPathSet.size() << ")\t" << q->getIndex() << " (" << qPathSet.size() << ")\n";
    // compare all pairs of paths from root to tips of interest
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
                size_t brIdx;
                
                // skip first step (root node)
                if (itp_step == itp_path->begin())
                    continue;
                
                // this step is not an admixture child event, so weight = 1
                if ( &(*itp_step)->getAdmixtureParent() == NULL)
                {
                    brIdx = p->getIndex();
                }
                
                // this step is an admixture event, and the previous path step is its parent
                else if ( &(*itp_step)->getAdmixtureParent() == (*itp_prev) )
                {
                    wp *= (*itp_step)->getWeight();
                    brIdx = (*itp_step)->getTopologyChild(0).getIndex();
                }
                
                // this step is an admixture event, but the previous path step is a topology node
                else if ( &(*itp_step)->getAdmixtureParent() != NULL )
                {
                    wp *= 1.0 - (*itp_step)->getWeight();
                    brIdx = (*itp_step)->getIndex();
                }
                
                // accumulate unit divergence for paths shared by p and q
                if (find( itq_path->begin(), itq_path->end(), *itp_step) != itq_path->end())
                {
                    u += ((*itp_prev)->getAge() - (*itp_step)->getAge())  * unitTreeScaler * branchRates->getValue()[brIdx];
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
