//
//  BrownianMotionAdmixtureGraphResiduals.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "BrownianMotionAdmixtureGraphResiduals.h"

using namespace RevBayesCore;

BrownianMotionAdmixtureGraphResiduals::BrownianMotionAdmixtureGraphResiduals(const StochasticNode<ContinuousCharacterData >* n) : TypedFunction< RbVector<double> >( new RbVector<double>() ),
    bmag( n )
{
    // add the tree parameter as a parent
    addParameter( bmag );
    //addParent(bmag);
    
    update();
}


BrownianMotionAdmixtureGraphResiduals::BrownianMotionAdmixtureGraphResiduals(const BrownianMotionAdmixtureGraphResiduals &n) : TypedFunction< RbVector<double> >( n ), bmag( n.bmag ) {
    // no need to add parameters, happens automatically
}


BrownianMotionAdmixtureGraphResiduals::~BrownianMotionAdmixtureGraphResiduals( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BrownianMotionAdmixtureGraphResiduals* BrownianMotionAdmixtureGraphResiduals::clone( void ) const {
    return new BrownianMotionAdmixtureGraphResiduals( *this );
}


void BrownianMotionAdmixtureGraphResiduals::update( void ) {
    
    
    //std::cout << "BMAGR\tupdate\n";
    const BrownianMotionAdmixtureGraph* b = static_cast<const BrownianMotionAdmixtureGraph*>(&bmag->getDistribution());
    std::vector<std::vector<double> > m = b->getResiduals();
    
    value->clear();
    for (size_t i = 0; i < m.size(); i++)
    {
        for (size_t j = 0; j < m.size(); j++)
        {
            value->push_back(m[i][j]);
            //std::cout << m[i][j] << "\t";
        }
        //std::cout << "\n";
    }
    //std::cout << "\n\n";
   // */
    
    //BrownianMotionAdmixtureGraph* b = static_cast<BrownianMotionAdmixtureGraph*>(&bmag->getDistribution());
    //*value = b->getResiduals();
    
    //bmag->g
    // *value = bmag->getResiduals();
    /*
    //    *value = tree->getValue().getNumberOfNodes();
    if (tree->getValue().getNumberOfAdmixtureParents() != tree->getValue().getNumberOfAdmixtureChildren())
        std::cout << "what!\t" << tree->getValue().getNumberOfAdmixtureParents() << "\t" << tree->getValue().getNumberOfAdmixtureChildren() << "\n";
    *value = tree->getValue().getNumberOfAdmixtureChildren();
     */
}



void BrownianMotionAdmixtureGraphResiduals::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == bmag) {
        bmag = static_cast<const StochasticNode<ContinuousCharacterData >* >( newP );
    }
}
