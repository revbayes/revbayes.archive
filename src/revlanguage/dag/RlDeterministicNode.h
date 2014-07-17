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


template<class valueType>
RevLanguage::DeterministicNode<valueType>::DeterministicNode( const std::string& n, RevBayesCore::TypedFunction<valueType>* fxn, RevLanguage::Function* rlFxn ) :
    RevBayesCore::DeterministicNode<valueType>( n, fxn ), rlFunction( rlFxn ) {

    touchMe( this );
}


template<class valueType>
RevLanguage::DeterministicNode<valueType>::DeterministicNode( const RevLanguage::DeterministicNode<valueType> &n )
    : RevBayesCore::DeterministicNode<valueType>( n ), rlFunction( n.rlFunction->clone() ) {

    touchMe( this );    // NB: This for safety; not needed if we trust all involved copy constructors
}


template<class valueType>
RevLanguage::DeterministicNode<valueType>::~DeterministicNode( void ) {

    delete rlFunction;
}


template<class valueType>
RevLanguage::DeterministicNode<valueType>* RevLanguage::DeterministicNode<valueType>::clone( void ) const {
    
    return new DeterministicNode<valueType>( *this );
}


template<class valueType>
RevLanguage::Function& RevLanguage::DeterministicNode<valueType>::getRlFunction( void ) {
    
    return *rlFunction;
}


template<class valueType>
const RevLanguage::Function& RevLanguage::DeterministicNode<valueType>::getRlFunction( void ) const {
    
    return *rlFunction;
}


/** Touch this node for recalculation. Here we use lazy evaluation. */
template<class valueType>
void RevLanguage::DeterministicNode<valueType>::touchMe( RevBayesCore::DagNode *toucher ) {
    
    this->touched = true;     //!< To be on the safe side; the flag is not used by this class
    
    
    // We need to touch the function anyways because it might not be filthy enough.
    // For example, the vector function wants to know if an additional elements has been touched to store the index to its touchedElementIndices.
//    if ( !this->isFunctionDirty() )
//    {
        // Essential for lazy evaluation
        this->touchFunction( toucher );

        // Dispatch the touch message to downstream nodes
        this->touchAffected();
//    }
}


/** Print struct for user */
template<class valueType>
void RevLanguage::DeterministicNode<valueType>::printStructureInfo( std::ostream& o ) const
{
    o << "_dagType      = Deterministic DAG node" << std::endl;

    o << "_function     = " << rlFunction->getRevDeclaration() << std::endl;

    o << "_touched      = " << ( this->isFunctionDirty() ? "TRUE" : "FALSE" ) << std::endl;
    o << "_value        = ";
    RevBayesCore::TypedDagNode<valueType>::printValue(o, ", ");
    o << std::endl;
    
    o << "_parents      = ";
    this->printParents(o);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o);

    o << std::endl;
}


#endif

