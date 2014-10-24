#ifndef RlConstantNode_H
#define RlConstantNode_H

#include "ConstantNode.h"
#include "RevMemberObject.h"

namespace RevLanguage {
    
    template<class valueType>
    class ConstantNode : public RevBayesCore::ConstantNode<valueType>, public RevMemberObject {
        
    public:
        ConstantNode(const std::string& n);
        ConstantNode(const ConstantNode<valueType> &n);
        virtual                            ~ConstantNode(void);
        
        // public methods
        ConstantNode<valueType>*            clone(void) const;                                                  //!< Clone the node
        const MethodTable&                  getMethods( void ) const;                                           //!< Get the member methods

    private:

        MethodTable                         methods;
 
    
    };
    
}


template<class valueType>
RevLanguage::ConstantNode<valueType>::ConstantNode( const std::string& n ) :
    RevBayesCore::ConstantNode<valueType>( n, fxn )
{
    
}


template<class valueType>
RevLanguage::ConstantNode<valueType>::ConstantNode( const RevLanguage::ConstantNode<valueType> &n ) :
    RevBayesCore::ConstantNode<valueType>( n ),
    methods( n.methods )
{
    
}


template<class valueType>
RevLanguage::ConstantNode<valueType>::~ConstantNode( void )
{
    
}


template<class valueType>
RevLanguage::ConstantNode<valueType>* RevLanguage::ConstantNode<valueType>::clone( void ) const
{
    
    return new ConstantNode<valueType>( *this );
}


/**
 * Get common member methods.
 */
template<class valueType>
const RevLanguage::MethodTable& RevLanguage::ConstantNode<valueType>::getMethods( void ) const
{
    
    // return the internal value
    return methods;
}


#endif

