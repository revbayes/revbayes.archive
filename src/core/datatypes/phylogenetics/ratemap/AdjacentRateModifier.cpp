//
//  AdjacentRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <iomanip>
#include <cmath>
#include "CharacterEvent.h"
#include "AdjacentRateModifier.h"
#include "RbConstants.h"

using namespace RevBayesCore;

AdjacentRateModifier::AdjacentRateModifier(size_t ns, size_t nc) : CharacterHistoryRateModifier(ns, nc),
  width(1),
  factor(1),
  context_matrix( std::vector<std::vector<adjacency> >() ),
  context_type("width")
{
    ;
}

AdjacentRateModifier::AdjacentRateModifier(const AdjacentRateModifier& g) : CharacterHistoryRateModifier(g)
{
    
    if (&g != this)
    {
        width = g.width;
        factor = g.factor;
        context_matrix = g.context_matrix;
        context_type = g.context_type;
    }
}

AdjacentRateModifier& AdjacentRateModifier::assign(const Assignable &m)
{
    
    const AdjacentRateModifier *crm = dynamic_cast<const AdjacentRateModifier*>(&m);
    if ( crm != NULL )
    {
        return operator=(*crm);
    }
    else
    {
        throw RbException("Could not assign character history rate modifier.");
    }
}

double AdjacentRateModifier::computeRateMultiplier(std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    
    
    if (context_type=="width") {
        return computeRateMultiplierUsingWidth(currState, newState, age);
    }
    else if (context_type=="matrix")
    {
        return computeRateMultiplierUsingMatrix(currState, newState, age);
    }
    else
    {
        std::stringstream ss;
        ss << "Undefined context_type (" << context_type << ")";
        throw RbException(ss.str());
    }
    
}


double AdjacentRateModifier::computeRateMultiplierUsingWidth(std::vector<CharacterEvent*> currState, CharacterEvent* newState, double age)
{
    double num_match = 0;
    
    size_t to_index = newState->getSiteIndex();
    size_t to_state = newState->getState();
    
    int lower_bound = ( (int)(to_index - width) < 0                     ? 0                               : (int)(to_index - width) );
    int upper_bound = ( (int)(to_index + width) >= this->num_characters ? (int)(this->num_characters - 1) : (int)(to_index + width) );
    
    for (int i = lower_bound; i <= upper_bound; i++)
    {
        if (i == to_index)
            continue;
        else if (currState[i]->getState() == to_state)
        {
            num_match += 1.0;
        }
        else
        {
            num_match -= 1.0;
        }
        
    }
    
    double r = exp(num_match * factor);
    
    return r;
}

double AdjacentRateModifier::computeRateMultiplierUsingMatrix(std::vector<CharacterEvent*> currState, CharacterEvent* newState, double age)
{
    double num_match = 0;
    
    size_t to_index = newState->getSiteIndex();
    size_t to_state = newState->getState();
    
    for (size_t i = 0; i < context_matrix.size(); i++)
    {
        for (size_t j = 0; j < context_matrix[i].size(); j++)
        {
            double w = 0.0;
            const adjacency& v = context_matrix[i][j];
            if (v.to == to_index)
            {
                continue;
            }
            else if (currState[v.to]->getState() == to_state)
            {
                w = v.weight;
            }
            else
            {
                w = -v.weight;
            }
            num_match += w;
//            std::cout << i << " " << j << " " << w << "\n";
        }
    }
    double r = exp(num_match * factor);
    
    return r;
}


double AdjacentRateModifier::computeSiteRateMultiplier(const TopologyNode& node, CharacterEvent* currState, CharacterEvent* newState, double age)
{
    return 1.0;
}

double AdjacentRateModifier::computeSiteRateMultiplier(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx, double age)
{
    return 1.0;
}


AdjacentRateModifier* AdjacentRateModifier::clone(void) const
{
    return new AdjacentRateModifier(*this);
}

void AdjacentRateModifier::update(void)
{
    ; // do nothing
}

void AdjacentRateModifier::setFactor(double f)
{
    factor = f;
}

void AdjacentRateModifier::setWidth(size_t w)
{
    context_type = "width";
    width = w;
}

void AdjacentRateModifier::setContextMatrix(const RbVector<RbVector<double> >& c)
{

    context_type = "matrix";
    context_matrix = std::vector<std::vector<adjacency> >(this->num_characters);
    
    for (size_t i = 0; i < this->num_characters; i++)
    {
        for (size_t j = 0; j < this->num_characters; j++)
        {
            if (c[i][j] > 0.0)
            {
                adjacency v;
                v.from = i;
                v.to = j;
                v.weight = c[i][j];
                context_matrix[i].push_back(v);
            }
        }
    }
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const AdjacentRateModifier& x)
{
    o << "AdjacentRateModifier";
    return o;
}
