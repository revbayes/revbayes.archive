#ifndef ModuloFunction_H
#define ModuloFunction_H

#include "TypedFunction.h"
#include "TypedDagNode.h"

#include <vector>

namespace RevBayesCore {
    
    
    /**
     * @brief Modulo operator function.
     *
     * This function performs the modulo operation, e.g., a % b = c
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-07-04
     *
     */
    class ModuloFunction : public TypedFunction<long> {
        
    public:
        ModuloFunction(const TypedDagNode<long> * l, const TypedDagNode<long> *r);
        virtual                                            ~ModuloFunction(void);                                                       //!< Virtual destructor
        
        // public member functions
        ModuloFunction*                                     clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<long>*                            left;
        const TypedDagNode<long>*                            right;
        
    };
    
}


#endif
