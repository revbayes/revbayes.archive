//
//  RateMap_Biogeography.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "RateMap_Biogeography.h"
#include <cmath>

using namespace RevBayesCore;

RateMap_Biogeography::RateMap_Biogeography(size_t nc) : RateMap(2, nc)
{
    geographicDistanceRateModifier = NULL;
    useGeographicDistanceRateModifier = false;
    branchHeterogeneousClockRates = false;
    branchHeterogeneousGainLossRates = false;
    
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
    }
    return *this;
}

void RateMap_Biogeography::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P) const
{
    
    double r = ( branchHeterogeneousClockRates ? heterogeneousClockRates[node.getIndex()] : homogeneousClockRate );
    const std::vector<double>& glr = ( branchHeterogeneousGainLossRates ? heterogeneousGainLossRates[node.getIndex()] : homogeneousGainLossRates );
    double expPart = exp( -(glr[0] + glr[1]) * r * node.getBranchLength());
    double p = glr[0] / (glr[0] + glr[1]);
    double q = 1.0 - p;
    
    P[0][0] = p + q * expPart;
    P[0][1] = q - q * expPart;
    P[1][0] = p - p * expPart;
    P[1][1] = q + p * expPart;
    
//    double tp0[2][2] = { { p0 + q0 * expPart0, q0 - q0 * expPart0 }, { p0 - p0 * expPart0, q0 + p0 * expPart0 } };
    
    
//    // get model settings
//    size_t nodeIdx = node.getIndex();
//    size_t leftIdx = node.getChild(0).getIndex();
//    size_t rightIdx = node.getChild(1).getIndex();
//    
//    //std::vector<CharacterEvent*> states = this->histories[nodeIdx].getChildCharacters();
//    
//    double bt[3] = { tree.getBranchLength(nodeIdx), tree.getBranchLength( leftIdx ), tree.getBranchLength( rightIdx ) };
//    if (node.isRoot())
//        bt[0] = node.getAge() * 2;
//    
//    double br[3] = { 1.0, 1.0, 1.0 };
//    if (branchHeterogeneousClockRates)
//    {
//        br[0] = heterogeneousClockRates->getValue()[nodeIdx];
//        br[1] = heterogeneousClockRates->getValue()[leftIdx];
//        br[2] = heterogeneousClockRates->getValue()[rightIdx];
//    }
//    else
//        br[0] = br[1] = br[2] = homogeneousClockRate->getValue();
//    
//    const RateMatrix* rm0;
//    const RateMatrix* rm1;
//    const RateMatrix* rm2;
//    if (branchHeterogeneousSubstitutionMatrices)
//    {
//        rm0 = &heterogeneousRateMatrices->getValue()[nodeIdx];
//        rm1 = &heterogeneousRateMatrices->getValue()[leftIdx];
//        rm2 = &heterogeneousRateMatrices->getValue()[rightIdx];
//    }
//    else
//        rm0 = rm1 = rm2 = &homogeneousRateMatrix->getValue();
//    
//    double bs0 = br[0] * bt[0];
//    double bs1 = br[1] * bt[1];
//    double bs2 = br[2] * bt[2];
//    
//    // compute transition probabilities
//    //double r[2] = { (*rm)[1][0], (*rm)[0][1] };
//    //        double expPart0 = exp( - (r[0] + r[1]) * bs0);
//    //        double expPart1 = exp( - (r[0] + r[1]) * bs1);
//    //        double expPart2 = exp( - (r[0] + r[1]) * bs2);
//    //        double pi0 = r[0] / (r[0] + r[1]);
//    //        double pi1 = 1.0 - pi0;
//    
//    //        double tp0[2][2] = { { pi0 + pi1 * expPart0, pi1 - pi1 * expPart0 }, { pi0 - pi0 * expPart0, pi1 + pi0 * expPart0 } };
//    //        double tp1[2][2] = { { pi0 + pi1 * expPart1, pi1 - pi1 * expPart1 }, { pi0 - pi0 * expPart1, pi1 + pi0 * expPart1 } };
//    //        double tp2[2][2] = { { pi0 + pi1 * expPart2, pi1 - pi1 * expPart2 }, { pi0 - pi0 * expPart2, pi1 + pi0 * expPart2 } };
//    
//    
//    double expPart0 = exp( -((*rm0)[1][0] + (*rm0)[0][1]) * bs0);
//    double expPart1 = exp( -((*rm1)[1][0] + (*rm1)[0][1]) * bs1);
//    double expPart2 = exp( -((*rm2)[1][0] + (*rm2)[0][1]) * bs2);
//    double p0 = (*rm0)[1][0] / ((*rm0)[1][0] + (*rm0)[0][1]);
//    double q0 = 1.0 - p0;
//    double p1 = (*rm1)[1][0] / ((*rm1)[1][0] + (*rm1)[0][1]);
//    double q1 = 1.0 - p1;
//    double p2 = (*rm2)[1][0] / ((*rm2)[1][0] + (*rm2)[0][1]);
//    double q2 = 1.0 - p2;
//    
//    double tp0[2][2] = { { p0 + q0 * expPart0, q0 - q0 * expPart0 }, { p0 - p0 * expPart0, q0 + p0 * expPart0 } };
//    double tp1[2][2] = { { p1 + q1 * expPart1, q1 - q1 * expPart1 }, { p1 - p1 * expPart1, q1 + p1 * expPart1 } };
//    double tp2[2][2] = { { p2 + q2 * expPart2, q2 - q2 * expPart2 }, { p2 - p2 * expPart2, q2 + p2 * expPart2 } };
//    
//    //    std::cout << tp0[0][0] << " " << tp0[0][1] << "\n" << tp0[1][0] << " " << tp0[1][1] << "\n";
//    //    std::cout << tp1[0][0] << " " << tp1[0][1] << "\n" << tp1[1][0] << " " << tp1[1][1] << "\n";
//    //    std::cout << tp2[0][0] << " " << tp2[0][1] << "\n" << tp2[1][0] << " " << tp2[1][1] << "\n";
    
}

