//
//  AdmixtureDelayDecrement.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/20/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureDelayDecrement.h"
#include "RbConstants.h"

using namespace RevBayesCore;

AdmixtureDelayDecrement::AdmixtureDelayDecrement( ConstantNode<int>* dt, double w ) : MoveOld(dt, w), delayTimer(dt)
{
    //nodes.insert(delayTimer);
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
    static std::string name = "AdmixtureEdgeDelayDecrement";
    
    return name;
}
    
void AdmixtureDelayDecrement::acceptMove(void)
{
    ;
}

double AdmixtureDelayDecrement::performMove(double& probRatio)
{
    std::cout << "\nAdmix Delay Decrement\n";
    
    
    double t = delayTimer->getValue();
    if (t > 0)
    {
        t--;
        delayTimer->setValue(t);
        //std::cout << "delayDecrement\t" << t << "\n";
    }

    return 0.0;
}

void AdmixtureDelayDecrement::rejectMove(void)
{
    ;
}
