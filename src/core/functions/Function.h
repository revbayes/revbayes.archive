/**
 * @file
 * This file contains the declaration of the interface for all functions.
 *
 * Functions either be values inside DAG nodes, i.e. the random variable of some distribution,
 * or be associated with a deterministic node.
 *
 * First, some distributions are requiring a function as a parameter, e.g. a rate function. Thus,
 * we need to implement functions as objects storable in DAG nodes.
 *
 * Second, all deterministic nodes hold a function. The value of the deterministic node is returned via
 * a call to get value in the function.
 *
 * Every functions owns it's value and hence this class is templated. Owning the value
 * has the advantage that calls to update can modify the value instead of creating a new object.
 * This is benefitial in functions generating large objects.
 *
 * @brief Declaration of functions.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-02, version 1.0
 *
 * $Id: Function.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef Function_H
#define Function_H

#include "Cloneable.h"

#include <iostream>
#include <set>

namespace RevBayesCore {
    
    class DagNode;
    
    class Function : public Cloneable {
        
    public:
        // constructors and destructor
        virtual                            ~Function(void);
               
        // public methods
        virtual void                        getAffected(std::set<DagNode *>& affected, DagNode* affecter);          //!< get affected nodes
        const std::set<const DagNode*>&     getParameters(void) const;                                              //!< get the parameters of the function
        virtual void                        keep(DagNode* affecter);
        virtual void                        reInitialized( void );                                                  //!< The model was re-initialized
        virtual void                        restore(DagNode *restorer);
        void                                swapParameter(const DagNode *oldP, const DagNode *newP);                //!< Exchange the parameter
        virtual void                        touch(DagNode *toucher );

        // pure virtual public methors
        virtual Function*                   clone(void) const = 0;                                                  //!< Clone the function
        virtual void                        update(void) = 0;                                                       //!< Clone the function
        
    protected:
        Function(void);                                                                                             //!< Default constructor
        Function(const Function &f);                                                                                //!< Copy constructor
        Function&                           operator=(const Function &f);                                           //!< Assignment operator
        
        void                                addParameter(const DagNode* p);                                         //!< add a parameter to the function
        void                                removeParameter(const DagNode* p);                                      //!< remove a parameter from the function
        virtual void                        swapParameterInternal(const DagNode *oldP, const DagNode *newP) = 0;    //!< Exchange the parameter
                
    private:
        
        std::set<const DagNode*>            parameters;
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Function& x);                                //!< Overloaded output operator
    
}

#endif
