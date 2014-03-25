/**
 * @file
 * This file contains the declaration of the deterministic DAG node class, which is our base class for all deterministc DAG nodes.
 * Deterministic DAG nodes reevaluate their value when the value of their parents change. Deterministic DAG nodes are used for 
 * parameter transformations. Derived class are simple functions, e.g. exp, ln or GTR.
 *
 * @brief Declaration of the deterministic DAG node base class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id$
 */



#ifndef DeterministicNode_H
#define DeterministicNode_H

#include "DynamicNode.h"
#include "TypedFunction.h"

namespace RevBayesCore {

    template<class valueType>
    class DeterministicNode : public DynamicNode<valueType> {
    
    public:
        DeterministicNode(const std::string &n, TypedFunction<valueType> *f);
        DeterministicNode(const DeterministicNode<valueType> &n);                                                                       //!< Copy constructor
        virtual                                            ~DeterministicNode(void);                                                    //!< Virtual destructor
    
        // public methods
        DeterministicNode<valueType>*                       clone(void) const;
        virtual TypedFunction<valueType>&                   getFunction(void);
        virtual const TypedFunction<valueType>&             getFunction(void) const;
        double                                              getLnProbability(void);
        double                                              getLnProbabilityRatio(void);
        valueType&                                          getValue(void);
        const valueType&                                    getValue(void) const;
        bool                                                isConstant(void) const;                                                     //!< Is this DAG node constant?
        void                                                printStructureInfo(std::ostream &o) const;                                  //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)
        void                                                update(void);                                                               //!< Update the current value by recomputation
        void                                                redraw(void);

    protected:
        void                                                getAffected(std::set<DagNode *>& affected, DagNode* affecter);              //!< Mark and get affected nodes
        void                                                keepMe(DagNode* affecter);                                                  //!< Keep value of this and affected nodes
        void                                                restoreMe(DagNode *restorer);                                               //!< Restore value of this nodes
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                    //!< Swap the parameter of this node (needs overwriting in deterministic and stochastic nodes)
        void                                                touchMe(DagNode *toucher);                                                  //!< Tell affected nodes value is reset

    private:
        TypedFunction<valueType>*                           function;
    };
    
}


template<class valueType>
RevBayesCore::DeterministicNode<valueType>::DeterministicNode( const std::string &n, TypedFunction<valueType> *f ) : DynamicNode<valueType>( n ), function( f ) {
    
    this->type = DagNode::DETERMINISTIC;
    
    // get the parameters from the function and add them as my parents in the DAG
    const std::set<const DagNode*> funcParents = function->getParameters();
    
    for (std::set<const DagNode*>::iterator it = funcParents.begin(); it != funcParents.end(); ++it) {
        this->addParent( *it );
    }
    
    // set myself as the DAG node of the function
    function->setDeterministicNode( this );
}


template<class valueType>
RevBayesCore::DeterministicNode<valueType>::DeterministicNode( const DeterministicNode<valueType> &n ) : DynamicNode<valueType>( n ), function( n.function->clone() ) {
    
    this->type = DagNode::DETERMINISTIC;
    
    // set myself as the DAG node of the function
    function->setDeterministicNode( this );
}


template<class valueType>
RevBayesCore::DeterministicNode<valueType>::~DeterministicNode( void ) {
    // nothing to do here
    
    delete function;
}



template<class valueType>
RevBayesCore::DeterministicNode<valueType>* RevBayesCore::DeterministicNode<valueType>::clone( void ) const {
    
    return new DeterministicNode<valueType>( *this );
}

/** 
 * Get the affected nodes.
 * This call is started by the parent. We need to delegate this call to all our children.
 */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    
    this->getAffectedNodes( affected );
    
}


template<class valueType>
RevBayesCore::TypedFunction<valueType>& RevBayesCore::DeterministicNode<valueType>::getFunction( void ) {
    
    return *function;
}


template<class valueType>
const RevBayesCore::TypedFunction<valueType>& RevBayesCore::DeterministicNode<valueType>::getFunction( void ) const {
    
    return *function;
}

template<class valueType>
double RevBayesCore::DeterministicNode<valueType>::getLnProbability( void ) {
    
    return 0.0;
}


template<class valueType>
double RevBayesCore::DeterministicNode<valueType>::getLnProbabilityRatio( void ) {
    
    return 0.0;
}


template<class valueType>
valueType& RevBayesCore::DeterministicNode<valueType>::getValue( void ) {
    
    return function->getValue();
}


template<class valueType>
const valueType& RevBayesCore::DeterministicNode<valueType>::getValue( void ) const {
    
    return function->getValue();
}


template<class valueType>
bool RevBayesCore::DeterministicNode<valueType>::isConstant( void ) const {
    
    // iterate over all parents and only if all parents are constant then this node is constant too
    for (std::set<const DagNode*>::iterator it = this->parents.begin(); it != this->parents.end(); ++it) 
    {
        if ( !(*it)->isConstant() ) 
        {
            return false;
        }
    }
    
    return true;
}


/**
 * Keep the current value of the node. 
 * At this point, we just delegate to the children.
 */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::keepMe( DagNode* affecter ) {
    
    // we just mark ourselves as clean
    this->touched = false;
    
    function->keep( affecter );
    
    // delegate call
    this->keepAffected();
    
    // clear the list of touched element indices
    this->touchedElements.clear();
    
}


/** Print struct for user */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::printStructureInfo( std::ostream& o ) const
{
    
    o << "_variableType = Deterministic DAG node" << std::endl;

    if ( function->getRevDeclaration().size() == 0)
        o << "_function     = Unknown function <" << function << ">" << std::endl;
    else
        o << "_function     = " << function->getRevDeclaration() << std::endl;

    o << "_touched      = " << ( this->touched ? "TRUE" : "FALSE" ) << std::endl;
    o << "_value        = " << function->getValue() << std::endl;
    
    o << "_parents      = ";
    this->printParents(o);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o);
    o << std::endl;
}



template <class valueType>
void RevBayesCore::DeterministicNode<valueType>::update() {
    
    function->update();

}



template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::redraw( void ) {
    // nothing to do
    // the touch should have called our update
}


/** Restore the old value of the node and tell affected */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::restoreMe( DagNode *restorer ) {
    
    // we need to recompute our value?!
    this->update();
    
    // we just mark ourselves as clean
    this->touched = false;
    
    // call for potential specialized handling (e.g. internal flags)
    function->restore(restorer);
    
    // delegate call
    this->restoreAffected();
    
    // clear the list of touched element indices
    this->touchedElements.clear();
    
}



template <class valueType>
void RevBayesCore::DeterministicNode<valueType>::swapParameter(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP) {
    
    function->swapParameter(oldP, newP);
    
}



/** touch this node for recalculation */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::touchMe( DagNode *toucher ) {
    
    if (!this->touched) {
        this->touched = true;
        
    }
    
    // call for potential specialized handling (e.g. internal flags), we might have been touched already by someone else, so we need to delegate regardless
    function->touch( toucher );
    
    // @todo: until now we update directly without lazy evaluation
    update();
    
    // we call the affected nodes every time
    this->touchAffected();
    
}

#endif

