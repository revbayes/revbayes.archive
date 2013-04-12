/**
 * @file
 * This file contains the declaration of the (discrete) geometric Brownian motion.
 * The random variable itself is the random path distributed by the Brownian motion (random walk) in log-scale
 * with (small) intervals between steps, and hence discretized.
 *
 * @brief Declaration of the discrete geometric Brownian motion.
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



#ifndef GeometricBrownianMotion_H
#define GeometricBrownianMotion_H

#include "ConstantNode.h"
#include "RandomWalkProcess.h"

namespace RevBayesCore {
    
    class GeometricBrownianMotion : public RandomWalkProcess {
        
    public:
        GeometricBrownianMotion(const TypedDagNode<double> *i, const TypedDagNode<double> *s, const TypedDagNode<double> *t, int n);
        GeometricBrownianMotion(const GeometricBrownianMotion &n);                                                                                  //!< Copy constructor
        virtual                                            ~GeometricBrownianMotion(void);                                                          //!< Virtual destructor
        
        // public member functions
        GeometricBrownianMotion*                            clone(void) const;                                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        int                                                 getNumberOfIntervals(void) const;
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
        
    protected:
        void                                                getAffectedSpecialization(std::set<DagNode *>& affected, DagNode* affecter);            //!< get affected nodes
        void                                                keepSpecialization(DagNode* affecter);
        void                                                restoreSpecialization(DagNode *restorer);                                               //!< We need to catch here if the initial value is touched
        void                                                touchSpecialization(DagNode *toucher);                                                  //!< We need to catch here if the initial value is touched
        
    private:
        LinearStepFunction*                                 simulate(void);
        
        // members
        const TypedDagNode<double>*                         initialValue;
        const TypedDagNode<double>*                         sigma;
        const TypedDagNode<double>*                         time;
        // internal DAG nodes used to create step function
        ConstantNode<std::vector<double> >*                 values;
        int                                                 blocks;
    };
    
}

#endif
