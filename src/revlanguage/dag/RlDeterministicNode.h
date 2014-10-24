#ifndef RlDeterministicNode_H
#define RlDeterministicNode_H

#include "DeterministicNode.h"
#include "RevPtr.h"
#include "RlFunction.h"

namespace RevLanguage {
    
    /**
     * RevLanguage::DeterministicNode
     *
     * DeterministicNode is used in the Rev language layer to represent deterministic nodes. It differs from the class used for
     * deterministic nodes in the core layer (RC::DeterministicNode) in that it keeps a pointer to the Rev function associated with the
     * deterministic node. This is essential for userdefined functions but it is also practical for all deterministic nodes handled
     * in the Rev layer. For instance, the Rev function object is the most natural place to keep information about the name of the
     * Rev function, its orgin, and its code. The Rev function object also keeps the arguments of the function, which can be
     * accessed by their argument names through the Rev function object.
     */

    template<class valueType>
    class DeterministicNode : public RevBayesCore::DeterministicNode<valueType> {
    
    public:
                                            DeterministicNode(const std::string& n, RevBayesCore::TypedFunction<valueType>* fxn, Function* rlFxn);
                                            DeterministicNode(const DeterministicNode<valueType> &n);
        virtual                            ~DeterministicNode(void);
        
        // public methods
        DeterministicNode<valueType>*       clone(void) const;                                                  //!< Clone the node
        Function&                           getRlFunction(void);                                                //!< Get the Rev function
        const Function&                     getRlFunction(void) const;                                          //!< Get the Rev function (const)
        void                                printStructureInfo(std::ostream &o, bool verbose=false) const;      //!< Print information on structure

    private:

        Function*                           rlFunction;                                                         //!< Rev function

    };
    
}


template<class valueType>
RevLanguage::DeterministicNode<valueType>::DeterministicNode( const std::string& n, RevBayesCore::TypedFunction<valueType>* fxn, RevLanguage::Function* rlFxn ) :
    RevBayesCore::DeterministicNode<valueType>( n, fxn ),
    rlFunction( rlFxn )
{

}


template<class valueType>
RevLanguage::DeterministicNode<valueType>::DeterministicNode( const RevLanguage::DeterministicNode<valueType> &n )
    : RevBayesCore::DeterministicNode<valueType>( n ),
    rlFunction( n.rlFunction->clone() )
{

}


template<class valueType>
RevLanguage::DeterministicNode<valueType>::~DeterministicNode( void )
{

    delete rlFunction;
}


template<class valueType>
RevLanguage::DeterministicNode<valueType>* RevLanguage::DeterministicNode<valueType>::clone( void ) const
{
    
    return new DeterministicNode<valueType>( *this );
}


template<class valueType>
RevLanguage::Function& RevLanguage::DeterministicNode<valueType>::getRlFunction( void )
{
    
    return *rlFunction;
}


template<class valueType>
const RevLanguage::Function& RevLanguage::DeterministicNode<valueType>::getRlFunction( void ) const
{
    
    return *rlFunction;
}


/** Print struct for user */
template<class valueType>
void RevLanguage::DeterministicNode<valueType>::printStructureInfo( std::ostream& o, bool verbose ) const
{
    if ( verbose )
    {
        o << "_dagNode      = " << this->name << " <" << this << ">" << std::endl;
    }
    else
    {
        if ( this->name != "")
            o << "_dagNode      = " << this->name << std::endl;
        else
            o << "_dagNode      = <" << this << ">" << std::endl;
    }
    o << "_dagType      = Deterministic Rev function node" << std::endl;
    
    if ( verbose == true )
    {
        o << "_refCount     = " << this->getReferenceCount() << std::endl;
    }
    o << "_function     = " << rlFunction->getRevDeclaration() << std::endl;

    
    if ( verbose == true )
    {
        o << "_touched      = " << ( this->isFunctionDirty() ? "TRUE" : "FALSE" ) << std::endl;
    }
    
    o << "_parents      = ";
    this->printParents( o, 16, 70, verbose );
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren( o, 16, 70, verbose );

    o << std::endl;
}


#endif

