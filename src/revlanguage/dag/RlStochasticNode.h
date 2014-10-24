#ifndef RlStochasticNode_H
#define RlStochasticNode_H

#include "StochasticNode.h"
#include "RevMemberObject.h"

namespace RevLanguage {
    
    template<class valueType>
    class StochasticNode : public RevBayesCore::StochasticNode<valueType>, public RevMemberObject {
        
    public:
        StochasticNode(const std::string& n);
        StochasticNode(const StochasticNode<valueType> &n);
        virtual                            ~StochasticNode(void);
        
        // public methods
        StochasticNode<valueType>*          clone(void) const;                                                  //!< Clone the node
        
    private:
        
        MethodTable                         methods;
        
        
    };
    
}


template<class valueType>
RevLanguage::StochasticNode<valueType>::StochasticNode( const std::string& n ) :
RevBayesCore::StochasticNode<valueType>( n, fxn )
{
    
    ArgumentRules* clampArgRules = new ArgumentRules();
    clampArgRules->push_back( new ArgumentRule("x", getTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("clamp", new MemberProcedure( RlUtils::Void, clampArgRules) );
    
    ArgumentRules* redrawArgRules = new ArgumentRules();
    this->methods.addFunction("redraw", new MemberProcedure( RlUtils::Void, redrawArgRules) );
    
    ArgumentRules* probArgRules = new ArgumentRules();
    this->methods.addFunction("probability", new MemberProcedure( RealPos::getClassTypeSpec(), probArgRules) );
    
    ArgumentRules* lnprobArgRules = new ArgumentRules();
    this->methods.addFunction("lnProbability", new MemberProcedure( Real::getClassTypeSpec(), lnprobArgRules) );
    
    ArgumentRules* setValueArgRules = new ArgumentRules();
    setValueArgRules->push_back( new ArgumentRule("x", getTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("setValue", new MemberProcedure( RlUtils::Void, setValueArgRules) );
    
    ArgumentRules* unclampArgRules = new ArgumentRules();
    this->methods.addFunction("unclamp", new MemberProcedure( RlUtils::Void, unclampArgRules) );
    
}


template<class valueType>
RevLanguage::StochasticNode<valueType>::StochasticNode( const RevLanguage::StochasticNode<valueType> &n ) :
    RevBayesCore::StochasticNode<valueType>( n ),
    methods( n.methods )
{
    
}


template<class valueType>
RevLanguage::StochasticNode<valueType>::~StochasticNode( void )
{
    
}


template<class valueType>
RevLanguage::StochasticNode<valueType>* RevLanguage::StochasticNode<valueType>::clone( void ) const
{
    
    return new StochasticNode<valueType>( *this );
}


/**
 * Get common member methods.
 */
template<class valueType>
const RevLanguage::MethodTable& RevLanguage::StochasticNode<valueType>::getMethods( void ) const
{
    
    // return the internal value
    return methods;
}


#endif

