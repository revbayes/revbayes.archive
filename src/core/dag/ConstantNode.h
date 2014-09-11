#ifndef ConstantNode_H
#define ConstantNode_H

#include "StringUtilities.h"
#include "AbstractConstantNode.h"
#include "UnivariateFunction.h"

namespace RevBayesCore {
    
    /**
     * @brief Constant DAG node class
     *
     * This is the class for constant DAG nodes holding fixed parameters of a model
     * in the core layer.
     */
    template<class valueType>
    class ConstantNode : public AbstractConstantNode<valueType> {
    
    public:
        ConstantNode(const std::string &n, valueType *v);                                                                               //!< Constructor from value
        ConstantNode(const ConstantNode<valueType> &c);                                                                                 //!< Copy constructor

        virtual                                            ~ConstantNode(void);                                                         //!< Virtual destructor
    
        // Functions implemented here
        ConstantNode<valueType>*                            clone(void) const;                                                          //!< Create a clone of this node.
        valueType&                                          getValue(void);                                                             //!< Get value
        const valueType&                                    getValue(void) const;                                                       //!< Get value (const version)
        void                                                printStructureInfo(std::ostream &o, bool verbose=false) const;              //!< Print structure info for user

        // This function should not be needed
//        void                                                setValue(const valueType& v);                                               //!< Change the value of the node
        
    private:
        // Member variable
        valueType*                                          value;                                                                      //!< Value of the node

    };
    
}


#include "RbException.h"


/**
 * Constructor from pointer to value. We assume ownership of the value.
 */
template<class valueType>
RevBayesCore::ConstantNode<valueType>::ConstantNode(const std::string &n, valueType *v) :
    AbstractConstantNode<valueType>( n ),
    value( v )
{
}


/**
 * Copy constructor. Here we use the Cloner functionality to clone the value of the
 * argument.
 */
template<class valueType>
RevBayesCore::ConstantNode<valueType>::ConstantNode(const ConstantNode<valueType> &c) :
    AbstractConstantNode<valueType>( c ),
    value( Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( *c.value ) )
{
}


/** Destructor. We own the value so we need to delete it. */
template<class valueType>
RevBayesCore::ConstantNode<valueType>::~ConstantNode( void )
{
    // We own the value so we need to delete it here
    delete value;
}


/** Type-safe clone of the node */
template<class valueType>
RevBayesCore::ConstantNode<valueType>* RevBayesCore::ConstantNode<valueType>::clone( void ) const
{
    return new ConstantNode<valueType>( *this );
}


/** Get value (non-const version) */
template<class valueType>
valueType& RevBayesCore::ConstantNode<valueType>::getValue( void )
{
    return *value;
}


/** Get value (const version) */
template<class valueType>
const valueType& RevBayesCore::ConstantNode<valueType>::getValue( void ) const
{
    return *value;
}


/** Print struct for user */
void RevBayesCore::ConstantNode<valueType>::printStructureInfo(std::ostream &o, bool verbose) const
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


#if 0
/**
 * Public function to set the value. We immediately accept the new state
 * by issuing the keep message right after the touch message to any downstream
 * DAG nodes.
 *
 * @todo Is this needed? Does it work for abstract types?
 */
template<class valueType>
void RevBayesCore::ConstantNode<valueType>::setValue( valueType const &v )
{
    *value = v;

    this->touch();

    this->keep();
}
#endif


#endif

