//
//  DispersalHistoryCtmc.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "DispersalHistoryCtmc.h"
#include "BranchHistory.h"
#include "DistributionExponential.h"
#include "ExponentialDistribution.h"
#include "TypedDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include <string>

// Uncomment the next line to enable abundance-dependent dispersal-extinction rates
//#define USE_DDBD

using namespace RevBayesCore;

DispersalHistoryCtmc::DispersalHistoryCtmc(TypedDagNode<RateMatrix> *rm, std::vector<const TypedDagNode<double>* > r, const TypedDagNode<TimeTree>* t, const TypedDagNode<double>* br, const TypedDagNode<double>* dp,  const TypedDagNode<double>* grsp, const TypedDagNode<double>* lrsp, const TypedDagNode<double>* rsf, const TypedDagNode<double>* asp, size_t nc, size_t ns, size_t idx, GeographicDistanceRateModifier* gd, RangeSizeRateModifier* grs, RangeSizeRateModifier* lrs, AreaSizeRateModifier* asrm) : AbstractCharacterHistoryCtmc(rm,r,t,br,nc,ns,idx), distancePower(dp), gainRangeSizePower(grsp), lossRangeSizePower(lrsp), rangeSizeFrequency(rsf), areaSizePower(asp), geographicDistances(gd), gainRangeSizeRateModifier(grs), lossRangeSizeRateModifier(lrs),areaSizeRateModifier(asrm)


{
    addParameter(distancePower);
    addParameter(lossRangeSizePower);
    addParameter(gainRangeSizePower);
    addParameter(rangeSizeFrequency);
    addParameter(areaSizePower);
    
    if (geographicDistances != NULL)
        geographicDistances->setDistancePower(distancePower->getValue());
    
    if (gainRangeSizeRateModifier != NULL)
    {
        gainRangeSizeRateModifier->updateFrequency(rangeSizeFrequency->getValue());
        gainRangeSizeRateModifier->updatePower(gainRangeSizePower->getValue());
    }
    if (lossRangeSizeRateModifier != NULL)
    {
        lossRangeSizeRateModifier->updateFrequency(rangeSizeFrequency->getValue());
        lossRangeSizeRateModifier->updatePower(lossRangeSizePower->getValue());
    }

    if (areaSizeRateModifier != NULL)
    {
        areaSizeRateModifier->updatePower(areaSizePower->getValue());
    }
}

DispersalHistoryCtmc::DispersalHistoryCtmc(std::vector<const TypedDagNode<double>* > r, const TypedDagNode<TimeTree>* t, const TypedDagNode<double>* br, const TypedDagNode<double>* dp,  const TypedDagNode<double>* grsp, const TypedDagNode<double>* lrsp, const TypedDagNode<double>* rsf, const TypedDagNode<double>* asp, size_t nc, size_t ns, size_t idx, GeographicDistanceRateModifier* gd, RangeSizeRateModifier* grs, RangeSizeRateModifier* lrs, AreaSizeRateModifier* asrm, std::vector<GeographicGridRateModifier*> ggrmv) : AbstractCharacterHistoryCtmc(r,t,br,nc,ns,idx), distancePower(dp), gainRangeSizePower(grsp), lossRangeSizePower(lrsp), rangeSizeFrequency(rsf), areaSizePower(asp), geographicDistances(gd), gainRangeSizeRateModifier(grs), lossRangeSizeRateModifier(lrs), areaSizeRateModifier(asrm), geographicGrids(ggrmv)

{
    addParameter(distancePower);
    addParameter(lossRangeSizePower);
    addParameter(gainRangeSizePower);
    addParameter(rangeSizeFrequency);
    addParameter(areaSizePower);
    
    if (geographicDistances != NULL)
        geographicDistances->setDistancePower(distancePower->getValue());
    
    if (gainRangeSizeRateModifier != NULL)
    {
        gainRangeSizeRateModifier->updateFrequency(rangeSizeFrequency->getValue());
        gainRangeSizeRateModifier->updatePower(gainRangeSizePower->getValue());
    }
    if (lossRangeSizeRateModifier != NULL)
    {
        lossRangeSizeRateModifier->updateFrequency(rangeSizeFrequency->getValue());
        lossRangeSizeRateModifier->updatePower(lossRangeSizePower->getValue());
    }
    
    if (areaSizeRateModifier != NULL)
    {
        areaSizeRateModifier->updatePower(areaSizePower->getValue());
    }
}

