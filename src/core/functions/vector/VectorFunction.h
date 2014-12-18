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



#ifndef VectorFunction_H
#define VectorFunction_H

#include "RbVector.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    template <class valueType>
    class VectorFunction : public TypedFunction< RbVector<valueType> > {
        
    public:
        VectorFunction(const std::vector<const TypedDagNode<valueType> *> &args);
        virtual                                            ~VectorFunction(void);                                                       //!< Virtual destructor
        
        // public member functions
        VectorFunction*                                     clone(void) const;                                                          //!< Create an independent clone
        void                                                keep(DagNode* affecter);
        void                                                restore(DagNode *restorer);   
        void                                                touch(DagNode *toucher );
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        std::vector<const TypedDagNode<valueType>* >        vectorParams;
        
    };
    
}

template <class valueType>
RevBayesCore::VectorFunction<valueType>::VectorFunction(const std::vector<const TypedDagNode<valueType> *> &args) : TypedFunction< RbVector<valueType> >( new RbVector<valueType>() ),
    vectorParams( args )
{

    // add the lambda parameter as a parent
    typename std::vector<const TypedDagNode<valueType>* >::iterator it;
    for (it = vectorParams.begin(); it != vectorParams.end(); ++it)
    {
        this->addParameter( *it );
    }
    
    update();
}


template <class valueType>
RevBayesCore::VectorFunction<valueType>::~VectorFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



template <class valueType>
RevBayesCore::VectorFunction<valueType>* RevBayesCore::VectorFunction<valueType>::clone( void ) const
{
    return new VectorFunction<valueType>( *this );
}

template <class valueType>
void RevBayesCore::VectorFunction<valueType>::keep( DagNode *toucher )
{
    TypedFunction< RbVector<valueType> >::keep( toucher );
    this->dagNode->clearTouchedElementIndices();
}


template <class valueType>
void RevBayesCore::VectorFunction<valueType>::restore( DagNode *toucher )
{
    TypedFunction< RbVector<valueType> >::restore( toucher );
    
    this->update();
    
    this->dagNode->clearTouchedElementIndices();
}



template <class valueType>
void RevBayesCore::VectorFunction<valueType>::update( void )
{
    
    bool updateAll = true;
    if ( this->dagNode != NULL )
    {
        const std::set<size_t> &indices = this->dagNode->getTouchedElementIndices();
        if ( indices.size() > 0 )
        {
            updateAll = false;
            for (std::set<size_t>::const_iterator it = indices.begin(); it != indices.end(); ++it)
            {
                (*this->value)[*it] = vectorParams[*it]->getValue();
            }
            
        }
    }
    
    if ( updateAll )
    {
        // empty current vector
        this->value->clear();
        
        typename std::vector<const TypedDagNode<valueType>* >::iterator it;
        for (it = vectorParams.begin(); it != vectorParams.end(); ++it)
        {
            this->value->push_back( (*it)->getValue() );
        }
    }
    
}



template <class valueType>
void RevBayesCore::VectorFunction<valueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    for (size_t i = 0; i < vectorParams.size(); ++i)
    {
        if (oldP == vectorParams[i])
        {
            vectorParams[i] = static_cast<const TypedDagNode<valueType>* >( newP );
            // don't jump out of the loop because we could have the same parameter multiple times for this vector, e.g., v(a,a,b,a)
        }
    }
    
}


template <class valueType>
void RevBayesCore::VectorFunction<valueType>::touch( DagNode *toucher )
{
    
    //delegate to base class
    TypedFunction< RbVector<valueType> >::touch( toucher );
    
    for (size_t i = 0; i < vectorParams.size(); ++i)
    {
        if (toucher == vectorParams[i]) 
        {
            this->dagNode->addTouchedElementIndex( i );
            // don't jump out of the loop because we could have the same parameter multiple times for this vector, e.g., v(a,a,b,a)
        }
    }
    
}

#endif
