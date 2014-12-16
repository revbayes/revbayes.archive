//
//  BrownianMotionAdmixtureGraphResiduals.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__BrownianMotionAdmixtureGraphResiduals__
#define __revbayes_mlandis__BrownianMotionAdmixtureGraphResiduals__


#include "BrownianMotionAdmixtureGraph.h"
#include "StochasticNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class BrownianMotionAdmixtureGraphResiduals : public TypedFunction< RbVector<double> > {
        
    public:
        BrownianMotionAdmixtureGraphResiduals(const StochasticNode<ContinuousCharacterData >* n);                                                                                   //!< Default constructor
        BrownianMotionAdmixtureGraphResiduals(const BrownianMotionAdmixtureGraphResiduals& n);                                                                                      //!< Copy constructor
        virtual                                    ~BrownianMotionAdmixtureGraphResiduals(void);                                                                  //!< Destructor
        
        BrownianMotionAdmixtureGraphResiduals&                  operator=(const BrownianMotionAdmixtureGraphResiduals& n);
        
        // Basic utility functions
        BrownianMotionAdmixtureGraphResiduals*                  clone(void) const;                                                                          //!< Clone object
        void                                                    update(void);                                                                               //!< Clone the function
        //void touchSpecialization(DagNode* toucher);
        
    protected:
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
        
    private:
        // members
        const StochasticNode<ContinuousCharacterData >*         bmag;
        
    };
    
}



#endif /* defined(__revbayes_mlandis__BrownianMotionAdmixtureGraphResiduals__) */
