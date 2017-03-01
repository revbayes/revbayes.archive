//
//  PhylogeneticDistanceRateModifier.cpp
//  revbayes-branch-proj
//
//  Created by Michael Landis on 2/24/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#include <iomanip>
#include <cmath>
#include "CharacterEvent.h"
#include "PhylogeneticDistanceRateModifier.h"
#include "RbConstants.h"
#include "Tree.h"
#include "TreeUtilities.h"

using namespace RevBayesCore;

PhylogeneticDistanceRateModifier::PhylogeneticDistanceRateModifier(size_t ns, size_t nc) : CharacterHistoryRateModifier(ns, nc),
scale( 1.0 ),
num_branches( 0 )
{
    ;
}

PhylogeneticDistanceRateModifier::PhylogeneticDistanceRateModifier(const PhylogeneticDistanceRateModifier& g) : CharacterHistoryRateModifier(g)
{
    
    if (&g != this)
    {
        tau = g.tau;
        scale = g.scale;
        num_branches = g.num_branches;
    }
}

PhylogeneticDistanceRateModifier& PhylogeneticDistanceRateModifier::assign(const Assignable &m)
{
    
    const PhylogeneticDistanceRateModifier *crm = dynamic_cast<const PhylogeneticDistanceRateModifier*>(&m);
    if ( crm != NULL )
    {
        return operator=(*crm);
    }
    else
    {
        throw RbException("Could not assign character history rate modifier.");
    }
}

double PhylogeneticDistanceRateModifier::computeRateMultiplier(std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    
    // which character will be gained?
    size_t index = newState->getSiteIndex();
    
    // losses are independent of other species
    if (currState[index]->getState() > newState->getState() || scale == 0.0)
    {
        return 1.0;
    }
    
    double f = 1.0;
    
    // which characters are currently occupied?
    std::vector<std::set<size_t> > states_by_sites(this->num_characters);
    for (size_t i = 0; i < this->num_characters; i++)
    {
        states_by_sites[i].insert( currState[i]->getState() );
    }
    
    // get phylogenetic distance between current lineage and new lineage at age
   
    
    
    
    return f;
}

double PhylogeneticDistanceRateModifier::computeSiteRateMultiplier(const TopologyNode& node, CharacterEvent* currState, CharacterEvent* newState, double age)
{
    return 1.0;
}

double PhylogeneticDistanceRateModifier::computeSiteRateMultiplier(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx, double age)
{
    return 1.0;
}


PhylogeneticDistanceRateModifier* PhylogeneticDistanceRateModifier::clone(void) const
{
    return new PhylogeneticDistanceRateModifier(*this);
}

void PhylogeneticDistanceRateModifier::update(void)
{
    ; // do nothing
}

void PhylogeneticDistanceRateModifier::setTree(const RevBayesCore::Tree &t)
{
    tau = t;
    num_branches = tau.getNumberOfNodes() - 1;
//    node_phylo_distances =
//    RevBayesCore::DistanceMatrix* matrix = TreeUtilities::getDistanceMatrix ( tau );
    
}

void PhylogeneticDistanceRateModifier::setScale(const double& s)
{
    scale = s;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const PhylogeneticDistanceRateModifier& x)
{
    o << "PhylogeneticDistanceRateModifier";
    return o;
}