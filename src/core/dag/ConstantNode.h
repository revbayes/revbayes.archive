/**
 * @file
 * This file contains the declaration of the constant DAG node class, which is our DAG node class holding fixed parameters of a model.
 *
 * @brief Declaration of the constant DAG node class.
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

#ifndef ConstantNode_H
#define ConstantNode_H

#include "StringUtilities.h"
#include "TypedDagNode.h"
#include "UnivariateFunction.h"

namespace RevBayesCore {
    
    template<class valueType>
    class ConstantNode : public TypedDagNode<valueType> {
        
    public:
        ConstantNode(const std::string &n, valueType *v);
        ConstantNode(const ConstantNode<valueType> &c);                                                                                 //!< Copy constructor
        virtual                                            ~ConstantNode(void);                                                         //!< Virtual destructor
        
        ConstantNode<valueType>*                            clone(void) const;                                                          //!< Create a clone of this node.
        DagNode*                                            cloneDAG(std::map<const DagNode*, DagNode*> &nodesMap, std::map<std::string, const DagNode* > &names) const; //!< Clone the entire DAG which is connected to this node
        double                                              getLnProbability(void);
        double                                              getLnProbabilityRatio(void);
        valueType&                                          getValue(void);
        const valueType&                                    getValue(void) const;
        bool                                                isConstant(void) const;                                                     //!< Is this DAG node constant?
        void                                                printStructureInfo(std::ostream &o, bool verbose=false) const;              //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)
        void                                                redraw(void);
        void                                                setValue(const valueType &v);
        
    protected:
        void                                                getAffected(std::set<DagNode *>& affected, DagNode* affecter);              //!< Mark and get affected nodes
        void                                                keepMe(DagNode* affecter);                                                  //!< Keep value of this and affected nodes
        void                                                restoreMe(DagNode *restorer);                                               //!< Restore value of this nodes
        void                                                touchMe(DagNode *toucher);                                                  //!< Tell affected nodes value is reset
        
    private:
        // members
        valueType*                                          value;
        
    };
    
    
}


#include "RbException.h"


template<class valueType>
RevBayesCore::ConstantNode<valueType>::ConstantNode(const std::string &n, valueType *v) : TypedDagNode<valueType>( n ), value( v ) {
    
    this->type = DagNode::CONSTANT;
    
}


template<class valueType>
RevBayesCore::ConstantNode<valueType>::ConstantNode(const ConstantNode<valueType> &c) : TypedDagNode<valueType>( c ), value( Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( *c.value ) ) {
    
    this->type = DagNode::CONSTANT;
    
}

template<class valueType>
RevBayesCore::ConstantNode<valueType>::~ConstantNode( void ) {
    
    // we own the value so we need to delete it here
    delete value;
    
}


/* Clone this node by creating an independent copy of the value. */
template<class valueType>
RevBayesCore::ConstantNode<valueType>* RevBayesCore::ConstantNode<valueType>::clone( void ) const {
    
    return new ConstantNode<valueType>( *this );
    
}


/** Cloning the entire graph only involves children for a constant node */
template<class valueType>
RevBayesCore::DagNode* RevBayesCore::ConstantNode<valueType>::cloneDAG( std::map<const DagNode*, DagNode* >& newNodes, std::map<std::string, const DagNode* > &names ) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
    {
        return ( newNodes[ this ] );
    }
    
    // just for self checking purposes we keep track of the names for the variables we already cloned
    if ( this->name != "" )
    {
        // check if we already added a variable with this name
        std::map<std::string, const DagNode* >::const_iterator n = names.find( this->name );
        if ( n == names.end() )
        {
            // no, we haven't cloned a variable with this name before
            names[ this->name ] = this;
        }
        else
        {
            std::cerr << "Cloning a DAG node with name '" << this->name << "' again, doh!" << std::endl;
        }
    }
    
    /* Make pristine copy */
    ConstantNode* copy = clone();
    newNodes[ this ] = copy;
    
    /* Make sure the children clone themselves */
    for( std::set<DagNode* >::const_iterator i = this->children.begin(); i != this->children.end(); i++ )
    {
        (*i)->cloneDAG( newNodes, names );
    }
    
    return copy;
}


/**
 * Get the affected nodes.
 * This call is started by the parent and since we don't have one this is a dummy implementation!
 */
template<class valueType>
void RevBayesCore::ConstantNode<valueType>::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    
    // do nothing
    throw RbException("You should never call getAffected() of a constant node!!!");
    
}


template<class valueType>
double RevBayesCore::ConstantNode<valueType>::getLnProbability( void ) {
    
    return 0.0;
}


template<class valueType>
double RevBayesCore::ConstantNode<valueType>::getLnProbabilityRatio( void ) {
    
    return 0.0;
}


template<class valueType>
valueType& RevBayesCore::ConstantNode<valueType>::getValue( void ) {
    
    return *value;
}


template<class valueType>
const valueType& RevBayesCore::ConstantNode<valueType>::getValue( void ) const {
    
    return *value;
}


template<class valueType>
bool RevBayesCore::ConstantNode<valueType>::isConstant( void ) const {
    
    return true;
}


template<class valueType>
void RevBayesCore::ConstantNode<valueType>::keepMe( DagNode* affecter )
{
    // nothing to do
}


template<class valueType>
/** Print struct for user */
void RevBayesCore::ConstantNode<valueType>::printStructureInfo(std::ostream &o, bool verbose) const
{
    
    if ( verbose == true )
    {
        o << "_dagNode      = " << this->name << " <" << this << ">" << std::endl;
    }
    
    o << "_dagType      = Constant DAG node" << std::endl;
    
    if ( verbose == true )
    {
        o << "_refCount     = " << this->getReferenceCount() << std::endl;
    }
    
    o << "_children     = ";
    this->printChildren(o, 16, 70, verbose);
    o << std::endl;
}


template<class valueType>
void RevBayesCore::ConstantNode<valueType>::redraw( void ) {
    // nothing to do
}


template<class valueType>
void RevBayesCore::ConstantNode<valueType>::restoreMe( DagNode *restorer ) {
    // nothing to do
}


template<class valueType>
void RevBayesCore::ConstantNode<valueType>::setValue(valueType const &v) {
    
    *value = v;
    this->touch();
    
}


template<class valueType>
void RevBayesCore::ConstantNode<valueType>::touchMe( DagNode *toucher ) {
    // nothing to do
}

#endif