DispersalHistoryCtmc::DispersalHistoryCtmc(const DispersalHistoryCtmc& m) : AbstractCharacterHistoryCtmc(m)
{
    geographicDistances = m.geographicDistances;
    distancePower = m.distancePower;
    gainRangeSizePower = m.gainRangeSizePower;
    lossRangeSizePower = m.lossRangeSizePower;
    rangeSizeFrequency = m.rangeSizeFrequency;
    gainRangeSizeRateModifier = m.gainRangeSizeRateModifier;
    lossRangeSizeRateModifier = m.lossRangeSizeRateModifier;
    areaSizePower = m.areaSizePower;
    areaSizeRateModifier = m.areaSizeRateModifier;
}

DispersalHistoryCtmc* DispersalHistoryCtmc::clone(void) const
{
    return new DispersalHistoryCtmc(*this);
}

double DispersalHistoryCtmc::transitionRate(std::vector<CharacterEvent *> currState, CharacterEvent *nextState)
{
    
    double rate = 0.0;
    int s = nextState->getState();
    int n1 = numOn(currState);
//    std::cout << rates[0]->getValue() << " " << rates[1]->getValue() << "\n";
//    std::cout << n1 << " " << s << "\n";
//    std::cout << nextState->getIndex() << " : " << currState[ nextState->getIndex() ]->getState() << " -> " << nextState->getState() << "\n";
    // rate to extinction cfg is 0
    if (n1 == 1 && s == 0)
        return 0.0;
    
    // rate according to binary rate matrix Q
    rate = rates[s]->getValue();
    
//    std::cout << "rate " << rate <<"\n";
    
    
    // adjust rate by diversity-dependent BD process
#ifdef USE_DDBD
    //std::cout << s << " " << n1 << " " << rate << "\n";
    double f1 = (double)(n1-1) / (numCharacters-1);
    
    if (s == 0)
    {
        // rate = rates[0]->getValue() * exp( -(1.0 - f1) * lossRangeSizePower->getValue() );
        // death = r0 * exp( -b0 * (n-pi1[i]) / ((1-f1) * n) )
        //rate *= exp( -lossRangeSizePower->getValue() * (1.0 - f1) / (1.0 - rangeSizeFrequency->getValue()));
        rate /= (1.0 + exp( -lossRangeSizePower->getValue() * (f1 - rangeSizeFrequency->getValue())));
    }
    else
    {
        // rate = rates[1]->getValue() * exp( -f1 * gainRangeSizePower->getValue() );
        // birth = r1 * exp( -b1 * pi1[i] / (f1 * n))
        
        //rate *= exp( -gainRangeSizePower->getValue() * f1 / rangeSizeFrequency->getValue() );
        rate /= (1.0 + exp( gainRangeSizePower->getValue() * (f1 - rangeSizeFrequency->getValue())));

        // redistribute rates over number of unoccupied areas
        rate *= (double)n1 / (double)(numCharacters - n1);
    }
#endif
    
    /*
    // account for range size modifier
    if (gainRangeSizeRateModifier != NULL && nextState->getState() == 1)
    {
        rate *= gainRangeSizeRateModifier->computeRateModifier(currState, nextState);
    }
    else if (lossRangeSizeRateModifier != NULL && nextState->getState() == 0)
    {
        rate *= lossRangeSizeRateModifier->computeRateModifier(currState, nextState);
    }
    */

    // account for distance
    double drm = 1.0;
    if (geographicDistances != NULL && nextState->getState() == 1)
    {
        //drm = geographicDistances->computeRateModifier(currState, nextState);
//        std::cout << rate*drm << " = " << rate << " * " << drm << "\n";
        rate *= drm;
    }
    
    if (areaSizeRateModifier != NULL && nextState->getState() == 1)
    {
        double arm = areaSizeRateModifier->computeRateModifier(currState, nextState);
        //std::cout << rate*arm << " = " << rate << " * " << arm << "; " << nextState->getIndex() << " " << nextState->getState() << "\n";
        rate *= arm;
    }
    
    if (!geographicGrids.empty() && nextState->getState() == 1)
    {
        TopologyNode p = tree->getValue().getNode(index);
        int t_idx = 0;
        if (!p.isRoot())
        {
            double parentAge = tree->getValue().getNode(index).getParent().getAge();
            double eventAge = parentAge + tree->getValue().getNode(index).getBranchLength() * nextState->getTime();
            t_idx = (int)eventAge;
        }
        double grm = geographicGrids[t_idx]->computeRateModifier(currState, nextState);
        rate *= grm;
    }
    
    //std::cout << rate <<"\n";
    
    return rate;
}

