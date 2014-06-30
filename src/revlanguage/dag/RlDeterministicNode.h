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
     * RlDeterministicNode.h is used in the Rev language layer to represent deterministic nodes. It differs from the class used for
     * deterministic nodes in the core layer (DeterministicNode) in that it keeps a pointer to the Rev function associated with the
     * deterministic node. This is essential for userdefined functions but it is also practical for all deterministic node handled
     * in the Rev layer. For instance, the Rev function object is the most natural place to keep information about the name of the
     * Rev function, its orgin, and its code. The Rev function object also keeps the arguments of the function, which can be
     * accessed by their argument names through the Rev function object.
     */

    template<class valueType>
    class DeterministicNode : public RevBayesCore::DeterministicNode<valueType> {
    
    public:
                                                            DeterministicNode(const std::string &n, Function *f);
                                                            DeterministicNode(const DeterministicNode<valueType> &n);
        virtual                                            ~DeterministicNode(void);                                                    //!< Virtual destructor
    
        // public methods
        DeterministicNode<valueType>*                       clone(void) const;
        Function<valueType>&                                getRlFunction(void);
        const Function<valueType>&                          getRlFunction(void) const;
        void                                                printStructureInfo(std::ostream &o) const;                                  //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)

    protected:

    private:
        Function*                                           rlFunction;
    };
    
}

using namespace RevLanguage;

template<class valueType>
DeterministicNode<valueType>::DeterministicNode( const std::string &n, RevBayesCore::Function* fxn, Function* rlFxn ) :
    RevBayesCore::DeterministicNode<valueType>( n, fxn ), rlFunction( rlFxn ) {
    
}


template<class valueType>
DeterministicNode<valueType>::DeterministicNode( const DeterministicNode<valueType> &x )
    : RevBayesCore::DeterministicNode<valueType>( x ), rlFunction( x.rlFunction->clone() ) {
    
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


/** Print struct for user */
template<class valueType>
void DeterministicNode<valueType>::printStructureInfo( std::ostream& o ) const
{
    
    o << "_dagType      = Deterministic DAG node" << std::endl;

    o << "_function     = " << rlFunction->getRevDeclaration() << std::endl;

    o << "_touched      = " << ( this->touched ? "TRUE" : "FALSE" ) << std::endl;
    o << "_value        = " << getValue() << std::endl;
    
    o << "_parents      = ";
    this->printParents(o);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o);
    o << std::endl;
}


#endif

