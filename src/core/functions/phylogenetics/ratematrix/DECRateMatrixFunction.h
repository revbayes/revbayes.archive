#ifndef DECRateMatrixFunction_H
#define DECRateMatrixFunction_H

#include "RateMatrix_DECRateMatrix.h"
#include "Simplex.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class DECRateMatrixFunction : public TypedFunction<RateGenerator> {
        
    public:
<<<<<<< HEAD
        DECRateMatrixFunction(const TypedDagNode< RbVector<RbVector<double> > > *dr, const TypedDagNode< RbVector<RbVector<double> > > *er, const TypedDagNode< RbVector<double> > *rs, bool cs=true, bool ex=false, bool uc=false, size_t mrs=0);
=======
        DECRateMatrixFunction(const TypedDagNode< RbVector<RbVector<double> > > *dr, const TypedDagNode< RbVector<RbVector<double> > > *er, const TypedDagNode< Simplex > *rs, bool cs=true, bool ex=false, bool uc=false, size_t mrs=0);
>>>>>>> development
        virtual                                             ~DECRateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        DECRateMatrixFunction*                              clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        size_t computeNumStates(size_t numAreas, size_t maxRangeSize, bool orderedStates);
        
        // members
        const TypedDagNode< RbVector<RbVector<double> > >*  dispersalRates;
        const TypedDagNode< RbVector<RbVector<double> > >*  extirpationRates;
<<<<<<< HEAD
        const TypedDagNode< RbVector<double> >*             rangeSize;
=======
        const TypedDagNode< Simplex >*                      rangeSize;
>>>>>>> development
    };
    
}

#endif /* defined(__revbayes_proj__DECRateMatrixFunction__) */
