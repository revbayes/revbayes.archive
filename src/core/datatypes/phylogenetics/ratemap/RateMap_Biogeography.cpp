
//  RateMap_Biogeography.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "RateMap_Biogeography.h"
#include <cmath>

using namespace RevBayesCore;

RateMap_Biogeography::RateMap_Biogeography(size_t nc, bool fe) : RateMap(2, nc),
    geographicDistanceRateModifier()
{
    useGeographicDistanceRateModifier = false;
    branchHeterogeneousClockRates = false;
    branchHeterogeneousGainLossRates = false;
    forbidExtinction = fe;
    geographicDistanceRateModifier = NULL;
    distancePower = 0.0;
    
    branchOffset=1;
    epochOffset=1;
    
}

RateMap_Biogeography::RateMap_Biogeography(const RateMap_Biogeography& m) : RateMap( m )
{
    homogeneousClockRate = m.homogeneousClockRate;
    heterogeneousClockRates = m.heterogeneousClockRates;
    homogeneousGainLossRates = m.homogeneousGainLossRates;
    heterogeneousGainLossRates = m.heterogeneousGainLossRates;
    distancePower = m.distancePower;
    
    geographicDistanceRateModifier = m.geographicDistanceRateModifier;
    useGeographicDistanceRateModifier = m.useGeographicDistanceRateModifier;
    
    branchHeterogeneousClockRates = m.branchHeterogeneousClockRates;
    branchHeterogeneousGainLossRates = m.branchHeterogeneousClockRates;
    forbidExtinction = m.forbidExtinction;
    
    branchOffset = m.branchOffset;
    epochOffset = m.epochOffset;
}

RateMap_Biogeography::~RateMap_Biogeography(void)
{
    
}

RateMap_Biogeography& RateMap_Biogeography::operator=(const RateMap_Biogeography& r)
{
    if (this != &r)
    {
        RateMap::operator=( r );
        
        homogeneousClockRate = r.homogeneousClockRate;
        heterogeneousClockRates = r.heterogeneousClockRates;
        homogeneousGainLossRates = r.homogeneousGainLossRates;
        heterogeneousGainLossRates = r.heterogeneousGainLossRates;
        distancePower = r.distancePower;
        
        geographicDistanceRateModifier = r.geographicDistanceRateModifier;
        useGeographicDistanceRateModifier = r.useGeographicDistanceRateModifier;
        
        branchHeterogeneousClockRates = r.branchHeterogeneousClockRates;
        branchHeterogeneousGainLossRates = r.branchHeterogeneousClockRates;
        forbidExtinction = r.forbidExtinction;
        
        branchOffset = r.branchOffset;
        epochOffset = r.epochOffset;
    }
    return *this;
}

void RateMap_Biogeography::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P) const
{
    
    double r = ( branchHeterogeneousClockRates ? heterogeneousClockRates[node.getIndex()] : homogeneousClockRate );
    const std::vector<double>& glr = ( branchHeterogeneousGainLossRates ? heterogeneousGainLossRates[node.getIndex()] : homogeneousGainLossRates );
    double l = node.getBranchLength();
    
    
    if (node.isRoot())
        l = 1000.0;
    double expPart = exp( -(glr[0] + glr[1]) * r * l);
    double p = glr[0] / (glr[0] + glr[1]);
    double q = 1.0 - p;
    
    P[0][0] = p + q * expPart;
    P[0][1] = q - q * expPart;
    P[1][0] = p - p * expPart;
    P[1][1] = q + p * expPart;

    //std::cout << node.getIndex() << " " << P[0][0] << " " << P[0][1] << " " << P[1][0] << " " << P[1][1] << "   " << glr[0] << " " << glr[1] << " " << l << " " << r << "\n";
    ;
    
    /*
    
     double r[2] = { rates[0]->getValue(), rates[1]->getValue() };
     double expPart0 = exp( - (r[0] + r[1]) * bs);
     double expPart1 = exp( - (r[0] + r[1]) * t1/rootAge); // needs *br1
     double expPart2 = exp( - (r[0] + r[1]) * t2/rootAge); // needs *br2
     double pi0 = r[0] / (r[0] + r[1]);
     double pi1 = 1.0 - pi0;
     double tp0[2][2] = { { pi0 + pi1 * expPart0, pi1 - pi1 * expPart0 }, { pi0 - pi0 * expPart0, pi1 + pi0 * expPart0 } };
     double tp1[2][2] = { { pi0 + pi1 * expPart1, pi1 - pi1 * expPart1 }, { pi0 - pi0 * expPart1, pi1 + pi0 * expPart1 } };
     double tp2[2][2] = { { pi0 + pi1 * expPart2, pi1 - pi1 * expPart2 }, { pi0 - pi0 * expPart2, pi1 + pi0 * expPart2 } };
     

     
     */
    
}

RateMap_Biogeography* RateMap_Biogeography::clone(void) const
{
    return new RateMap_Biogeography( *this );
}

double RateMap_Biogeography::getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* count, double age) const
{
    double rate = 0.0;
    int s = to->getState();
    
    if (from[ to->getIndex() ]->getState() == to->getState())
    {
        std::cout << count[0] << " " << count[1] << "\n";
        std::cout << node.getIndex() << " problem...\n";
        ;
    }
    
    // rate to extinction cfg is 0
    if (count[1] == 1 && s == 0 && forbidExtinction)
        return 0.0;
    
    // rate according to binary rate matrix Q(node)
    if (branchHeterogeneousGainLossRates)
        rate = heterogeneousGainLossRates[node.getIndex()][s];
    else
        rate = homogeneousGainLossRates[s];
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneousClockRates[node.getIndex()];
    else
        rate *= homogeneousClockRate;
    
    // apply rate modifiers
    if (useGeographicDistanceRateModifier) // want this to take in age as an argument...
        rate *= geographicDistanceRateModifier->computeRateModifier(node, from, to, age);
    
    return rate;

}

