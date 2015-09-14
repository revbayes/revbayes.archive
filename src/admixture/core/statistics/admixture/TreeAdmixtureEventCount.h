//
//  TreeAdmixtureEventCount.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/28/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__TreeAdmixtureEventCount__
#define __revbayes_mlandis__TreeAdmixtureEventCount__


//#include "Statistic.h"
#include "AdmixtureTree.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class TreeAdmixtureEventCount : public TypedFunction<size_t> {
        
    public:
        TreeAdmixtureEventCount(const TypedDagNode<AdmixtureTree> *t);                                                                                   //!< Default constructor
        TreeAdmixtureEventCount(const TreeAdmixtureEventCount& t);                                                                                      //!< Copy constructor
        virtual                                    ~TreeAdmixtureEventCount(void);                                                                  //!< Destructor
        
        TreeAdmixtureEventCount&                    operator=(const TreeAdmixtureEventCount& t);
        
        // Basic utility functions
        TreeAdmixtureEventCount*                    clone(void) const;                                                                          //!< Clone object
        void                                        update(void);                                                                               //!< Clone the function
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
        
    private:
        // members
        const TypedDagNode<AdmixtureTree>*          tree;
        
    };
    
}



#endif /* defined(__revbayes_mlandis__TreeAdmixtureEventCount__) */