double DispersalHistoryCtmc::sumOfRates(std::vector<CharacterEvent *> currState)
{
    // get rate away away from currState
    size_t n1 = numOn(currState);
    size_t n0 = numCharacters - n1;
    
    // forbid extinction events
    if (n1 == 1)
        n1 = 0;
    
    double r0 = 0.0;
    double r1 = 0.0;
    
#ifdef USE_DDBD
    // density-dependent BD process
    double f1 = (double)(n1-1) / (numCharacters-1); // f1 offset due to forbidden extinction
    
//    r0 = n1 * rates[0]->getValue() * exp( -(1.0 - f1) * lossRangeSizePower->getValue() );
//    r1 = n1 * rates[1]->getValue() * exp( -f1 * gainRangeSizePower->getValue() );
//    birth = r1 * exp( -b1 * pi1[i] / (f1 * n))
//    death = r0 * exp( -b0 * (n-pi1[i]) / ((1-f1) * n) )

//    r0 = n1 * rates[0]->getValue() * exp( -lossRangeSizePower->getValue() * (1.0 - f1) / (1.0 - rangeSizeFrequency->getValue()));
//    r1 = n1 * rates[1]->getValue() * exp( -gainRangeSizePower->getValue() * f1 / rangeSizeFrequency->getValue() );
    r0 = n1 * rates[0]->getValue() / (1.0 + exp( -lossRangeSizePower->getValue() * (f1 - rangeSizeFrequency->getValue())));
    r1 = n1 * rates[1]->getValue() / (1.0 + exp( gainRangeSizePower->getValue() * (f1 - rangeSizeFrequency->getValue())));
    
#else
    // density-independent (BayArea version)
    r0 = n1 * rates[0]->getValue();
    r1 = n0 * rates[1]->getValue();
#endif
    
    // modify sum of rates according to range size
    /*
    if (gainRangeSizeRateModifier != NULL)
    {
        ;//r1 *= gainRangeSizeRateModifier->computeRateModifier(currState, 1);
    }
    if (lossRangeSizeRateModifier != NULL)
    {
        ;//r0 *= lossRangeSizeRateModifier->computeRateModifier(currState, 1);
    }
    */
    
    double sum = r0 + r1;
    
    return sum;
}

void DispersalHistoryCtmc::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    AbstractCharacterHistoryCtmc::swapParameter(oldP, newP);
    
    if (oldP == distancePower)
    {
        distancePower = static_cast<const TypedDagNode<double>* >( newP );
    }
    
    else if (oldP == areaSizePower)
    {
        areaSizePower = static_cast<const TypedDagNode<double>* >(newP);
    }
    
    else if (oldP == gainRangeSizePower)
    {
        gainRangeSizePower = static_cast<const TypedDagNode<double>* >(newP);
    }
    
    else if (oldP == lossRangeSizePower)
    {
        lossRangeSizePower = static_cast<const TypedDagNode<double>* >(newP);
    }
    else if (oldP == rangeSizeFrequency)
    {
        rangeSizeFrequency = static_cast<const TypedDagNode<double>* >(newP);
    }

}

