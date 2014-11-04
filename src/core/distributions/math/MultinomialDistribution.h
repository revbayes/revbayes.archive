//
//  MultinomialDistribution.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/6/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__MultinomialDistribution__
#define __rb_mlandis__MultinomialDistribution__

#include "TypedDagNode.h"
#include "TypedDistribution.h"

#include <vector>

namespace RevBayesCore {
    
    class MultinomialDistribution : public TypedDistribution< RbVector<int> > {
        
    public:
        MultinomialDistribution(const TypedDagNode< RbVector<double> > *p, const TypedDagNode<int> *n);
        MultinomialDistribution(const MultinomialDistribution &m);                                                                          //!< Copy constructor
        virtual                                            ~MultinomialDistribution(void);                                                //!< Virtual destructor
        
        // public member functions
        MultinomialDistribution*                            clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             p;
        const TypedDagNode<int>*                            n;
    };
    
}


#endif /* defined(__rb_mlandis__MultinomialDistribution__) */
