
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
    geographyRateModifier()
{
    useGeographyRateModifier = false;
//    useUnnormalizedRates = false;
    branchHeterogeneousClockRates = false;
    branchHeterogeneousGainLossRates = false;
    forbidExtinction = fe;
    geographyRateModifier = NULL;
    distancePower = 0.0;
    
    
    epochs = std::vector<double>(1,0.0);
    numEpochs = epochs.size();
    epochOffset = numEpochs * numCharacters;
//    extinctionValues = std::vector<double>(numEpochs * this->numCharacters, 1.0);
//    dispersalValues = std::vector<double>(numEpochs * this->numCharacters * this->numCharacters, 1.0);
    adjacentAreaVector = std::vector<double>(numEpochs * this->numCharacters * this->numCharacters, 1.0);
    availableAreaVector = std::vector<double>(numEpochs * this->numCharacters, 1.0);
    
    useAreaAvailable = false;
    useAreaAdjacency = false;
    useDistanceDependence = false;
    
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

    epochs = m.epochs;
    numEpochs = m.numEpochs;
    epochOffset = m.epochOffset;
//    extinctionValues = m.extinctionValues;
//    dispersalValues = m.dispersalValues;
    adjacentAreaVector = m.adjacentAreaVector;
    availableAreaVector = m.availableAreaVector;
    
    useAreaAvailable = m.useAreaAvailable;
    useAreaAdjacency = m.useAreaAdjacency;
    useDistanceDependence = m.useDistanceDependence;
    
//    inboundDispersalValues = m.inboundDispersalValues;
//    useUnnormalizedRates = m.useUnnormalizedRates;
    
    geographyRateModifier = m.geographyRateModifier;
    useGeographyRateModifier = m.useGeographyRateModifier;
    
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

        epochs = r.epochs;
        numEpochs = r.numEpochs;
        epochOffset = r.epochOffset;
//        extinctionValues = r.extinctionValues;
//        dispersalValues = r.dispersalValues;
        adjacentAreaVector = r.adjacentAreaVector;
        availableAreaVector = r.availableAreaVector;
//        inboundDispersalValues = r.inboundDispersalValues;
//        useUnnormalizedRates = r.useUnnormalizedRates;
        
        geographyRateModifier = r.geographyRateModifier;
        useGeographyRateModifier = r.useGeographyRateModifier;
        
        useAreaAdjacency = r.useAreaAdjacency;
        useAreaAvailable = r.useAreaAvailable;
        useDistanceDependence = r.useDistanceDependence;
        
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
   
    double branchLength = node.getBranchLength();
    
    double r = ( branchHeterogeneousClockRates ? heterogeneousClockRates[node.getIndex()] : homogeneousClockRate );
    const std::vector<double>& glr = ( branchHeterogeneousGainLossRates ? heterogeneousGainLossRates[node.getIndex()] : homogeneousGainLossRates );

    if (node.isRoot())
        branchLength = 1e10;

    double expPart = exp( -(glr[0] + glr[1]) * r * branchLength);
    double p = glr[0] / (glr[0] + glr[1]);
    double q = 1.0 - p;
    
    P[0][0] = p + q * expPart;
    P[0][1] = q - q * expPart;
    P[1][0] = p - p * expPart;
    P[1][1] = q + p * expPart;
}

void RateMap_Biogeography::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P, size_t charIdx) const
{

    double startAge = ( node.isRoot() ? 1e10 : node.getParent().getAge() );
    double endAge = node.getAge();
    double currAge = startAge;
    
    double r = ( branchHeterogeneousClockRates ? heterogeneousClockRates[node.getIndex()] : homogeneousClockRate );
    const std::vector<double>& glr = ( branchHeterogeneousGainLossRates ? heterogeneousGainLossRates[node.getIndex()] : homogeneousGainLossRates );
    
    // start at earliest epoch
    int epochIdx = getEpochIndex(startAge);
    

    // initialize P = I
    P[0][0] = 1.0;
    P[0][1] = 0.0;
    P[1][0] = 0.0;
    P[1][1] = 1.0;
    

    
    bool stop = false;
    while (!stop)
    {
        // get dispersal and extinction rates for site
        size_t idx = this->numCharacters * epochIdx + charIdx;
        
        double dispersalRate  = ( (useUnnormalizedRates && availableAreaVector[ idx ] > 0) ? 1.0 : 0.0);
        double extinctionRate = ( (useUnnormalizedRates && availableAreaVector[ idx ] > 0) ? 1.0 : 1e10);
        
        // get age of start of next oldest epoch
        double epochAge = epochs[ epochIdx ];
        
        // get next oldest age boundary (node or epoch)
        double incrAge = epochAge;
        
        // no more epochs, exit loop after computation
        if (endAge >= epochAge)
        {
            incrAge = endAge;
            stop = true;
        }
        
        // get branch length
        double diffAge = currAge - incrAge;
        
        // transition probabilities w/ sum-product
        double glr0 = glr[0] * extinctionRate;
        double glr1 = glr[1] * dispersalRate;
        double expPart = exp( -(glr0 + glr1) * r * diffAge);
        double p = glr0 / (glr0 + glr1);
        double q = 1.0 - p;
        
        double p00 = (p + q * expPart);
        double p01 = (q - q * expPart);
        double p10 = (p - p * expPart);
        double p11 = (q + p * expPart);

        double q00 = P[0][0];
        double q01 = P[0][1];
        double q10 = P[1][0];
        double q11 = P[1][1];
        
        P[0][0] = p00 * q00 + p01 * q10;
        P[0][1] = p00 * q01 + p01 * q11;
        P[1][0] = p10 * q00 + p11 * q10;
        P[1][1] = p10 * q01 + p11 * q11;
        
        if (!stop)
        {
            epochIdx += 1;
            currAge = epochAge;
        }
    }
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
    if (useGeographyRateModifier) // want this to take in age as an argument...
        rate *= geographyRateModifier->computeRateModifier(node, from, to, age);
    
    return rate;

}

