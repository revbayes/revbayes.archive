//
//  AdmixtureAdmixtureConstantBirthDeathProcess.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureConstantBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbStatisticsHelper.h"
#include "DistributionBeta.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

AdmixtureConstantBirthDeathProcess::AdmixtureConstantBirthDeathProcess(const TypedDagNode<double> *d, const TypedDagNode<double> *t, unsigned int nTaxa, const std::vector<std::string> &tn) : TypedDistribution<AdmixtureTree>( new AdmixtureTree() ),
    diversification( d ),
    turnover( t ),
    numTaxa( nTaxa ),
    taxonNames( tn ),
    outgroup(std::vector<bool>(nTaxa,false))
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( diversification );
    addParameter( turnover );
  
    double lnFact = 0.0;
    for (size_t i = 2; i < numTaxa; i++) {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    simulateTree();
    
}

AdmixtureConstantBirthDeathProcess::AdmixtureConstantBirthDeathProcess(const TypedDagNode<double> *d, const TypedDagNode<double> *t, unsigned int nTaxa, const std::vector<std::string> &tn, const std::vector<bool>& o) : TypedDistribution<AdmixtureTree>( new AdmixtureTree() ), diversification( d ), turnover( t ), numTaxa( nTaxa ), numOutgroup(0), taxonNames( tn ), outgroup(o) {
  
    double lnFact = 0.0;
    for (size_t i = 2; i < numTaxa; i++) {
        lnFact += std::log(i);
    }

    for (size_t i = 0; i < outgroup.size(); i++)
    {
        if (outgroup[i] == true)
            numOutgroup++;
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    simulateTree();
    
}


AdmixtureConstantBirthDeathProcess::~AdmixtureConstantBirthDeathProcess() {
    
}



void AdmixtureConstantBirthDeathProcess::attachTimes(std::vector<AdmixtureNode *> &tips, int index, const std::vector<double> &times, double T) {
    
    if (index < (int)numTaxa-1) {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        AdmixtureNode* parent = tips.at(tip_index);
        parent->setAge( T - times[index] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+tip_index);
        
        // add a left child
        AdmixtureNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() ) {
            tips.push_back(leftChild);
        }
        
        // add a right child
        AdmixtureNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() ) {
            tips.push_back(rightChild);
        }
        
        // recursive call to this function
        attachTimes(tips, index+1, times, T);
    }
}


void AdmixtureConstantBirthDeathProcess::buildRandomBinaryTree(std::vector<AdmixtureNode*> &tips, size_t n, bool tf) {
    
    if (tips.size() < n) {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        AdmixtureNode* parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+index);
        
        // add a left child
        AdmixtureNode* leftChild = new AdmixtureNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        leftChild->setOutgroup(tf);
        tips.push_back(leftChild);
        
        // add a right child
        AdmixtureNode* rightChild = new AdmixtureNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        rightChild->setOutgroup(tf);
        tips.push_back(rightChild);
        
        // recursive call to this function
        buildRandomBinaryTree(tips, n, tf);
    }
}


AdmixtureConstantBirthDeathProcess* AdmixtureConstantBirthDeathProcess::clone( void ) const {
    return new AdmixtureConstantBirthDeathProcess( *this );
}


double AdmixtureConstantBirthDeathProcess::computeLnProbability( void ) {
    
    // uncomment for uniform prior
    // return 0.0;
    
    // retrieved the speciation times
    double lnPrior = 0.0;
    std::vector<double> times;
    for (unsigned i = numTaxa; i < value->getNumberOfNodes(); ++i) {
        AdmixtureNode* p = &value->getNode(i);
        if (p->getNumberOfChildren() == 2)
            times.push_back(p->getTime());
        if (&p->getAdmixtureParent() != NULL)
        {
            lnPrior += RbStatistics::Beta::lnPdf(1.0, 5.0, p->getWeight()*2);
        }
    }
    
    // uniform topology, priors on weights
    //return lnPrior;
    
    // sort the vector of times in ascending order
    std::sort(times.begin(), times.end());
    
    //std::cout << "tau times    ";
    //for (size_t i = 0; i < times.size(); i++) std::cout << times[i] << "\t";
    //std::cout << "\n";
    
    // present time
    double T = value->getTipNode(0).getTime();
    
    double lnProbTimes = 0.0;
    // bd process
    int numInitialSpecies = 1;
    // check if we condition on the root or origin
    if ( times[0] == 0.0 ) {
        //        lnProbTimes = - 2 * log( pSurvival(0,T) );
        ++numInitialSpecies;
    } else {
        //        lnProbTimes = - log( pSurvival(0,T) );
    }

    lnProbTimes = numInitialSpecies * ( p1(0,T) - log(pSurvival(0,T,T)) );
    
    
    for (unsigned i = (numInitialSpecies-1); i < numTaxa-1; ++i) {

        if ( lnProbTimes == RbConstants::Double::nan || lnProbTimes == RbConstants::Double::inf || lnProbTimes == RbConstants::Double::neginf) {
            return RbConstants::Double::nan;
        }
        lnProbTimes += log(diversification->getValue() + turnover->getValue()) + p1(times[i],T);
    }

    return lnProbTimes + lnPrior; // + logTreeTopologyProb;
    
}


