#ifndef GeneralRateGeneratorSequenceFunction_H
#define GeneralRateGeneratorSequenceFunction_H

//#include "GeographyRateModifier.h"
#include "RateGeneratorSequence.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class GeneralRateGeneratorSequenceFunction : public TypedFunction<RateGeneratorSequence> {
        
    public:
        GeneralRateGeneratorSequenceFunction(size_t ns, size_t nc);
        GeneralRateGeneratorSequenceFunction(const GeneralRateGeneratorSequenceFunction &n);                                                                        //!< Copy constructor
        virtual                                            ~GeneralRateGeneratorSequenceFunction(void);                                               //!< Virtual destructor
        
        // public member functions
        GeneralRateGeneratorSequenceFunction*               clone(void) const;                                                          //!< Create an independent clone
        
        // set parameters
        void                                                setRateMatrix(const TypedDagNode<RateGenerator>* rm);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swapping parameters
        void                                                update(void);                                                               //!< Update the value of the function
        
    private:
        
        // members
        const TypedDagNode<RateGenerator>*                  homogeneousRateMatrix;
        
    };
    
//    std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x);
    
}

#endif /* defined(__rb_mlandis__GeneralRateGeneratorSequenceFunction__) */
