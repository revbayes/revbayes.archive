/**
 * @file
 * This file contains the declaration of the interface for all distribution.
 *
 * Distributions are either values inside DAG nodes, i.e. a constant node used as input for the DPP,
 * or be associated with a stochastic node.
 *
 * First, some distributions are requiring a distribution as a parameter, e.g. a generating distribution. Thus,
 * we need to implement distributions as objects storable in DAG nodes.
 *
 * Second, all stochastic nodes hold a distribution pointer. The value of the stochastic node is returned via
 * a call to get value in the distribution.
 *
 * Every distribution owns its value and hence this class is templated. Owning the value
 * has the advantage that calls to update can modify the value instead of creating a new object.
 * This is benefitial in functions generating large objects.
 *
 * @brief Declaration of distributions.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-04, version 1.0
 *
 * $Id: Function.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef Distribution_H
#define Distribution_H

#include "Cloneable.h"

#include <iostream>
#include <set>

namespace RevBayesCore {
    
    class DagNode;
    
    class Distribution : public Cloneable {
        
    public:
        // destructor
        virtual                            ~Distribution(void) {}
        
        // public methods
        virtual void                        getAffected(std::set<DagNode *>& affected, DagNode* affecter);  //!< get affected nodes
        const std::set<const DagNode*>&     getParameters(void) const;                                      //!< get the parameters of the distribution
        void                                keep(DagNode* affecter);
        void                                restore(DagNode *restorer);
        void                                touch(DagNode *toucher );

        virtual void                        reInitialized( void );                                          //!< The model was re-initialized
        
        // pure virtual public methods
        virtual Distribution*               clone(void) const = 0;                                          //!< Clone the distribution
        virtual double                      computeLnProbability(void) = 0;                                 //!< Compute the ln probability
        virtual void                        redrawValue(void) = 0;                                          //!< Draw a new random value from the distribution
        virtual void                        swapParameter(const DagNode *oldP, const DagNode *newP) = 0;    //!< Exchange the parameter
        
    protected:
        // hidden constructor
        Distribution(void);
        
        // inheritable methods
        void                                addParameter(const DagNode* p);                                 //!< add a parameter to the distribution
        void                                removeParameter(const DagNode *p);
  
        virtual void                        keepSpecialization(DagNode* affecter);
        virtual void                        restoreSpecialization(DagNode *restorer);
        virtual void                        touchSpecialization(DagNode *toucher);

    private:
        std::set<const DagNode*>            parameters;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Distribution& x);                                //!< Overloaded output operator
    
}

#endif
