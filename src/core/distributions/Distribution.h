#ifndef Distribution_H
#define Distribution_H

#include "Cloneable.h"

#include <iostream>
#include <set>

namespace RevBayesCore {
    
    /**
     * @brief Distribution: interface for all core distributions
     *
     * Distributions are typically associated with stochastic nodes but can also be used as input parameters
     * to other distributions, like the DPP, or to functions.
     *
     * All stochastic nodes hold a distribution pointer. The value of the stochastic node is returned via
     * a call to getValue in the distribution.
     *
     * Some distributions require a distribution as a parameter, e.g. a generating distribution. Thus,
     * we need to implement distributions as objects storable in DAG nodes.
     *
     * Every distribution owns its value and hence this class is templated. Owning the value
     * has the advantage that calls to update can modify the value instead of creating a new object.
     * This is beneficial in distributions on large objects, making it possible to work with partial
     * updates and restores, and partial probability calculations.
     *
     * Each derived distribution is responsible for managing its parameters, swapping them and returning
     * a set of pointers to them.
     */
    class DagNode;
    
    class Distribution : public Cloneable {
        
    public:
        // destructor
        virtual                                ~Distribution(void) {}
        
        // public methods
        virtual void                            getAffected(std::set<DagNode *>& affected, DagNode* affecter);  //!< get affected nodes
        void                                    keep(DagNode* affecter);
        void                                    restore(DagNode *restorer);
        void                                    touch(DagNode *toucher );

        virtual void                            reInitialized( void );                                          //!< The model was re-initialized
        
        // pure virtual public methods
        virtual Distribution*                   clone(void) const = 0;                                          //!< Clone the distribution
        virtual double                          computeLnProbability(void) = 0;                                 //!< Compute the ln probability
        virtual void                            redrawValue(void) = 0;                                          //!< Draw a new random value from the distribution

        // Parameter management functions
        virtual std::set<const DagNode*>        getParameters(void) const = 0;                                  //!< Return parameters
        virtual void                            swapParameter(const DagNode *oldP, const DagNode *newP) = 0;    //!< Swap a parameter
        
    protected:
        // hidden constructor
        Distribution(void);
        
        virtual void                            keepSpecialization(DagNode* affecter);
        virtual void                            restoreSpecialization(DagNode *restorer);
        virtual void                            touchSpecialization(DagNode *toucher);

    private:
        
    };
    
    // Global functions using the class
    std::ostream&                               operator<<(std::ostream& o, const Distribution& x);             //!< Overloaded output operator
    
}

#endif
