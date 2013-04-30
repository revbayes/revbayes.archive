//
//  AdmixtureDelayDecrement.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/20/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureDelayDecrement.h"

using namespace RevBayesCore;

AdmixtureDelayDecrement::AdmixtureDelayDecrement( ConstantNode<int>* dt, double w ) : Move(dt, w), delayTimer(dt)
{
    
}

AdmixtureDelayDecrement* AdmixtureDelayDecrement::clone() const
{
    return new AdmixtureDelayDecrement( *this );
}

void AdmixtureDelayDecrement::swapNode(DagNode *oldN, DagNode *newN)
{
    if (oldN == delayTimer)
    {
        delayTimer = static_cast<ConstantNode<int>* >(newN);
    }
}

const std::string& AdmixtureDelayDecrement::getMoveName(void) const
{
    return "delay decrement";
}
    
void AdmixtureDelayDecrement::acceptMove(void)
{
    ;
}

double AdmixtureDelayDecrement::performMove(double& probRatio)
{
    return 0.0;
}

void AdmixtureDelayDecrement::rejectMove(void)
{
    ;
}

bool AdmixtureDelayDecrement::isGibbs(void) const
{
    return true;
}

void AdmixtureDelayDecrement::performGibbsMove(void)
{
    double t = delayTimer->getValue();
    if (t > 0)
    {
        t--;
        delayTimer->setValue(t);
        std::cout << t << "\n";
    }


}

