#include "BranchHistory.h"
#include "RateMap_Biogeography.h"
#include <cmath>

using namespace RevBayesCore;

RateMap_Biogeography::RateMap_Biogeography(size_t nc, bool fe, unsigned mrs) : RateMap(2, nc),
    geographyRateModifier()
{

    forbidExtinction = fe;
    geographyRateModifier = NULL;
    distancePower = 0.0;
    maxRangeSize = mrs;
    if (mrs == 0 || mrs > nc)
        mrs = (unsigned)nc;
//    rootFrequencies = std::vector<double>(2,0.5);
    
    
    epochs = std::vector<double>(1,0.0);
    numEpochs = epochs.size();
    epochOffset = numEpochs * numCharacters;
//    extinctionValues = std::vector<double>(numEpochs * this->numCharacters, 1.0);
//    dispersalValues = std::vector<double>(numEpochs * this->numCharacters * this->numCharacters, 1.0);
    adjacentAreaVector = std::vector<double>(numEpochs * this->numCharacters * this->numCharacters, 1.0);
    availableAreaVector = std::vector<double>(numEpochs * this->numCharacters, 1.0);

    useGeographyRateModifier = false;
    useAreaAvailable = false;
    useAreaAdjacency = false;
    useDistanceDependence = false;
    useUnnormalizedRates = false;
//    useRootFrequencies = true;
    
    branchOffset=1;
    epochOffset=1;
    
}

RateMap_Biogeography::RateMap_Biogeography(const RateMap_Biogeography& m) : RateMap( m )
{
    distancePower = m.distancePower;
    maxRangeSize = m.maxRangeSize;

    epochs = m.epochs;
    numEpochs = m.numEpochs;
    epochOffset = m.epochOffset;
    adjacentAreaVector = m.adjacentAreaVector;
    availableAreaVector = m.availableAreaVector;
    
    useAreaAvailable = m.useAreaAvailable;
    useAreaAdjacency = m.useAreaAdjacency;
    useDistanceDependence = m.useDistanceDependence;
    useRootFrequencies = m.useRootFrequencies;
    useUnnormalizedRates = m.useUnnormalizedRates;
    
    geographyRateModifier = m.geographyRateModifier;
    useGeographyRateModifier = m.useGeographyRateModifier;
    

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
        
        distancePower = r.distancePower;
        maxRangeSize = r.maxRangeSize;

        epochs = r.epochs;
        numEpochs = r.numEpochs;
        epochOffset = r.epochOffset;
        adjacentAreaVector = r.adjacentAreaVector;
        availableAreaVector = r.availableAreaVector;
        
        geographyRateModifier = r.geographyRateModifier;
        useGeographyRateModifier = r.useGeographyRateModifier;
        
        useAreaAdjacency = r.useAreaAdjacency;
        useAreaAvailable = r.useAreaAvailable;
        useDistanceDependence = r.useDistanceDependence;
        useUnnormalizedRates = r.useUnnormalizedRates;

        forbidExtinction = r.forbidExtinction;
        
        branchOffset = r.branchOffset;
        epochOffset = r.epochOffset;
    }
    return *this;
}

void RateMap_Biogeography::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P) const
{
   
    double branchLength = node.getBranchLength();
    
    double r = ( branchHeterogeneousClockRates ? heterogeneous_clock_rates[node.getIndex()] : homogeneous_clock_rate );
    const RateGenerator* rm = ( branchHeterogeneousRateMatrices ? &heterogeneousRateMatrices[node.getIndex()] : homogeneousRateMatrix );

    if (node.isRoot())
        branchLength = node.getAge() * 5;

    double expPart = exp( -( rm->getRate(1,0,0,1) + rm->getRate(0,1,0,1) ) * r * branchLength);
    double p = rm->getRate(1,0,0,1) / (rm->getRate(1,0,0,1) + rm->getRate(0,1,0,1));
    double q = 1.0 - p;
    
    P[0][0] = p + q * expPart;
    P[0][1] = q - q * expPart;
    P[1][0] = p - p * expPart;
    P[1][1] = q + p * expPart;
}