double AdmixtureConstantBirthDeathProcess::p1(double t, double T) const {
    double a = log(pSurvival(t,T,T));
    double b = diversification->getValue() * (t-T);
    if ( b > 0 ) {
        return RbConstants::Double::neginf;
    }
    
    //    // remove the probability that we might not have sampled this species (or any descendant)
    //    double samplingProbability = rho->getValue().evaluate(t);
    //    b -= log(samplingProbability);
    
    double p = 2.0 * a + b;
    //std::cout << "\t" << a << "\t" << b << "\t" << p << "\n";
    return p;
    
}


double AdmixtureConstantBirthDeathProcess::pSurvival(double start, double end, double T) const {
    
    double div = diversification->getValue();
    double xxx = ( exp( div * (start-end) ) - 1.0 ) / (-div);
    double den = 1.0 + turnover->getValue() * ( xxx );// - ( rho - 1.0 ) * exp( rate(start,end,lambda,mu,rho) );
    
    //std::cout << "\t\t" << div << "\t" <<  xxx << "\t" << den << "\n";
    
    return (1.0 / den);
}


void AdmixtureConstantBirthDeathProcess::redrawValue( void ) {
    
    // MJL 060713: calling simulateTree() does not correctly generate a new tree -- crashes on admixture add RW proposal!
    // MJL 071613: not convinced the new nodes are copied to value properly
    simulateTree();

}



double AdmixtureConstantBirthDeathProcess::simOrigin( void ) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // random draw
    double u = rng->uniform01();
    
    // get the parameters
    double div = diversification->getValue();
    double mu = turnover->getValue();
    double lambda = div + mu;
    
	double origin = std::log( (-lambda + mu * std::pow(u, (1.0/numTaxa) )) / (lambda * (-1 + std::pow(u,(1.0/numTaxa))))) / div;
	
    return origin;
}


double AdmixtureConstantBirthDeathProcess::simSpeciation(double origin) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
	double u = rng->uniform01();
    
    // get the parameters
    double div = diversification->getValue();
    double mu = turnover->getValue();
    double lambda = div + mu;
    
    double t = 1.0/div * log((lambda - mu * exp((-div)*origin) - mu * (1.0 - exp((-div) * origin)) * u )/(lambda - mu * exp((-div) * origin) - lambda * (1.0 - exp(( -div ) * origin)) * u ) );
	
    return t;
}