RateMap_Biogeography* RateMap_Biogeography::clone(void) const
{
    return new RateMap_Biogeography( *this );
}

double RateMap_Biogeography::getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, double age) const
{
    double rate = 0.0;
    int s = to->getState();
    int n1 = numOn(from);
    
    // rate to extinction cfg is 0
    if (n1 == 1 && s == 0)
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
        rate *= geographicDistanceRateModifier->computeRateModifier(node, from, to);
    
    return rate;

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

double RateMap_Biogeography::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age) const
{
    size_t nodeIndex = node.getIndex();
    
    // get rate away away from currState
    size_t n1 = numOn(from);
    size_t n0 = numCharacters - n1;
    
    // forbid extinction events
    if (n1 == 1)
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
    
    // apply rate for branch
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

double RateMap_Biogeography::getLnTransitionProbability(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, double t, double age) const
{
    double lnP = log(getRate(node, from, to, age)) - getSumOfRates(node, from, age) * t;
    return lnP;
}

void RateMap_Biogeography::updateMap(void)
{
    ; // do nothing
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
    homogeneousGainLossRates = r;
}

const std::vector<std::vector<double> >& RateMap_Biogeography::getHeterogeneousGainLossRates(void) const
{
    return heterogeneousGainLossRates;
}

void RateMap_Biogeography::setHeterogeneousGainLossRates(const std::vector<std::vector<double> > &r)
{
    heterogeneousGainLossRates = r;
}

double RateMap_Biogeography::getHomogeneousClockRate(void) const
{
    return homogeneousClockRate;
}

void RateMap_Biogeography::setHomogeneousClockRate(double r)
{
    homogeneousClockRate = r;
}

const std::vector<double>& RateMap_Biogeography::getHeterogeneousClockRates(void) const
{
    return heterogeneousClockRates;
}

void RateMap_Biogeography::setHeterogeneousClockRates(const std::vector<double> &r)
{
    heterogeneousClockRates = r;
}

void RateMap_Biogeography::setGeographicDistanceRateModifier(GeographicDistanceRateModifier* gdrm)
{
    useGeographicDistanceRateModifier = true;
    geographicDistanceRateModifier = gdrm;
}

GeographicDistanceRateModifier* RateMap_Biogeography::getGeographicDistanceRateModifier(void)
{
    return geographicDistanceRateModifier;
}


size_t RateMap_Biogeography::numOn(const std::vector<CharacterEvent*>& s) const
{
    size_t n = 0;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i]->getState() == 1)
            n++;
    return n;
}

