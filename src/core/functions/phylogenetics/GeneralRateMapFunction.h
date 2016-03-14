//#ifndef GeneralRateGeneratorSequenceFunction_H
//#define GeneralRateGeneratorSequenceFunction_H
//
//#include "GeographyRateModifier.h"
//#include "RateGeneratorSequence.h"
//#include "RateMatrix.h"
//#include "RbVector.h"
//#include "Tree.h"
//#include "TypedDagNode.h"
//#include "TypedFunction.h"
//
//#include <vector>
//
//namespace RevBayesCore {
//    
//    class GeneralRateGeneratorSequenceFunction : public TypedFunction<RateGeneratorSequence> {
//        
//    public:
//        GeneralRateGeneratorSequenceFunction(size_t ns, size_t nc);
//        GeneralRateGeneratorSequenceFunction(const GeneralRateGeneratorSequenceFunction &n);                                                                        //!< Copy constructor
//        virtual                                            ~GeneralRateGeneratorSequenceFunction(void);                                               //!< Virtual destructor
//        
//        // public member functions
//        GeneralRateGeneratorSequenceFunction*                             clone(void) const;                                                          //!< Create an independent clone
//        
//        // set parameters
//        void                                                setClockRate(const TypedDagNode< double > *r);
//        void                                                setClockRate(const TypedDagNode< RevBayesCore::RbVector< double > > *r);
//        void                                                setRateMatrix(const TypedDagNode<RateGenerator>* rm);
////        void                                                setRateMatrix(const TypedDagNode<RbVector<RateGenerator> > >* rm);
//        void                                                setRootFrequencies(const TypedDagNode< RevBayesCore::RbVector< double > > *f);
//        void                                                update(void);
//        
//    protected:
//        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swapping parameters
//        
//    private:
//        
//        // members
//        const TypedDagNode< double >*                               homogeneousClockRate;
//        const TypedDagNode< RevBayesCore::RbVector< double > >*     heterogeneousClockRates;
//        const TypedDagNode<RateGenerator>*                             homogeneousRateMatrix;
//        const TypedDagNode<RbVector<RateGenerator> >*                  heterogeneousRateMatrices;
////        const TypedDagNode<TimeTree>*                               tau;
//        const TypedDagNode<RevBayesCore::RbVector<double> >*        rootFrequencies;
//        
//        // geography epochs
//        bool                                                branchHeterogeneousClockRates;
//        bool                                                branchHeterogeneousRateMatrices;
//        
//    };
//    
////    std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x);
//    
//}
//
//#endif /* defined(__rb_mlandis__GeneralRateGeneratorSequenceFunction__) */
