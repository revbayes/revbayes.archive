/**
 * RlDeterministicNode.h
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */



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
        void                                printStructureInfo(std::ostream &o) const;                          //!< Print information on structure
        void                                touchMe(RevBayesCore::DagNode *toucher);                            //!< Use lazy evaluation (required by user-defined functions)
            
    protected:

    private:
        Function*                           rlFunction;                                                                             //!< Rev function
    };
    
}

using namespace RevLanguage;

template<class valueType>
DeterministicNode<valueType>::DeterministicNode( const std::string& n, RevBayesCore::TypedFunction<valueType>* fxn, Function* rlFxn ) :
    RevBayesCore::DeterministicNode<valueType>( n, fxn ), rlFunction( rlFxn ) {

    touchMe( this );
}


template<class valueType>
DeterministicNode<valueType>::DeterministicNode( const DeterministicNode<valueType> &n )
    : RevBayesCore::DeterministicNode<valueType>( n ), rlFunction( n.rlFunction->clone() ) {

    touchMe( this );    // NB: This for safety; not needed if we trust all involved copy constructors
}


template<class valueType>
DeterministicNode<valueType>::~DeterministicNode( void ) {

    delete rlFunction;
}


template<class valueType>
DeterministicNode<valueType>* DeterministicNode<valueType>::clone( void ) const {
    
    return new DeterministicNode<valueType>( *this );
}


template<class valueType>
Function& DeterministicNode<valueType>::getRlFunction( void ) {
    
    return *rlFunction;
}


template<class valueType>
const Function& DeterministicNode<valueType>::getRlFunction( void ) const {
    
    return *rlFunction;
}


/** Touch this node for recalculation. Here we use lazy evaluation. */
template<class valueType>
void DeterministicNode<valueType>::touchMe( RevBayesCore::DagNode *toucher ) {
    
    if ( !this->isFunctionDirty() )
    {
        // Essential for lazy evaluation
        this->touchFunction( toucher );

        // Dispatch the touch message to downstream nodes
        this->touchAffected();
    }
}


/** Print struct for user */
template<class valueType>
void DeterministicNode<valueType>::printStructureInfo( std::ostream& o ) const
{
    o << "_dagType      = Deterministic DAG node" << std::endl;
    o << "_dagAddress   = <" << this << ">" << std::endl;

    o << "_function     = " << rlFunction->getRevDeclaration() << std::endl;

    o << "_touched      = " << ( this->isFunctionDirty() ? "TRUE" : "FALSE" ) << std::endl;
    o << "_value        = " << RevBayesCore::DeterministicNode<valueType>::getValue() << std::endl;
    
    o << "_parents      = ";
    this->printParents(o);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o);
    o << std::endl;
}


#endif

