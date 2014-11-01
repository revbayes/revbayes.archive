//
//  AdmixtureEdgePerTaxonBipartitionPair.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/11/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeCountPerBranch.h"
#include <iomanip>

using namespace RevBayesCore;

AdmixtureEdgeCountPerBranch::AdmixtureEdgeCountPerBranch(const TypedDagNode<AdmixtureTree>* t,  const TypedDagNode<int>* d, size_t nr) : TypedFunction< std::map< std::vector<bool>, std::map< std::vector<bool>, std::vector<AdmixtureNode*> > > >(new std::map<std::vector<bool>, std::map<std::vector<bool>, std::vector<AdmixtureNode*> > >()), tau(t), delayTimer(d), numSamples(0), numResults(nr)
{
    initializeTaxonBipartition();
    addParameter(tau);
    addParameter(delayTimer);
}

AdmixtureEdgeCountPerBranch::AdmixtureEdgeCountPerBranch(const AdmixtureEdgeCountPerBranch& n) : TypedFunction<std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureNode*> > > >(n), tau(n.tau), delayTimer(n.delayTimer), numTaxa(n.numTaxa), numSamples(n.numSamples), numResults(n.numResults)
{
    initializeTaxonBipartition();
}

AdmixtureEdgeCountPerBranch::~AdmixtureEdgeCountPerBranch(void)
{
    ;
}

AdmixtureEdgeCountPerBranch* AdmixtureEdgeCountPerBranch::clone() const {
    
    return new AdmixtureEdgeCountPerBranch( *this );
}

void AdmixtureEdgeCountPerBranch::update(void)
{
    if (delayTimer->getValue() <= 0)
    {
        //std::cout << "update AE count!\n";
        std::map<std::vector<bool>, std::vector<AdmixtureNode*> >::iterator it;
        
        std::vector<AdmixtureNode*> admixtureParents = tau->getValue().getAdmixtureParents();
        for (size_t i = 0; i < admixtureParents.size(); i++)
        {
            // find bipartitions determined by admixtureChild
            AdmixtureNode* p = admixtureParents[i];
            AdmixtureNode* c = &p->getAdmixtureChild();
            std::vector<bool> p_key = std::vector<bool>(numTaxa,false);
            std::vector<bool> c_key = std::vector<bool>(numTaxa,false);
            findTaxonBipartition(p_key, p);
            findTaxonBipartition(c_key, c);
            
            // if key exists, push on to existing value as copy of original node
            if (value->find(p_key) != value->end() && value->at(p_key).find(c_key) != value->at(p_key).end())
            {
                //AdmixtureNode n(*c);
                //(*value)[p_key][c_key].push_back(&n);
                (*value)[p_key][c_key].push_back(new AdmixtureNode(*c));
            }
            
            // otherwise, create new vector
            else
            {
                std::vector<AdmixtureNode*> tmp(1,p);
                (*value)[p_key][c_key] = tmp;
            }
        }
        numSamples++;
        
        // ... if final update(), call summarize
        if (numSamples % 1000 == 0)
            ;//std::cout << getSummary() << "\n";
    }
}

void AdmixtureEdgeCountPerBranch::initializeTaxonBipartition(void)
{
    numTaxa = tau->getValue().getNumberOfTips();
}

void AdmixtureEdgeCountPerBranch::findTaxonBipartition(std::vector<bool>& b, AdmixtureNode* p)
{
    size_t numChildren = p->getNumberOfChildren();
    for (size_t i = 0; i < numChildren; i++)
        findTaxonBipartition(b, &p->getChild(i));
    
    if (numChildren == 0)
        b[p->getIndex()] = 1;
}

void AdmixtureEdgeCountPerBranch::flipTaxonBipartitionToMinor(std::vector<bool>& b)
{
    size_t bitSum = 0;
    
    for (size_t i = 0; i < b.size(); i++)
        bitSum += b[i];
    
    if (bitSum > b.size()/2)
        b.flip();
}

