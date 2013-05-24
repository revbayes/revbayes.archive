//
//  AdmixtureTreeLengthStatistic.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/16/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureTreeLengthStatistic__
#define __rb_mlandis__AdmixtureTreeLengthStatistic__

#include "AdmixtureTree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class AdmixtureTreeLengthStatistic : public TypedFunction<double> {
        
    public:
        AdmixtureTreeLengthStatistic(const TypedDagNode<AdmixtureTree> *t);                                                                     //!< Default constructor
        AdmixtureTreeLengthStatistic(const AdmixtureTreeLengthStatistic& t);                                                                    //!< Copy constructor
        
        AdmixtureTreeLengthStatistic&              operator=(const AdmixtureTreeLengthStatistic& t);
        
        // Basic utility functions
        AdmixtureTreeLengthStatistic*               clone(void) const;                                                                          //!< Clone object
        void                                        update(void);                                                                               //!< Clone the function
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
        
    private:
        // members
        const TypedDagNode<AdmixtureTree>*          tree;
        
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureAdmixtureTreeLengthStatistic__) */
