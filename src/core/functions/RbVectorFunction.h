/**
 * @file
 * This file contains the declaration of the deterministic variable class for Vectors.
 * This class is derived from the deterministic node and each instance will represent a deterministic variable
 * computing the Vector of its parameters.
 *
 * @brief Declaration of the deterministic variable for Vectors.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-06, version 1.0
 * @interface TypedDagNode
 *
 * $Id$
 */



#ifndef RbVectorFunction_H
#define RbVectorFunction_H

#include "RbVector.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    template <class valueType>
    class RbVectorFunction : public TypedFunction< RbVector<valueType> > {
        
    public:
        RbVectorFunction(const std::vector<const TypedDagNode<valueType> *> &args);
        RbVectorFunction(const RbVectorFunction &n);                                                                                        //!< Copy constructor
        virtual                                            ~RbVectorFunction(void);                                                       //!< Virtual destructor
        
        // public member functions
        RbVectorFunction*                                   clone(void) const;                                                          //!< Create an independent clone
        void                                                keep(DagNode* affecter);
        void                                                restore(DagNode *restorer);   
        void                                                touch(DagNode *toucher );
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        std::vector<const TypedDagNode<valueType>* >        parameters;
        
    };
    
}

template <class valueType>
RevBayesCore::RbVectorFunction<valueType>::RbVectorFunction(const std::vector<const TypedDagNode<valueType> *> &args) : TypedFunction< RbVector<valueType> >( new RbVector<valueType>() ), parameters( args ) {
    // add the lambda parameter as a parent
    typename std::vector<const TypedDagNode<valueType>* >::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it)
    {
        this->addParameter( *it );
    }
    
    update();
}


template <class valueType>
RevBayesCore::RbVectorFunction<valueType>::RbVectorFunction(const RbVectorFunction<valueType> &n) : TypedFunction< RbVector<valueType> >( n ), parameters( n.parameters ) {
    // no need to add parameters, happens automatically
    
    update();
}


template <class valueType>
RevBayesCore::RbVectorFunction<valueType>::~RbVectorFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



template <class valueType>
RevBayesCore::RbVectorFunction<valueType>* RevBayesCore::RbVectorFunction<valueType>::clone( void ) const
{
    return new RbVectorFunction<valueType>( *this );
}

template <class valueType>
void RevBayesCore::RbVectorFunction<valueType>::keep( DagNode *toucher )
{
    this->dagNode->clearTouchedElementIndices();
}


template <class valueType>
void RevBayesCore::RbVectorFunction<valueType>::restore( DagNode *toucher )
{
    this->dagNode->clearTouchedElementIndices();
}



template <class valueType>
void RevBayesCore::RbVectorFunction<valueType>::update( void )
{
    
    // empty current vector
    this->value->clear();
    
    typename std::vector<const TypedDagNode<valueType>* >::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it)
    {
        this->value->push_back( (*it)->getValue() );
    }
}



template <class valueType>
void RevBayesCore::RbVectorFunction<valueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    for (size_t i = 0; i < parameters.size(); ++i)
    {
        
        if (oldP == parameters[i])
        {
            parameters[i] = static_cast<const TypedDagNode<valueType>* >( newP );
            // we can jump out of the loop now
            break;
        }
        
    }
    
}


template <class valueType>
void RevBayesCore::RbVectorFunction<valueType>::touch( DagNode *toucher )
{
    RevBayesCore::TypedFunction< RbVector<valueType> >::touch( toucher );
    
    for (size_t i = 0; i < parameters.size(); ++i)
    {
        if (toucher == parameters[i])
        {
            this->dagNode->addTouchedElementIndex( i );
            // we can jump out of the loop now
            break;
        }
    }
}

#endif
