//#ifndef RlPositiveContinuousStochasticNode_H
//#define RlPositiveContinuousStochasticNode_H
//
//#include "ContinuousStochasticNode.h"
//#include "RevMemberObject.h"
//#include "RlDistribution.h"
//
//namespace RevLanguage {
//    
//    class PositiveContinuousStochasticNode : public RevBayesCore::ContinuousStochasticNode, public RevMemberObject {
//        
//    public:
//        PositiveContinuousStochasticNode(const std::string& n, RevBayesCore::ContinuousDistribution* dist, Distribution* rlDist);
//        PositiveContinuousStochasticNode(const PositiveContinuousStochasticNode &n);
//        virtual                            ~PositiveContinuousStochasticNode(void);
//        
//        // public methods
//        PositiveContinuousStochasticNode*           clone(void) const;                                                  //!< Clone the node
//        const MethodTable&                          getMethods( void ) const;                                           //!< Get the member methods
//        Distribution&                               getRlDistribution(void);                                            //!< Get the Rev distribution
//        const Distribution&                         getRlDistribution(void) const;                                      //!< Get the Rev distribution (const)
//        
//    private:
//        
//        Distribution*                               rlDistribution;                                                     //!< Rev function
//        MethodTable                                 methods;
//        
//        
//    };
//    
//}
//
//#endif
//