double DispersalHistoryCtmc::computeLnProbability(void)
{
    //return 0.0;
    
    double lnL = 0.0;
    
    if (tree->getValue().getNode(index).isRoot())
        return 0.0;
    
    BranchHistory* bh = value;
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    
    // reject extinction cfgs
    unsigned int n = numOn(currState);
    if (n == 0)
        return 0.0;
    
    std::multiset<CharacterEvent*,CharacterEventCompare> history = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    // rate/time scaler
    double t = 0.0;
    double dt = 0.0;
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0)
        bt = 10.0;
    //else bt = 1.0;
    
    double bs = bt * br;
    
//    std::cout << "bt,br" << bt << " " << br << "\n";
//    std::cout << "branch " << index << "\n";
 
    // stepwise events
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        // next event time
        double idx = (*it_h)->getIndex();
        dt = (*it_h)->getTime() - t;
        
        // rescale time
        dt *= bs;
        
        // reject extinction cfgs
        if ((*it_h)->getState() == 0)
            n--;
        else
            n++;
        
        if (n == 0)
            return RbConstants::Double::neginf;
        
        double tr = transitionRate(currState, *it_h);
        double sr = sumOfRates(currState);
        
        // lnL for stepwise events for p(x->y)
        lnL += log(tr) - sr * dt;
        
        // update state
        currState[idx] = *it_h;
        t += dt;
        //std::cout << t << " " << dt << " " << tr << " " << sr << " " << lnL << "; " << bs << " = " << bt << " * " << br << "; " << dt/bs << "; " << (*it_h)->getState() << " " << numOn(currState) << "\n";
    }
    
    // lnL for final non-event
    double sr = sumOfRates(currState);
    lnL += -sr * (1.0 * bs - t);

    //std::cout << "lnL " << lnL << "\n";
    
    ;
    return lnL;
}

double DispersalHistoryCtmc::computeLnProposal(void)
{
    double lnL = 0.0;
    
    if (tree->getValue().getNode(index).isRoot())
        return 0.0;
    
    BranchHistory* bh = value;
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    
    // reject extinction cfgs
    unsigned int n = numOn(currState);
    if (n == 0)
        return 0.0;
    
    std::multiset<CharacterEvent*,CharacterEventCompare> history = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    // rate/time scale
    double t = 0.0;
    double dt = 0.0;
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0)
        bt = 10.0;
    double bs = bt * br;
    
    // stepwise events
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        // next event time and index
        double idx = (*it_h)->getIndex();
        dt = (*it_h)->getTime() - t;
        
        // rescale time
        dt *= bs;
        
        if ((*it_h)->getState() == 0)
            n--;
        else
            n++;
        if (n == 0)
            return RbConstants::Double::neginf;
        
        double tr = rates[ (*it_h)->getState() ]->getValue();
        double sr = sumOfRates(currState);
        
        // lnL for stepwise events for p(x->y)
        lnL += log(tr) - sr * dt;
        
        // update state
        currState[idx] = *it_h;
        t += dt;
        //std::cout << t << " " << dt << " " << tr << " " << sr << " " << lnL << "; " << bt << " " << br << " " << dt << "; " << (*it_h)->getState() << " " << numOn(currState) << "\n";
    }
    
    // lnL for final non-event
    double sr = sumOfRates(currState);
    lnL += -sr * (1.0 * bs - t);
    
    // proposal probs for internal states is a little trickier, since whether you consider lnP for parent or child depends on proposal mechanism
    /*
     std::cout << lnL << "\n";
    std::cout << "----\n";
    
    // child and parent character states
    double p1 = rates[1]->getValue() / (rates[0]->getValue() + rates[1]->getValue());
    unsigned int m = numOn(value->getParentCharacters()) + numOn(value->getChildCharacters());
    std::cout << "m " << m << " " << " p1 " << p1 << "\n";
    lnL += m * log(p1) + (2 * numCharacters - m) * log(1.0 - p1);
    
    std::cout << lnL << "\n";
    std::cout << "----\n";
    */
    
    //std::cout << "lnP " << lnL << "\n";
    
    return lnL;
}



