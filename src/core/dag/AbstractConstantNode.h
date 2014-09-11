#ifndef AbstractConstantNode_H
#define AbstractConstantNode_H

#include "StringUtilities.h"
#include "TypedDagNode.h"
#include "UnivariateFunction.h"

namespace RevBayesCore {
    
    /**
     * @brief Abstract base class for constant DAG nodes
     *
     * This is the base class for constant DAG nodes holding fixed parameters of a model,
     * or parameters with NA values.
     */
    template<class valueType>
    class AbstractConstantNode : public TypedDagNode<valueType> {
        
    public:
        virtual                                            ~AbstractConstantNode(void);                                                 //!< Virtual destructor
        
        // Functions you have to override
        virtual AbstractConstantNode<valueType>*            clone(void) const = 0;                                                      //!< Create a clone of this node.
        virtual void                                        printStructureInfo(std::ostream &o, bool verbose=false) const = 0;          //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)

        // Functions implemented here
        DagNode*                                            cloneDAG(std::map<const DagNode*, DagNode*> &nodesMap) const;               //!< Clone the entire DAG which is connected to this node
        bool                                                isConstant(void) const;                                                     //!< Is this DAG node constant?
        
    protected:
        // Helper functions for DAG touch/keep/restore messages
        void                                                keepMe(DagNode* affecter);                                                  //!< Keep value of this and affected nodes
        void                                                restoreMe(DagNode *restorer);                                               //!< Restore value of this nodes
        void                                                touchMe(DagNode *toucher);                                                  //!< Tell affected nodes value is reset
        
    };
    
}


#include "RbException.h"



/**
 * Clone the entire graph. We clone ourselves if we are not already cloned, and then simply
 * ask our children to clone themselves.
 */
template<class valueType>
RevBayesCore::DagNode* RevBayesCore::AbstractConstantNode<valueType>::cloneDAG( std::map<const DagNode*, DagNode* >& newNodes ) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    /* Make pristine copy */
    AbstractConstantNode* copy = clone();
    newNodes[ this ] = copy;
    
    /* Make sure the children clone themselves */
    for( std::set<DagNode* >::const_iterator i = this->children.begin(); i != this->children.end(); i++ )
    {
        (*i)->cloneDAG( newNodes );
    }
    
    return copy;
}


/** Is this node constant? We return true */
template<class valueType>
bool RevBayesCore::AbstractConstantNode<valueType>::isConstant( void ) const
{
    return true;
}


/** Keep this node. Nothing to do (we should never receive this call) */
template<class valueType>
void RevBayesCore::AbstractConstantNode<valueType>::keepMe( DagNode* affecter )
{
    // nothing to do
}


template<class valueType>
/** Print struct for user */
void RevBayesCore::AbstractConstantNode<valueType>::printStructureInfo(std::ostream &o, bool verbose) const
{
    if ( verbose == true )
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
    
    o << "_dagType      = Constant DAG node" << std::endl;
    
    if ( verbose == true )
    {
        o << "_refCount     = " << this->getReferenceCount() << std::endl;
    }
    
    if ( verbose == true )
    {
        o << "_parents      = ";
        this->printParents(o, 16, 70, verbose);
        o << std::endl;
    }
    
    o << "_children     = ";
    this->printChildren(o, 16, 70, verbose);
    o << std::endl;
}


/** Restore me. Nothing to do (we should never receive this call) */
template<class valueType>
void RevBayesCore::AbstractConstantNode<valueType>::restoreMe( DagNode *restorer )
{
    // nothing to do
}


/** Touch me. Nothing to do (we should never receive this call) */
template<class valueType>
void RevBayesCore::AbstractConstantNode<valueType>::touchMe( DagNode *toucher )
{
    // nothing to do
}

#endif