double RateMap_Biogeography::getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, double age) const
{
    unsigned n1 = (unsigned)numOn(from);
    unsigned n0 = (unsigned)(numCharacters - n1);
    unsigned counts[2] = { n0, n1 };
    return getRate(node, from, to, counts, age);
}

double RateMap_Biogeography::getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age) const
{
    double rate = 0.0;
    int s = to->getState();
//    int charIdx = to->getIndex();
//    int epochIdx = getEpochIndex(age);
    
    // rate according to binary rate matrix Q(node)
    if (branchHeterogeneousGainLossRates)
        rate = heterogeneousGainLossRates[node.getIndex()][s];
    else
        rate = homogeneousGainLossRates[s];
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneousClockRates[node.getIndex()];
    else
        rate *= homogeneousClockRate;
    
    // area effects
    if (useGeographyRateModifier)
        rate *= geographyRateModifier->computeSiteRateModifier(node,from,to,age);

    
    return rate;
}

double RateMap_Biogeography::getSiteRate(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx, double age) const
{
    double rate = 0.0;
    int s = to;
//    int epochIdx = getEpochIndex(age);
    
    // rate according to binary rate matrix Q(node)
    if (branchHeterogeneousGainLossRates)
        rate = heterogeneousGainLossRates[node.getIndex()][s];
    else
        rate = homogeneousGainLossRates[s];
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneousClockRates[node.getIndex()];
    else
        rate *= homogeneousClockRate;
    
    // area effects
    if (useGeographyRateModifier)
        rate *= geographyRateModifier->computeSiteRateModifier(node,from,to,charIdx,age);

    
    return rate;
}

double RateMap_Biogeography::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age) const
{
    
    if (useUnnormalizedRates)
        return getUnnormalizedSumOfRates(node, from, counts, age);
    
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

double RateMap_Biogeography::getUnnormalizedSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age) const
{
    size_t nodeIndex = node.getIndex();
    size_t epochIdx = getEpochIndex(age);
    
    // apply ctmc for branch
    const std::vector<double>& glr = ( branchHeterogeneousGainLossRates ? heterogeneousGainLossRates[nodeIndex] : homogeneousGainLossRates );
    
    // get sum of rates
    double sum = 0.0;
    for (size_t i = 0; i < from.size(); i++)
    {
        unsigned s = from[i]->getState();
        double v = availableAreaVector[ epochIdx * this->numCharacters + i ];
        
        if (forbidExtinction && s == 1 && counts[1] == 1)
            sum += 0.0;
        else if (s == 1 && v > 0)
            sum += glr[0];
        else if (s == 1 && v == 0)
            sum += 10e10;
        else  if (s == 0)
            sum += glr[1] * v;
    }
    
    // apply rate for branch
    if (branchHeterogeneousClockRates)
        sum *= heterogeneousClockRates[nodeIndex];
    else
        sum *= homogeneousClockRate;
    
    return sum;
}

double RateMap_Biogeography::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age) const
{
    unsigned n1 = (unsigned)numOn(from);
    unsigned n0 = (unsigned)(numCharacters - n1);
    unsigned counts[2] = {n0,n1};
    
    return getSumOfRates(node, from, counts, age);
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

void RateMap_Biogeography::setGeographyRateModifier(const GeographyRateModifier& gdrm)
{
    useGeographyRateModifier = true;
    
    // ugly hack, prob better way to handle constness...
    geographyRateModifier = &const_cast<GeographyRateModifier&>(gdrm);

    epochs = geographyRateModifier->getEpochs();
    numEpochs = epochs.size();
    adjacentAreaVector = geographyRateModifier->getAdjacentAreaVector();
    availableAreaVector = geographyRateModifier->getAvailableAreaVector();
    
    useUnnormalizedRates = true;
    
//    extinctionValues = geographyRateModifier->getAdjacentVector();
//    dispersalValues = geographyRateModifier->getAvailableVector();
//    setInboundDispersal( geographyRateModifier->getDispersalValues() );

}

void RateMap_Biogeography::setGeographicDistancePowers(const GeographyRateModifier& gdrm)
{
    useGeographyRateModifier = true;
    geographyRateModifier->setGeographicDistancePowers(gdrm.getGeographicDistancePowers());
}


const GeographyRateModifier& RateMap_Biogeography::getGeographyRateModifier(void)
{
    return *geographyRateModifier;
}

const bool RateMap_Biogeography::isAreaAvailable(size_t charIdx, double age) const
{
    size_t epochIdx = getEpochIndex(age);
    return availableAreaVector[epochIdx*this->numCharacters + charIdx] > 0.0;
}

const bool RateMap_Biogeography::areAreasAdjacent(size_t fromCharIdx, size_t toCharIdx, double age) const
{
    size_t epochIdx = getEpochIndex(age);
    return adjacentAreaVector[epochIdx*epochOffset + this->numCharacters*fromCharIdx + toCharIdx] > 0.0;

}

const std::vector<double>& RateMap_Biogeography::getEpochs(void) const
{
    return epochs;
}

size_t RateMap_Biogeography::numOn(const std::vector<CharacterEvent*>& s) const
{
    size_t n = 0;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i]->getState() == 1)
            n++;
    return n;
}

unsigned RateMap_Biogeography::getEpochIndex(double age) const
{
    // epochs are ordered from oldest to youngest, typically over (-neginf, 0.0)
    unsigned index = 0;
    while (age <= epochs[index] && index < epochs.size() - 1)
    {
        index++;
    };
    return index;
}
