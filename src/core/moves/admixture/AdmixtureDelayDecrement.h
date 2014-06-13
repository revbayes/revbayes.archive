//
//  AdmixtureDelayDecrement.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/20/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureDelayDecrement__
#define __revbayes_mlandis__AdmixtureDelayDecrement__


#include <ostream>
#include <set>
#include <string>

#include "MoveOld.h"
#include "ConstantNode.h"

namespace RevBayesCore {
    
    class AdmixtureDelayDecrement : public MoveOld {
        
    public:
        AdmixtureDelayDecrement( ConstantNode<int>* dt, double w );                                                    //!<  constructor
        
        // Basic utility functions
        AdmixtureDelayDecrement*        clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
    private:
        
        ConstantNode<int>*              delayTimer;
    };
    
}


#endif /* defined(__revbayes_mlandis__AdmixtureDelayDecrement__) */