//virtual void                        keepSpecialization(DagNode* affecter);
//virtual void                        restoreSpecialization(DagNode *restorer);
void DispersalHistoryCtmc::touchSpecialization(DagNode *toucher)
{
    if (toucher == distancePower && geographicDistances != NULL)
    {
        geographicDistances->setDistancePower(distancePower->getValue());
    }
    
    else if (toucher == lossRangeSizePower && lossRangeSizeRateModifier != NULL)
    {
        lossRangeSizeRateModifier->updatePower(lossRangeSizePower->getValue());
    }
    else if (toucher == gainRangeSizePower && gainRangeSizeRateModifier != NULL)
    {
        gainRangeSizeRateModifier->updatePower(gainRangeSizePower->getValue());
    }
    
    else if (toucher == areaSizePower && areaSizeRateModifier != NULL)
    {
        areaSizeRateModifier->updatePower(areaSizePower->getValue());
    }
}



void DispersalHistoryCtmc::samplePath(const std::set<size_t>& indexSet)
{

    value->clearEvents(indexSet);
    
    // get transition rates
    double r[2] = { rates[0]->getValue(), rates[1]->getValue() };
    //std::cout << "rates " << r[0] << " " << r[1] << "\n";
    
    // reject sample path history
    std::vector<CharacterEvent*> parentVector = value->getParentCharacters();
    std::vector<CharacterEvent*> childVector = value->getChildCharacters();
    std::multiset<CharacterEvent*,CharacterEventCompare> history;
    
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0) // root bt
        bt = 10.0;
    double bs = br * bt;
    //std::cout << "index " << index << "\n";
    
    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
    {
        size_t i = *it;
        std::set<CharacterEvent*> tmpHistory;
        
        unsigned int currState = parentVector[i]->getState();
        unsigned int endState = childVector[i]->getState();
        do
        {
            tmpHistory.clear();
            currState = parentVector[i]->getState();
            double t = 0.0;
            do
            {
                unsigned int nextState = (currState == 1 ? 0 : 1);
                t += RbStatistics::Exponential::rv( r[nextState] * bs, *GLOBAL_RNG );
                //std::cout << i << " " << t << " " << currState << " " << r[currState] * bs << "\n";
                if (t < 1.0)
                {
                    currState = nextState;
                    CharacterEvent* evt = new CharacterEvent(i , nextState, t);
                    tmpHistory.insert(evt);
                }
                else
                {
                    ;//std::cout << "------\n";
                }
            }
            while(t < 1.0);
        }
        while (currState != endState);
        
        for (std::set<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
            history.insert(*it);
    }
    
    /*
    if (historyContainsExtinction(parentVector, history) == true)
    {
        std::cout << "contains extinction\n";
        samplePath(indexSet);
        return;
    }
     */
    //else
    value->updateHistory(history,indexSet);
    
    //value->print();
    //
    //std::cout << value->getHistory().size() << "\n";
    //std::cout << bt << "\n";
    //std::cout << bt * (double)value->getHistory().size() << "\n";
    
    ;
}

bool DispersalHistoryCtmc::historyContainsExtinction(const std::vector<CharacterEvent*>& v, const std::multiset<CharacterEvent*,CharacterEventCompare>& s)
{
    int n = numOn(v);
 
    std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
    
    //for (size_t i = 0; i < v.size(); i++) std::cout << v[i]->getState(); std::cout << "\n";
    
    for (it = s.begin(); it != s.end(); it++)
    {
        //for (size_t i = 0; i < (*it)->getIndex(); i++) std::cout << " "; std::cout << (*it)->getState() << "\n";
        if ( (*it)->getState() == 0 )
            n--;
        else
            n++;

        if (n == 0)
            return true;
    }
    
    return false;
}