void RateMap_Biogeography::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P, size_t charIdx) const
{

    double startAge = ( node.isRoot() ? node.getAge() * 5 : node.getParent().getAge() );
    double endAge = node.getAge();
    double currAge = startAge;
    
    double r = ( branchHeterogeneousClockRates ? heterogeneous_clock_rates[node.getIndex()] : homogeneous_clock_rate );
    const RateGenerator* rm = ( branchHeterogeneousRateMatrices ? &heterogeneousRateMatrices[node.getIndex()] : homogeneousRateMatrix );
    
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
        
        double dispersalRate  = ( (availableAreaVector[ idx ] > 0) ? 1.0 : 0.0);
        double extinctionRate = ( (availableAreaVector[ idx ] > 0) ? 1.0 : 1e10);
        
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
        double glr0 = rm->getRate(1,0,currAge,extinctionRate);
        double glr1 = rm->getRate(0,1,currAge,dispersalRate);
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
        
//        std::cout << P[0][0] << "," << P[0][1] << ";" << P[1][0] << "," << P[1][1] << "\n";
        
        
        
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
    size_t s = to->getState();
    
    if (from[ to->getCharacterIndex() ]->getState() == to->getState())
    {
        std::cout << count[0] << " " << count[1] << "\n";
        std::cout << node.getIndex() << " problem...\n";
        ;
    }
    
    // rate to extinction cfg is 0
    if (count[1] == 1 && s == 0 && forbidExtinction)
        return 0.0;
    
    // rate for dispersal to exceed max range size is 0
    if (!node.isRoot() && count[1] >= maxRangeSize && s == 1)
    {
        
        return 0.0;
    }
    
    // rate according to binary rate matrix Q(node)
    if (branchHeterogeneousRateMatrices)
        rate = heterogeneousRateMatrices[node.getIndex()].getRate(!s,s,age,1);
    else
        rate = homogeneousRateMatrix->getRate(!s,s,age,1);
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneous_clock_rates[node.getIndex()];
    else
        rate *= homogeneous_clock_rate;

    // apply rate modifiers
    if (useGeographyRateModifier) // want this to take in age as an argument...
        rate *= geographyRateModifier->computeRateModifier(node, from, to, age);
    
    // root freqs
    if (useRootFrequencies)
    {
        int countDiff[2][2] = { {1, -1}, {-1, 1} };
        double rfr = pow(2*rootFrequencies[0], count[0] + countDiff[s][0]) * pow(2*rootFrequencies[1], count[1] + countDiff[s][1]);
//        double rfr = ( s == 0 ? rootFrequencies[0] / rootFrequencies[1] : rootFrequencies[1] / rootFrequencies[0] );
//        double rfr = ( s == 1 ? rootFrequencies[1] / rootFrequencies[0] : 1.0 );
//        double rfr = ( s == 0 ? rootFrequencies[0] : rootFrequencies[1] );
        rate *= rfr;
    }
    
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
    size_t s = to->getState();
//    int charIdx = to->getIndex();
//    int epochIdx = getEpochIndex(age);
    
    // rate according to binary rate matrix Q(node)
    if (branchHeterogeneousRateMatrices)
        rate = heterogeneousRateMatrices[node.getIndex()].getRate(!s,s,age,1);
    else
        rate = homogeneousRateMatrix->getRate(!s,s,age,1);
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneous_clock_rates[node.getIndex()];
    else
        rate *= homogeneous_clock_rate;
    
    // area effects
    if (useGeographyRateModifier)
        rate *= geographyRateModifier->computeSiteRateModifier(node,from,to,age);

    
    return rate;
}

double RateMap_Biogeography::getSiteRate(const TopologyNode& node, size_t from, size_t to, size_t charIdx, double age) const
{
    double rate = 0.0;
    size_t s = to;
    
//    int pres = !s;
//    int epochIdx = getEpochIndex(age);
    
    // rate according to binary rate matrix Q(node)
    if (branchHeterogeneousRateMatrices)
        rate = heterogeneousRateMatrices[node.getIndex()].getRate(!s,s,age,1);
    else
        rate = homogeneousRateMatrix->getRate(!s,s,age,1);
    
    if (branchHeterogeneousClockRates)
        rate *= heterogeneous_clock_rates[node.getIndex()];
    else
        rate *= homogeneous_clock_rate;
    
    // area effects
    if (useGeographyRateModifier)
    {
        double rm = geographyRateModifier->computeSiteRateModifier(node,from,to,charIdx,age);

        rate *= rm;
    }
//        rate *= geographyRateModifier->computeSiteRateModifier(node,from,to,charIdx,age);

    
    return rate;
}