void AdmixtureConstantBirthDeathProcess::simulateTree( void ) {
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Draw a random topology
    AdmixtureTree tau = AdmixtureTree();
    
    // internally we treat unrooted topologies the same as rooted
    tau.setRooted( true );
    
    //std::cout << "gen ingroup\n";
    // recursively build the ingroup tree
    AdmixtureNode* rootIngroup = new AdmixtureNode();
    std::vector<AdmixtureNode* > nodesIngroup;
    nodesIngroup.push_back(rootIngroup);
    buildRandomBinaryTree(nodesIngroup, numTaxa-numOutgroup, false);
    
    //std::cout << "gen outgroup\n";
    // recursively build and attach the outgroup tree
    AdmixtureNode* rootOutgroup = new AdmixtureNode();
    std::vector<AdmixtureNode*> nodesOutgroup;
    nodesOutgroup.push_back(rootOutgroup);
    buildRandomBinaryTree(nodesOutgroup, numOutgroup, true);
    
    /*
    std::cout << numOutgroup << "\t" << outgroup.size() << "\n";
    std::cout << "ingroup\t" << nodesIngroup.size() << "\n";
    std::cout << "outgroup\t" << nodesOutgroup.size() << "\n";
    */
    
    // set tip names
    //outgroup = std::vector<bool>(numTaxa, false);
    for (unsigned i=0; i<numTaxa; i++) {
        
        // std::cout << i << "\t";
        
        AdmixtureNode* node = NULL;
        // assign tip to outgroup
        if (outgroup[i] == true)
        {
            //std::cout << "T\t";
            size_t index = size_t( floor(rng->uniform01() * nodesOutgroup.size()) );
            //std::cout << index << "\n";

            // get the node from the list
            node = nodesOutgroup.at(index);
            
            // remove the randomly drawn node from the list
            nodesOutgroup.erase(nodesOutgroup.begin()+index);
            
        }

        // assign tip to ingroup
        else
        {
           // std::cout << "F\t";
            size_t index = size_t( floor(rng->uniform01() * nodesIngroup.size()) );
           // std::cout << index << "\n";
            
            // get the node from the list
            node = nodesIngroup.at(index);
            
            // remove the randomly drawn node from the list
            nodesIngroup.erase(nodesIngroup.begin()+index);
            
        }
        // set name
        std::string& name = taxonNames[i];
        node->setName(name);
        node->setIndex(i+1);
        node->setOutgroup(outgroup[i]);
        
    }
    
    //std::cout << "merge groups\n";
    // if there is an outgroup, combine ingroup and outgroup into one tree
    AdmixtureNode* root;
    if (numOutgroup > 0)
    {
        root = new AdmixtureNode();
        root->addChild(rootIngroup);
        root->addChild(rootOutgroup);
        rootOutgroup->setParent(root);
        rootIngroup->setParent(root);
    }
    // otherwise, the ingroup is the entire tree
    else
    {
        root = rootIngroup;
    }
    
    // initialize the topology by setting the root
    tau.setRoot(root);
    
    // now simulate the speciation times
    // draw a sample for the origin

    // unit tree
	double t_or = 1.0; // simOrigin();
    
    // allocate the vector for the times
    std::vector<double> t = std::vector<double>(numTaxa,0.0);
	t[0] = 0.0;
    
    // draw a time for each speciation event condition on the time of the process
	for (size_t i = 1; i < numTaxa; ++i) {
		t[i] = t_or - simSpeciation(t[i-1]);
	}
    
    std::sort(t.begin(),t.end());
    
    std::vector<AdmixtureNode*> nodes; 
    nodes.push_back( root );
    attachTimes(nodes, 0, t, t_or);
    for (size_t i = 0; i < numTaxa; ++i) {
        AdmixtureNode& n = tau.getTipNode(i);
        //std::cout << i << "\t" << n.getIndex() << "\t" << n.getName() << "\n";
        //n.setName(taxonNames[n.getIndex()]); // match taxonName and index orders
        n.setAge( 0.0 );
    }
    
    //std::cout << "outgroups\n";
    // propagate ingroup/outgroup flag towards root
    for (size_t i = 0; i < tau.getNumberOfTips(); i++)
    {
        AdmixtureNode* n = &tau.getTipNode(i);
        bool tf = n->isOutgroup();
        while (n != &tau.getRoot())
        {
            n->setOutgroup(tf);
            n = &n->getParent();
        }
        //std::cout << n->getName() << "  " << n->getIndex() << "  " << (n->isOutgroup() ? "T" : "F") << "\n";
    }
    /*
    std::cout << "outgroups\n";
    for (size_t i = 0; i < tau.getNumberOfNodes(); i++)
    {
        AdmixtureNode* n = &tau.getNode(i);
        std::cout << n->getName() << "  " << n->getIndex() << "  " << (n->isOutgroup() ? "T" : "F") << "\n";
    }*/
    // sort tau.nodes by index...
    // std::sort(nodes.begin(), nodes.end(),
    
    tau.setNames(taxonNames);
    tau.updateTipOrderByNames(taxonNames);
//    for (size_t i = 0; i < taxonNames.size(); i++) std::cout << "ACBDP " << taxonNames[i] << " " << tau.getNode(i).getName() << "\n";
    
    // finally store the new value
    *value = tau;
    
    ;
}


/** Swap a parameter of the distribution */
void AdmixtureConstantBirthDeathProcess::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == diversification)
    {
        diversification = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == turnover)
    {
        turnover = static_cast<const TypedDagNode<double>* >( newP );
    }
}
