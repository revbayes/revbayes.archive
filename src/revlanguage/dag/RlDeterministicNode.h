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
        void                                touchMe(RevBayesCore::DagNode *toucher);                            //!< Use lazy evaluation (required by user-defined functions)
            
    protected:

    private:
        Function*                           rlFunction;                                                         //!< Rev function
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

    RevBayesCore::DagNode::touch();    // NB: This for safety; not needed if we trust all involved copy constructors
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


/**
 * Touch this node for recalculation. We use lazy evaluation but pass the touched call on
 * to our function and to downstream nodes, regardless of whether our function is dirty.
 * This is essential when functions have multiple variable arguments associated with them
 * and uses specialized code to keep track of which arguments have been touched.
 *
 * @todo We should not have to pass the message on when we have already been touched because
 *       in the next cycle of the algorithm the toucher is us, so the call is identical
 *       regardless of the toucher in the current call to touchMe.
 */
template<class valueType>
void RevLanguage::DeterministicNode<valueType>::touchMe( RevBayesCore::DagNode *toucher ) {
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In touchMe of Rev function node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    this->touched = true;     //!< To be on the safe side; the flag is not used by this class
    
    // Essential for lazy evaluation and for keeping track of the source(s) of the touch.
    this->touchFunction( toucher );
    
    // Dispatch the touch message to downstream nodes. TODO: Not needed when we are already touched.
    this->touchAffected();
}


#endif