double DispersalHistoryCtmc::sampleRootCharacterState(const std::set<size_t>& indexSet)
{
    if (!tree->getValue().getNode(index).isRoot())
        return 0.0;
    
    // compute transition probabilities
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0) // root bt
        bt = 100.0;
    double bs = br * bt;
    
    double r[2] = { rates[0]->getValue(), rates[1]->getValue() };
    double expPart = exp( -(r[0] + r[1]) * bs);
    
    double pi0 = r[0] / (r[0] + r[1]);
    double pi1 = 1.0 - pi0;
    double tp[2][2] = { { pi0 + pi1 * expPart, pi1 - pi1 * expPart }, { pi0 - pi0 * expPart, pi1 + pi0 * expPart } };
    
    std::vector<CharacterEvent*> parentStates = value->getParentCharacters();
    std::vector<CharacterEvent*> childStates = value->getChildCharacters();
    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
    {
        // should sample conditioned on descendant states...
        // not entirely sure why, though
        // for Gibbs sampler, must sample internal node states wp of model, not arbitrarily randomly
        // ... the bad news is I need information from descendant lineages then...
        
        unsigned int parentS = 0;
        unsigned int childS = childStates[*it]->getState();
        double u = GLOBAL_RNG->uniform01();
        if (u < tp[childS][1])
            parentS = 1;
        parentStates[*it] = new CharacterEvent(*it, parentS, 0.0);
    }
    
    //double lnP = sampleCharacterState(indexSet, childStates, 1.0);
    value->setParentCharacters(parentStates);
    
    return 0.0;
}

double DispersalHistoryCtmc::sampleChildCharacterState(const std::set<size_t>& indexSet)
{
    
    // compute transition probabilities
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0) // root bt
        bt = 100.0;
    double bs = br * bt;

    double r[2] = { rates[0]->getValue(), rates[1]->getValue() };
    double expPart = exp( - (r[0] + r[1]) * bs);

    double pi0 = r[0] / (r[0] + r[1]);
    double pi1 = 1.0 - pi0;
    double tp[2][2] = { { pi0 + pi1 * expPart, pi1 - pi1 * expPart }, { pi0 - pi0 * expPart, pi1 + pi0 * expPart } };
    
    std::vector<CharacterEvent*> parentState = value->getParentCharacters();
    std::vector<CharacterEvent*> childStates = value->getChildCharacters();
    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
    {
        unsigned int parentS = parentState[*it]->getState();
        unsigned int childS = 0;
        double u = GLOBAL_RNG->uniform01();
        if (u < tp[parentS][1])
            childS = 1;
        childStates[*it] = new CharacterEvent(*it, childS, 1.0);
    }
    
    //double lnP = sampleCharacterState(indexSet, childStates, 1.0);
    value->setChildCharacters(childStates);
    
    return 0.0;
    //return lnP;
}


