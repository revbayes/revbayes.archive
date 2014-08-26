#ifndef SimplexFromVectorFunction_H
#define SimplexFromVectorFunction_H

#include "TypedFunction.h"
#include "TypedDagNode.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief Simplex constructor from vector
     *
     * This class implements a function that constructs a simplex from a vector of
     * real positive values.
     */
    class SimplexFromVectorFunction : public TypedFunction< std::vector<double> > {
        
    public:
        SimplexFromVectorFunction(const TypedDagNode< std::vector<double> >* vec);                                              //!< Basic constructor
        SimplexFromVectorFunction(const SimplexFromVectorFunction &n);                                                          //!< Copy constructor

        virtual                                         ~SimplexFromVectorFunction(void);                                       //!< Virtual destructor for derived classes
        
        // public member functions
        SimplexFromVectorFunction*                      clone(void) const;                                                      //!< Create a clone
        void                                            update(void);                                                           //!< Update the value of the function
        
    protected:
        void                                            swapParameterInternal(const DagNode *oldP, const DagNode *newP);        //!< Swap parameters
        
    private:
        
        // members
        const TypedDagNode< std::vector<double> >*      realPosVector;                                                          //!< Real positive vector parameter
        
    };
    
}


#endif
