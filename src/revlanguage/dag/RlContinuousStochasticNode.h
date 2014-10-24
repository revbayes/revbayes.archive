#ifndef RlContinuousStochasticNode_H
#define RlContinuousStochasticNode_H

#include "ContinuousStochasticNode.h"
#include "RevMemberObject.h"
#include "RlContinuousDistribution.h"
#include "RlPositiveContinuousDistribution.h"

namespace RevLanguage {
    
    class ContinuousStochasticNode : public RevBayesCore::ContinuousStochasticNode, public RevMemberObject {
        
    public:
        ContinuousStochasticNode(const std::string& n, RevBayesCore::ContinuousDistribution* dist, ContinuousDistribution* rlDist);
        ContinuousStochasticNode(const std::string& n, RevBayesCore::ContinuousDistribution* dist, PositiveContinuousDistribution* rlDist);
        ContinuousStochasticNode(const ContinuousStochasticNode &n);
        virtual                            ~ContinuousStochasticNode(void);
        
        // public methods
        ContinuousStochasticNode*                   clone(void) const;                                                                              //!< Clone the node
        virtual RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args, bool &found);         //!< Execute member method (if applicable)
        const MethodTable&                          getMethods( void ) const;                                                                       //!< Get the member methods
        Distribution&                               getRlDistribution(void);                                                                        //!< Get the Rev distribution
        const Distribution&                         getRlDistribution(void) const;                                                                  //!< Get the Rev distribution (const)
        
    private:
        
        Distribution*                               rlDistribution;                                                                                 //!< Rev distribution
        MethodTable                                 methods;
        
        
    };
    
}

#endif