void AdmixtureEdgeCountPerBranch::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {

    if (oldP == tau) {
        tau = static_cast<const TypedDagNode< AdmixtureTree >* >( newP );
    }
    else if (oldP == delayTimer)
    {
        delayTimer = static_cast<const TypedDagNode<int>* >(newP);
    }
}

size_t AdmixtureEdgeCountPerBranch::getNumSamples(void)
{
    return numSamples;
}

std::string AdmixtureEdgeCountPerBranch::getSummary(void)
{

    // insert all map elements into set sorted by number of edges in decreasing order
    std::set< std::pair< std::pair< std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureNode*> >, moreEdgesCompare> s;
    std::map< std::vector<bool>, std::map< std::vector<bool>, std::vector<AdmixtureNode*> > >::const_iterator it1;
    std::map< std::vector<bool>, std::vector<AdmixtureNode*> >::const_iterator it2;
    
    for (it1 = value->begin(); it1 != value->end(); it1++)
    {
        for (it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
        {

            // create ((bitfield1,bitfield2),edge)
            std::pair<std::pair<std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureNode*> > v(std::pair<std::vector<bool>, std::vector<bool> >(it1->first, it2->first), it2->second);
            s.insert(v);
        }
    }
    
    // store the taxon bipartition pairs with the greatest frequency of admixture events
    size_t curResults = 0;
    std::set< std::pair< std::pair< std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureNode*> >, moreEdgesCompare>::iterator its;
    std::vector<std::pair<std::vector<bool>, std::vector<bool> > > bestPairs;
    
    for (its = s.begin(); its != s.end(); its++)
    {
        if (curResults >= numResults)
            break;
        
        bestPairs.push_back(std::pair<std::vector<bool>, std::vector<bool> >(its->first.first, its->first.second));
        curResults++;
    }
    
    // query the map using bestPairs[i] to get the ith set of admixture edges given the two partitions exist
    // ...
    std::vector<TopologyNode*> tips = tau->getValue().getNodes();
    std::stringstream ss("");
    for (size_t i = 0; i < bestPairs.size(); i++)
    {

        ss << "(";
        //        for (size_t j = 0; j < bestPairs[i].second.size(); j++)
        //            ss << (bestPairs[i].second[j] ? "*" : ".");
        bool firstfirst = true;
        for (size_t j = 0; j < bestPairs[i].first.size(); j++)
        {
            if (bestPairs[i].first[j] == true)
            {
                if (firstfirst)
                    firstfirst = false;
                else
                    ss << ",";
                ss << tips[j]->getName();
            }
        }
        
        ss << ") -> (";
        //        for (size_t j = 0; j < bestPairs[i].second.size(); j++)
        //            ss << (bestPairs[i].second[j] ? "*" : ".");
        bool firstsecond = true;
        for (size_t j = 0; j < bestPairs[i].first.size(); j++)
        {
            if (bestPairs[i].second[j] == true)
            {
                if (firstsecond)
                    firstsecond = false;
                else
                    ss << ",";
                ss << tips[j]->getName();
            }
        }
        ss << ")";
        

        std::vector<AdmixtureNode*>* v = &(*value)[bestPairs[i].first][bestPairs[i].second];
        
        ss << "\t" << std::setprecision(8) << std::fixed << (double)v->size() / numSamples << "\t" << std::setw(8) << v->size() << "\t" << std::setw(8) << numSamples;
        

        double w = 0.0;
        double t = 0.0;
        for (std::vector<AdmixtureNode*>::iterator it = v->begin(); it != v->end(); it++)
        {
            w += (*it)->getWeight();
            t += (*it)->getAge();
            /*
            ss << "\t";
            ss << "p: " << &(*it)->getAdmixtureParent();
            ss << "c: " << &(*it)->getAdmixtureChild();
            ss << "w: " << (*it)->getWeight();
            ss << "\n";
             */
        }
        ss << "\tw: " << w / v->size();
        // ss << "\tt: " << t / v->size(); // ... not sure how meaningful this is...
        ss << "\n";
    }
    
    return ss.str();
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::map< std::vector<bool>, std::map< std::vector<bool>, std::vector<AdmixtureNode*> > >& x)
{

    // do nothing ...
    return o;
}