double RateMap_Biogeography::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age) const
{
    
    if (useUnnormalizedRates)
        return getUnnormalizedSumOfRates(node, from, counts, age);
    
    size_t nodeIndex = node.getIndex();
    
    // get rate away away from currState
    unsigned n0 = counts[0];
//    if (useGeographyRateModifier)
//        n0 = geographyRateModifier->getNumAvailableAreas(node, from, age);
    unsigned n1 = counts[1];

    // forbid extinction events
//    if (counts[1] == 1 && forbidExtinction)
//        n1 = 0;
    if (counts[1] == 0 && forbidExtinction)
    {
        return 0.0;
    }
    
    // do not allow dispersal events if range size is at max
    if (!node.isRoot() && counts[1] >= maxRangeSize)
    {
        
        n0 = 0;
    }

    
    // get (effective) num characters in each state
    double r0 = n1;
    double r1 = n0;
    if (useAreaAdjacency)
    {
        r1 = n1;
//        r0 = geographyRateModifier->getNumAvailableAreas(node,from,age);
        r0 = geographyRateModifier->getNumEmigratableAreas(node,from,age);
    }
    
//    if (useAreaAdjacency || useAreaAvailable)
//        r0 = geographyRateModifier->getNumAvailableAreas(node,from,age);
//    
    // apply ctmc for branch
    if (branchHeterogeneousRateMatrices)
    {
        r0 *= heterogeneousRateMatrices[nodeIndex].getRate(1,0,age,1);
        r1 *= heterogeneousRateMatrices[nodeIndex].getRate(0,1,age,1);
    }
    else
    {
        r0 *= homogeneousRateMatrix->getRate(1,0,age,1);
        r1 *= homogeneousRateMatrix->getRate(0,1,age,1);
    }
    
    
    if (useRootFrequencies)
    {
        // root freqs
        int countDiff[2][2] = { {1, -1}, {-1, 1} };
        double rfr0 = pow(2*rootFrequencies[0], counts[0] + countDiff[0][0]) * pow(2*rootFrequencies[1], counts[1] + countDiff[0][1]);
        double rfr1 = pow(2*rootFrequencies[0], counts[0] + countDiff[1][0]) * pow(2*rootFrequencies[1], counts[1] + countDiff[1][1]);
//        double rfr0 = rootFrequencies[0];
//        double rfr1 = rootFrequencies[1];
        r0 *= rfr0;
        r1 *= rfr1;
    }
    
    // apply rate for branch.
    double sum = r0 + r1;
    if (branchHeterogeneousClockRates)
    {
        sum *= heterogeneous_clock_rates[nodeIndex];
    }
    else
    {
        sum *= homogeneous_clock_rate;
    }
        
    return sum;
}

double RateMap_Biogeography::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age) const
{
    unsigned n1 = (unsigned)numOn(from);
    unsigned n0 = (unsigned)(numCharacters - n1);
    unsigned counts[2] = {n0,n1};
    
    return RateMap_Biogeography::getSumOfRates(node, from, counts, age);
}

double RateMap_Biogeography::getUnnormalizedSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age) const
{
    size_t nodeIndex = node.getIndex();
    size_t epochIdx = getEpochIndex(age);
    
    // apply ctmc for branch
    const RateGenerator* rm = ( branchHeterogeneousRateMatrices ? &heterogeneousRateMatrices[node.getIndex()] : homogeneousRateMatrix );
    
    // get sum of rates
    double sum = 0.0;
    for (size_t i = 0; i < from.size(); i++)
    {
        size_t s = from[i]->getState();
        double v = availableAreaVector[ epochIdx * this->numCharacters + i ];
        
        if (forbidExtinction && s == 1 && counts[1] == 0)
            sum += 0.0;
        else if (s == 1 && v > 0)
            sum += rm->getRate(1,0,age,1);
        else if (s == 1 && v == 0)
            sum += 1e10;
        else  if (s == 0)
            sum += rm->getRate(0,1,age,v);
    }
    
    // apply rate for branch
    if (branchHeterogeneousClockRates)
        sum *= heterogeneous_clock_rates[nodeIndex];
    else
        sum *= homogeneous_clock_rate;
    
    return sum;
}



void RateMap_Biogeography::updateMap(void)
{
    if (needs_update)
    {
        ; // do nothing ...
        
        needs_update = false;
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

void RateMap_Biogeography::setGeographyRateModifier(const GeographyRateModifier& gdrm)
{
    useGeographyRateModifier = true;
    
    // ugly hack, prob better way to handle constness...
    geographyRateModifier = &const_cast<GeographyRateModifier&>(gdrm);

    epochs = geographyRateModifier->getEpochs();
    numEpochs = epochs.size();
    adjacentAreaVector = geographyRateModifier->getAdjacentAreaVector();
    availableAreaVector = geographyRateModifier->getAvailableAreaVector();
    useAreaAdjacency = geographyRateModifier->getUseAreaAdjacency();
    useAreaAvailable = geographyRateModifier->getUseAreaAvailable();
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

const std::set<size_t> RateMap_Biogeography::getRangeAndFrontierSet(const TopologyNode& node, BranchHistory* bh, double age) const
{
    std::set<size_t> ret;
    const std::vector<CharacterEvent*>& from = bh->getParentCharacters();
//    const std::vector<CharacterEvent*>& to = bh->getParentCharacters();
    size_t epochIdx = getEpochIndex(age);
    const std::vector<std::set<size_t> >& adjacentAreaSet = geographyRateModifier->getAdjacentAreaSet();
    
//    std::set<size_t>
    for (size_t i = 0; i < from.size(); i++)
    {
        if (from[i]->getState() == 1)
        {
            ret.insert(i);
            const std::set<size_t> adj = adjacentAreaSet[epochIdx*from.size() + i];
            std::set<size_t>::const_iterator it_adj;
            for (it_adj = adj.begin(); it_adj != adj.end(); it_adj++)
            {
                if (from[*it_adj]->getState() == 0)
                {
                    ret.insert(*it_adj);
                }
            }
        }
    }
    
    return ret;
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