double DispersalHistoryCtmc::sampleChildCharacterState(const std::set<size_t>& indexSet, const std::vector<CharacterEvent*>& state1, const std::vector<CharacterEvent*>& state2, double t1, double t2)
{
    
    // get branch scaler
    double br = branchRate->getValue();
    double rootAge = tree->getValue().getRoot().getAge();
    double bt = tree->getValue().getBranchLength(index) / rootAge;
    if (bt == 0.0) // root bt
        bt = 100.0;
    double bs = br * bt;
    //std::cout << "bs  " << bs << "\n";
    
    // compute transition probabilities
    double r[2] = { rates[0]->getValue(), rates[1]->getValue() };
    double expPart0 = exp( - (r[0] + r[1]) * bs);
    double expPart1 = exp( - (r[0] + r[1]) * t1/rootAge); // needs *br1
    double expPart2 = exp( - (r[0] + r[1]) * t2/rootAge); // needs *br2
    double pi0 = r[0] / (r[0] + r[1]);
    double pi1 = 1.0 - pi0;
    double tp0[2][2] = { { pi0 + pi1 * expPart0, pi1 - pi1 * expPart0 }, { pi0 - pi0 * expPart0, pi1 + pi0 * expPart0 } };
    double tp1[2][2] = { { pi0 + pi1 * expPart1, pi1 - pi1 * expPart1 }, { pi0 - pi0 * expPart1, pi1 + pi0 * expPart1 } };
    double tp2[2][2] = { { pi0 + pi1 * expPart2, pi1 - pi1 * expPart2 }, { pi0 - pi0 * expPart2, pi1 + pi0 * expPart2 } };
    
//    std::cout << tp0[0][0] << " " << tp0[0][1] << "\n" << tp0[1][0] << " " << tp0[1][1] << "\n";
//    std::cout << tp1[0][0] << " " << tp1[0][1] << "\n" << tp1[1][0] << " " << tp1[1][1] << "\n";
//    std::cout << tp2[0][0] << " " << tp2[0][1] << "\n" << tp2[1][0] << " " << tp2[1][1] << "\n";
    
    std::vector<CharacterEvent*> parentState = value->getParentCharacters();
    std::vector<CharacterEvent*> childState = value->getChildCharacters();
    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
    {
        unsigned int ancS = parentState[*it]->getState();
        //unsigned int thisS = childState[*it]->getState();
        unsigned int desS1 = state1[*it]->getState();
        unsigned int desS2 = state2[*it]->getState();

        double u = GLOBAL_RNG->uniform01();
        double g0 = tp0[ancS][0] * tp1[0][desS1] * tp2[0][desS2];
        double g1 = tp0[ancS][1] * tp1[1][desS1] * tp2[1][desS2];
        
        unsigned int s = 0;
        if (u < g1 / (g0 + g1))
            s = 1;
        
//        std::cout << "\t" << *it << " " << s << " : " << ancS << " -> (" << g0 << "," << g1 << ") -> (" << desS1 << "," << desS2 << ")\n";
        
        childState[*it] = new CharacterEvent(*it, s, 1.0);
    }
    
    //double lnP = sampleCharacterState(indexSet, childStates, 1.0);
    value->setChildCharacters(childState);
    
    return 0.0;
    //return lnP;
}


double DispersalHistoryCtmc::sampleParentCharacterState(const std::set<size_t>& indexSet)
{

    std::vector<CharacterEvent*> parentStates = value->getParentCharacters();
    double lnP = sampleCharacterState(indexSet, parentStates, 0.0);
    value->setParentCharacters(parentStates);
    return lnP;
}

double DispersalHistoryCtmc::sampleCharacterState(const std::set<size_t>& indexSet, std::vector<CharacterEvent*>& states, double t)
{
    double p1 = rates[1]->getValue() / (rates[0]->getValue() + rates[1]->getValue());
    int n1_old = 0;
    int n1_new = 0;
    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
    {
        
        unsigned int s = 0;
        if (GLOBAL_RNG->uniform01() < p1)
        {
            s = 1;
            n1_new++;
        }
        
        if (states[*it]->getState() == 1)
            n1_old++;
        
        states[*it] = new CharacterEvent(*it,s,t);
    }
    
    // retry if extinct
    double lnP = 0.0;
    unsigned int n = numOn(states);
    if (n == 0)
    {
        lnP = sampleCharacterState(indexSet, states,t);
        return lnP;
    }
    else
    {
        // proposal density (n0 - n'0) * ln p0 + (n1 - n'1) * ln p1
        lnP = (n1_old - n1_new) * log(p1) + (n1_new - n1_old) * log(1.0 - p1);
        //std::cout << (n1_old - n1_new) * log(p1)  << "\n";
        //std::cout << (n1_new - n1_old) * log(1.0 - p1) << "\n";
        ;
        return lnP;
    }
}


