#ifndef SegregatingSitesFunction_H
#define SegregatingSitesFunction_H

#include "AbstractDiscreteCharacterData.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief Segregating sites function.
     *
     * This function computes the number of segregating sites in an alignment.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2015-04-30
     *
     */
    class SegregatingSitesFunction : public TypedFunction<int> {
        
    public:
        SegregatingSitesFunction(const TypedDagNode<AbstractDiscreteCharacterData> *a);
        virtual                                            ~SegregatingSitesFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        SegregatingSitesFunction*                               clone(void) const;                                                              //!< Create an independent clone
        void                                                    update(void);
        
    protected:
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< AbstractDiscreteCharacterData >*    alignment;
        
    };
    
}

#endif
