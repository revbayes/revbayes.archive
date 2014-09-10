
//  RateMap_AminoAcid.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "RateMap_AminoAcid.h"
#include <cmath>

using namespace RevBayesCore;

RateMap_AminoAcid::RateMap_AminoAcid(size_t nc) : RateMap(2, nc)
{
    homogeneousClockRate = 1.0;
    rootFrequencies = std::vector<double>(20,0.05);

    useRootFrequencies = true;
    branchHeterogeneousClockRates = false;
    branchHeterogeneousRateMatrices = false;
}

RateMap_AminoAcid::RateMap_AminoAcid(const RateMap_AminoAcid& m) : RateMap( m )
{
    homogeneousClockRate = m.homogeneousClockRate;
    heterogeneousClockRates = m.heterogeneousClockRates;
    homogeneousRateMatrix = m.homogeneousRateMatrix;
    heterogeneousRateMatrices = m.heterogeneousRateMatrices;
    rootFrequencies = m.rootFrequencies;
    
    useRootFrequencies = m.useRootFrequencies;
    branchHeterogeneousClockRates = m.branchHeterogeneousClockRates;
    branchHeterogeneousRateMatrices = m.branchHeterogeneousRateMatrices;
}

RateMap_AminoAcid::~RateMap_AminoAcid(void)
{
    
}

RateMap_AminoAcid& RateMap_AminoAcid::operator=(const RateMap_AminoAcid& r)
{
    if (this != &r)
    {
        RateMap::operator=( r );
        
        homogeneousClockRate = r.homogeneousClockRate;
        heterogeneousClockRates = r.heterogeneousClockRates;
        homogeneousRateMatrix = r.homogeneousRateMatrix;
        heterogeneousRateMatrices = r.heterogeneousRateMatrices;
        rootFrequencies = r.rootFrequencies;
        
        useRootFrequencies = r.useRootFrequencies;
        branchHeterogeneousClockRates = r.branchHeterogeneousClockRates;
        branchHeterogeneousRateMatrices = r.branchHeterogeneousClockRates;
    }
    return *this;
}

void RateMap_AminoAcid::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P) const
{
    RateMatrix* rm;
    if (branchHeterogeneousRateMatrices)
        rm = heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;
    
    rm->calculateTransitionProbabilities(node.getBranchLength(), P);
}

void RateMap_AminoAcid::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P, size_t charIdx) const
{
    RateMatrix* rm;
    if (branchHeterogeneousRateMatrices)
        rm = heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;
    
    rm->calculateTransitionProbabilities(node.getBranchLength(), P);
}

RateMap_AminoAcid* RateMap_AminoAcid::clone(void) const
{
    return new RateMap_AminoAcid( *this );
}

double RateMap_AminoAcid::getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double age) const
{
    unsigned fromState = from[ to->getIndex() ]->getState();
    unsigned toState = to->getState();
    
    double rate = 0.0;
    RateMatrix* rm;
    if (branchHeterogeneousRateMatrices)
        rm = heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;
    
    rate = (*rm)[fromState][toState];
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneousClockRates[node.getIndex()];
    else
        rate *= homogeneousClockRate;
    
    return rate;
    
}

double RateMap_AminoAcid::getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age) const
{
    
    double rate = 0.0;
    RateMatrix* rm;
    if (branchHeterogeneousRateMatrices)
        rm = heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;
    
    rate = (*rm)[from->getState()][to->getState()];
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneousClockRates[node.getIndex()];
    else
        rate *= homogeneousClockRate;
    
    return rate;
}

double RateMap_AminoAcid::getSiteRate(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx, double age) const
{
    
    double rate = 0.0;
    RateMatrix* rm;
    if (branchHeterogeneousRateMatrices)
        rm = heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;
    
    rate = (*rm)[from][to];
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneousClockRates[node.getIndex()];
    else
        rate *= homogeneousClockRate;
    
    return rate;
}

double RateMap_AminoAcid::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age) const
{
    
    // get characters in each state
    if (counts == NULL)
    {
        unsigned tmpCounts[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        counts = tmpCounts;
    }
    for (size_t i = 0; i < from.size(); i++)
        counts[ from[i]->getState() ] += 1;
    
    // get rate matrix
    RateMatrix* rm;
    if (branchHeterogeneousRateMatrices)
        rm = heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;

    // get the rate of leaving the sequence-state
    double sum = 0.0;
    for (size_t i = 0; i < 20; i++)
        sum += -(*rm)[i][i] * counts[i];
    
    // apply rate for branch
    if (branchHeterogeneousClockRates)
        sum *= heterogeneousClockRates[node.getIndex()];
    else
        sum *= homogeneousClockRate;
    
    return sum;
}

double RateMap_AminoAcid::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age) const
{
    
    unsigned counts[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    for (size_t i = 0; i < from.size(); i++)
        counts[ from[i]->getState() ] += 1;
    
    return getSumOfRates(node, from, counts, age);
}

void RateMap_AminoAcid::updateMap(void)
{
    if (needsUpdate)
    {
        ; // do nothing ...
        
        needsUpdate = false;
    }
}

const RateMatrix* RateMap_AminoAcid::getHomogeneousRateMatrix(void) const
{
    return homogeneousRateMatrix;
}

void RateMap_AminoAcid::setHomogeneousRateMatrix(const RateMatrix* r)
{
    branchHeterogeneousRateMatrices = false;
    *homogeneousRateMatrix = *r;
}

const RbVector<RateMatrix*>& RateMap_AminoAcid::getHeterogeneousRateMatrices(void) const
{
    return heterogeneousRateMatrices;
}

void RateMap_AminoAcid::setHeterogeneousRateMatrices(const RbVector<RateMatrix*>& r)
{
    branchHeterogeneousRateMatrices = true;
    heterogeneousRateMatrices = r;
}

double RateMap_AminoAcid::getHomogeneousClockRate(void) const
{
    return homogeneousClockRate;
}

void RateMap_AminoAcid::setHomogeneousClockRate(double r)
{
    branchHeterogeneousClockRates = false;
    homogeneousClockRate = r;
}

void RateMap_AminoAcid::setRootFrequencies(const std::vector<double>& r)
{
    rootFrequencies = r;
}

const std::vector<double>& RateMap_AminoAcid::getRootFrequencies(void) const
{
    return rootFrequencies;
}

const std::vector<double>& RateMap_AminoAcid::getHeterogeneousClockRates(void) const
{
    return heterogeneousClockRates;
}

void RateMap_AminoAcid::setHeterogeneousClockRates(const std::vector<double> &r)
{
    branchHeterogeneousClockRates = true;
    heterogeneousClockRates = r;
}


