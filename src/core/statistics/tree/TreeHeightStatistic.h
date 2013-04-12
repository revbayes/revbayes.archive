/**
 * @file
 * This file contains the declaration of the tree-height statistic, which simply computes
 * the height of a tree from root to tips.
 *
 * @brief Declaration of the TreeHeightStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: TimeTree.h 1651 2012-07-05 14:47:08Z hoehna $
 */

#ifndef TreeHeightStatistic_H
#define TreeHeightStatistic_H

//#include "Statistic.h"
#include "TimeTree.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class TreeHeightStatistic : public TypedFunction<double> {
        
    public:
        TreeHeightStatistic(const TypedDagNode<TimeTree> *t);                                                                                   //!< Default constructor
        TreeHeightStatistic(const TreeHeightStatistic& t);                                                                                      //!< Copy constructor
        virtual                                    ~TreeHeightStatistic(void);                                                                  //!< Destructor
        
        TreeHeightStatistic&                        operator=(const TreeHeightStatistic& t);
        
        // Basic utility functions
        TreeHeightStatistic*                        clone(void) const;                                                                          //!< Clone object
        void                                        update(void);                                                                               //!< Clone the function
 
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
        
    private:
        // members
        const TypedDagNode<TimeTree>*               tree;

    };
        
}

#endif

