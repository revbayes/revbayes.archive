/**
 * @file
 * This file contains the declaration of the (discrete) Brownian motion.
 * The random variable itself is the random path distributed by the Brownian motion (random walk)
 * with (small) intervals between steps, and hence discretized.
 *
 * @brief Declaration of the Brownian motion.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id:$
 */



#ifndef BrownianMotion_H
#define BrownianMotion_H

#include "RandomWalkProcess.h"

namespace RevBayesCore {
    
    class BrownianMotion : public RandomWalkProcess {
        
    public:
        BrownianMotion(const TypedDagNode<double> *m, const TypedDagNode<double> *s, const TypedDagNode<double> *t, int n);
        BrownianMotion(const BrownianMotion &n);                                                                                        //!< Copy constructor
        virtual                                            ~BrownianMotion(void);                                                       //!< Virtual destructor
        
        // public member functions
        BrownianMotion*                                     clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        int                                                 getNumberOfIntervals(void) const;
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    protected:
        void                                                touchSpecialization(DagNode *toucher);                                      //!< We need to catch here if the initial value is touched
        
    private:
        LinearStepFunction*                                 simulate(void);
        
        // members
        const TypedDagNode<double>*                         initialValue;
        const TypedDagNode<double>*                         sigma;
        const TypedDagNode<double>*                         time;
        int                                                 n;
    };
    
}

#endif
