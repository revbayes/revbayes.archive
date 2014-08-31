#ifndef NAValueNode_H
#define NAValueNode_H

#include "ConstantNode.h"
#include "StringUtilities.h"
#include "TypedDagNode.h"
#include "UnivariateFunction.h"

namespace RevLanguage {
    
    template<class rlType>
    class NAValueNode : public RevBayesCore::TypedDagNode<typename rlType::valueType> {

        typedef typename rlType::valueType                  valueType;

    public:
        NAValueNode(const std::string &n = "");                                                                                     // Basic constructor
        NAValueNode(const NAValueNode<rlType> &c);                                                                                  //!< Copy constructor
        virtual                                            ~NAValueNode(void);                                                      //!< Virtual destructor
        
        NAValueNode<rlType>*                                clone(void) const;                                                      //!< Create a clone of this node.
        RevBayesCore::DagNode*                              cloneDAG(std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*> &nodesMap) const;   //!< Clone the entire DAG connected to this node
        valueType&                                          getValue(void);                                                         //!< Get value (throw error)
        const valueType&                                    getValue(void) const;                                                   //!< Get value (throw error) (const)
        bool                                                isConstant(void) const;                                                 //!< Is this DAG node constant?
        bool                                                isNAValue(void) const;                                                  //!< Is this an NA value node?
        void                                                printStructureInfo(std::ostream &o, bool verbose=false) const;          //!< Print structure info
        void                                                printValue(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const;    //!< Monitor/Print this variable
        void                                                setValue(const valueType &v);                                           //!< Stop attempts to set value of this node
        
    protected:
        void                                                keepMe(RevBayesCore::DagNode* affecter);                                //!< Keep value of this and affected nodes
        void                                                restoreMe(RevBayesCore::DagNode *restorer);                             //!< Restore value of this nodes
        void                                                touchMe(RevBayesCore::DagNode *toucher);                                //!< Tell affected nodes value is reset
    };
    
}


#include "RbException.h"

using namespace RevLanguage;

/** Standard constructor. We set value to NULL */
template<class rlType>
NAValueNode<rlType>::NAValueNode(const std::string &n) :
    RevBayesCore::TypedDagNode<typename rlType::valueType>( n )
{
    this->type = RevBayesCore::DagNode::CONSTANT;
}


/** Copy constructor */
template<class rlType>
NAValueNode<rlType>::NAValueNode(const NAValueNode<rlType> &c) :
    RevBayesCore::TypedDagNode<typename rlType::valueType>( c )
{
    this->type = RevBayesCore::DagNode::CONSTANT;
}


/**
 * Nothing to be done in the destructor; no value to delete. Deletion of
 * the null value pointer in the ConstantNode base class should not cause
 * any problems.
 */
template<class rlType>
NAValueNode<rlType>::~NAValueNode( void )
{
}


/**
 * Type-safe clone function. We use our own copy constructor, which deals correctly with
 * the NULL value pointer.
 */
template<class rlType>
NAValueNode<rlType>* NAValueNode<rlType>::clone( void ) const
{
    return new NAValueNode<rlType>( *this );
}


/** Cloning the entire graph only involves children for a NA value node */
template<class rlType>
RevBayesCore::DagNode* NAValueNode<rlType>::cloneDAG( std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode* >& newNodes ) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    /* Make pristine copy */
    NAValueNode* copy = clone();
    newNodes[ this ] = copy;
    
    /* Make sure the children clone themselves */
    for( std::set<RevBayesCore::DagNode* >::const_iterator i = this->children.begin(); i != this->children.end(); i++ )
    {
        (*i)->cloneDAG( newNodes );
    }
    
    return copy;
}


/**
 * You cannot get the value of an NA object, so we throw an error in
 * the getValue() function.
 */
template<class rlType>
typename rlType::valueType& NAValueNode<rlType>::getValue( void )
{
    throw RbException( "Invalid attempt to get value from NA object" );
}


/**
 * You cannot get the value of an NA object, so we throw an error in
 * the getValue() function (const version).
 */
template<class rlType>
const typename rlType::valueType& NAValueNode<rlType>::getValue( void ) const
{
    throw RbException( "Invalid attempt to get value from NA object" );
}


/** Is this a constant value? */
template<class rlType>
bool NAValueNode<rlType>::isConstant( void ) const {
    
    return true;
}


/** Is this an NA value? */
template<class rlType>
bool NAValueNode<rlType>::isNAValue( void ) const {
    
    return true;
}


template<class rlType>
void NAValueNode<rlType>::keepMe( RevBayesCore::DagNode* affecter ) {
    // nothing to do
}


/** Print struct for user */
template<class rlType>
void NAValueNode<rlType>::printStructureInfo(std::ostream &o, bool verbose) const
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
    o << "_dagType      = NA value DAG node" << std::endl;
    
    if ( verbose )
        o << "_refCount     = " << this->getReferenceCount() << std::endl;
    
    if ( verbose )
    {
        o << "_parents      = ";
        this->printParents( o, 16, 70, verbose );
        o << std::endl;
    }
    
    o << "_children     = ";
    this->printChildren( o, 16, 70, verbose );
    o << std::endl;
}


/**
 * Print value. We override this function to print "NA".
 */
template<class rlType>
void NAValueNode<rlType>::printValue( std::ostream &o, const std::string &sep, int l, bool left ) const
{
    std::stringstream ss;
    ss << "NA";
    std::string s = ss.str();
    if ( l > 0 )
    {
        StringUtilities::fillWithSpaces(s, l, left);
    }
    o << s;
}


template<class rlType>
void NAValueNode<rlType>::restoreMe( RevBayesCore::DagNode *restorer ) {
    // nothing to do
}


/** Throw an error if someone tries to set our value */
template<class rlType>
void NAValueNode<rlType>::setValue( typename rlType::valueType const &v)
{
    throw RbException( "Invalid attempt to set value of NA object" );
}


template<class rlType>
void NAValueNode<rlType>::touchMe( RevBayesCore::DagNode *toucher ) {
    // nothing to do
}


#endif