void DispersalHistoryCtmc::redrawValue(void)
{
    
    //std::cout << "ns " << numStates << "   nc " << numCharacters << "\n";
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < numCharacters; i++)
        indexSet.insert(i);
    
    if (value->getRedrawParentCharacters())
    {
        //std::cout << index << " redraw parent\n";
        sampleParentCharacterState(indexSet);
        value->setRedrawParentCharacters(false);
    }
    
    
    if (value->getRedrawChildCharacters())
    {
        //std::cout << index << " redraw child\n";
        sampleChildCharacterState(indexSet);
        value->setRedrawChildCharacters(false);
    }
    
    
    if (value->getRedrawHistory())
    {
        //std::cout << index << " redraw path\n";
        samplePath(indexSet);
        value->setRedrawHistory(false);
    }
    
//    std::cout << "redrawValue done\n";
    //value->print();
}


void DispersalHistoryCtmc::simulate(void)
{
    
    //std::cout << "ns " << numStates << "   nc " << numCharacters << "\n";
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < numCharacters; i++)
        indexSet.insert(i);

    simulatePath(indexSet);

    //value->print();
}

void DispersalHistoryCtmc::simulatePath(const std::set<size_t>& indexSet)
{
    value->clearEvents(indexSet);
    
    // branch length modifiers
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0) // root bt
        bt = 100.0;
    double bs = br * bt;
    
    //std::cout << br << " " << bt << "\n";
    
    // start state
    std::vector<CharacterEvent*> currState = value->getParentCharacters();
    std::set<CharacterEvent*,CharacterEventCompare> history;
    
    // simulate to end state
    double t = 0.0;
    unsigned int n = numOn(currState);
    do
    {
        // sample next event time
        double r_sum = sumOfRates(currState);
        t += RbStatistics::Exponential::rv( r_sum * bs, *GLOBAL_RNG );
        
        if (t < 1.0)
        {
            // sample next event index
            double u = GLOBAL_RNG->uniform01() * r_sum;
            for (size_t i = 0; i < numCharacters; i++)
            {
                unsigned int s = (currState[i]->getState() == 0 ? 1 : 0);

                // forbid extinction
                if (n == 1 && s == 0)
                    continue;
                
                CharacterEvent* evt = new CharacterEvent(i,s,t);
                double tx_rate = transitionRate(currState, evt);
                u -= tx_rate;
                
                //std::cout << "\tu " << u << ": " << i << " " << s << " " << t << " " << r_sum << " " << n << " " << tx_rate << " " << bt << " " << br << "\n";
                if (u <= 0.0)
                {
                    currState[i] = evt;
                    history.insert(evt);
                    
                    // check how many areas on
                    if (s == 0)
                        n--;
                    else
                        n++;
                    break;
                }
            }
        }
    }
    while (t < 1.0);
    
    /*
    std::cout << history.size() << "\n";
    std::cout << sumOfRates(currState) << "\n";
    std::cout << bt << "\n";
    std::cout << (double)(history.size()) / sumOfRates(currState) / bt << "\n";
    */
     
    value->setHistory(history);
    value->setChildCharacters(currState);

}

unsigned int DispersalHistoryCtmc::numOn(std::vector<CharacterEvent*> v)
{
    unsigned int numOn = 0;
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i]->getState() == 1)
            numOn++;
    }

    
    return numOn;
}

unsigned int DispersalHistoryCtmc::numOn(std::set<CharacterEvent*> s)
{
    unsigned int numOn = 0;
    for (std::set<CharacterEvent*>::iterator it = s.begin(); it != s.end(); it++)
    {
        if ( (*it)->getState() == 1)
            numOn++;
    }
    
    return numOn;
}

unsigned int DispersalHistoryCtmc::numOn(std::set<CharacterEvent*,CharacterEventCompare> s)
{
    unsigned int numOn = 0;
    for (std::set<CharacterEvent*,CharacterEventCompare>::iterator it = s.begin(); it != s.end(); it++)
    {
        if ( (*it)->getState() == 1)
            numOn++;
    }
    
    return numOn;
    
}