double RateMap_Biogeography::getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, double age) const
{
    unsigned n1 = numOn(from);
    unsigned n0 = numCharacters - n1;
    unsigned counts[2] = { n0, n1 };
    return getRate(node, from, to, counts, age);
}

double RateMap_Biogeography::getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age) const
{
    double rate = 0.0;
    int s = to->getState();
    
    // rate according to binary rate matrix Q(node)
    if (branchHeterogeneousGainLossRates)
        rate = heterogeneousGainLossRates[node.getIndex()][s];
    else
        rate = homogeneousGainLossRates[s];
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneousClockRates[node.getIndex()];
    else
        rate *= homogeneousClockRate;
    
    return rate;
}

double RateMap_Biogeography::getSiteRate(const TopologyNode& node, unsigned from, unsigned to, double age) const
{
    double rate = 0.0;
    int s = to;
    
    // rate according to binary rate matrix Q(node)
    if (branchHeterogeneousGainLossRates)
        rate = heterogeneousGainLossRates[node.getIndex()][s];
    else
        rate = homogeneousGainLossRates[s];
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneousClockRates[node.getIndex()];
    else
        rate *= homogeneousClockRate;
    
    return rate;
}

double RateMap_Biogeography::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age) const
{
    size_t nodeIndex = node.getIndex();
    
    // get rate away away from currState
    unsigned n0 = counts[0];
    unsigned n1 = counts[1];

    // forbid extinction events
    if (counts[1] == 1 && forbidExtinction)
        n1 = 0;
    
    // get characters in each state
    double r0 = n1;
    double r1 = n0;
    
    // apply ctmc for branch
    if (branchHeterogeneousGainLossRates)
    {
        r0 *= heterogeneousGainLossRates[nodeIndex][0];
        r1 *= heterogeneousGainLossRates[nodeIndex][1];
    }
    else
    {
        r0 *= homogeneousGainLossRates[0];
        r1 *= homogeneousGainLossRates[1];
    }
    
    // apply rate for branch.
    double sum = r0 + r1;
    if (branchHeterogeneousClockRates)
    {
        sum *= heterogeneousClockRates[nodeIndex];
    }
    else
    {
        sum *= homogeneousClockRate;
    }
    
    return sum;
}

double RateMap_Biogeography::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age) const
{
    unsigned n1 = numOn(from);
    unsigned n0 = numCharacters - n1;
    unsigned counts[2] = {n0,n1};
    return getSumOfRates(node, from, counts, age);
}

double RateMap_Biogeography::getLnTransitionProbability(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, double t, double age) const
{
    double lnP = log(getRate(node, from, to, age)) - getSumOfRates(node, from, age) * t;
    return lnP;
}

void RateMap_Biogeography::updateMap(void)
{
    if (needsUpdate)
    {
        ; // do nothing ...
        
        needsUpdate = false;
    }
}

double RateMap_Biogeography::getDistancePower(void) const
{
    return distancePower;
}

void RateMap_Biogeography::setDistancePower(double d)
{
    distancePower = d;
}

const std::vector<double>& RateMap_Biogeography::getHomogeneousGainLossRates(void) const
{
    return homogeneousGainLossRates;
}

void RateMap_Biogeography::setHomogeneousGainLossRates(const std::vector<double> &r)
{
    branchHeterogeneousGainLossRates = false;
    homogeneousGainLossRates = r;
}

const std::vector<std::vector<double> >& RateMap_Biogeography::getHeterogeneousGainLossRates(void) const
{
    return heterogeneousGainLossRates;
}

void RateMap_Biogeography::setHeterogeneousGainLossRates(const std::vector<std::vector<double> > &r)
{
    branchHeterogeneousGainLossRates = true;
    heterogeneousGainLossRates = r;
}

double RateMap_Biogeography::getHomogeneousClockRate(void) const
{
    return homogeneousClockRate;
}

void RateMap_Biogeography::setHomogeneousClockRate(double r)
{
    branchHeterogeneousClockRates = false;
    homogeneousClockRate = r;
}

const std::vector<double>& RateMap_Biogeography::getHeterogeneousClockRates(void) const
{
    return heterogeneousClockRates;
}

void RateMap_Biogeography::setHeterogeneousClockRates(const std::vector<double> &r)
{
    branchHeterogeneousClockRates = true;
    heterogeneousClockRates = r;
}

void RateMap_Biogeography::setGeographicDistanceRateModifier(const GeographicDistanceRateModifier& gdrm)
{
    useGeographicDistanceRateModifier = true;
    
    //*geographicDistanceRateModifier = gdrm;
//    geographicDistanceRateModifier = const_cast<GeographicDistanceRateModifier&>(gdrm);
    
    // ugly hack, prob better way to handle constness...
    geographicDistanceRateModifier = &const_cast<GeographicDistanceRateModifier&>(gdrm);
    
//    std::cout << *geographicDistanceRateModifier << " " << gdrm << "\n";    
//    std::cout << "ok\n";
//    true;
}

void RateMap_Biogeography::setGeographicDistancePowers(const GeographicDistanceRateModifier& gdrm)
{
    useGeographicDistanceRateModifier = true;
    geographicDistanceRateModifier->setGeographicDistancePowers(gdrm.getGeographicDistancePowers());
}


const GeographicDistanceRateModifier& RateMap_Biogeography::getGeographicDistanceRateModifier(void)
{
    return *geographicDistanceRateModifier;
}


size_t RateMap_Biogeography::numOn(const std::vector<CharacterEvent*>& s) const
{
    size_t n = 0;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i]->getState() == 1)
            n++;
    return n;
}

