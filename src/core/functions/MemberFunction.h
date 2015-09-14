#ifndef MemberFunction_H
#define MemberFunction_H

#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    template<class valueType>
    class MemberObject;
    
    
    /**
     * \brief Member function that maps a function call to a method of a member object.
     *
     * A member function is a function that is used inside deterministic nodes and provides
     * access to a member method of a member object.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2014-07-17
     *
     */
    template<class memberObjectType, class valueType>
    class MemberFunction : public TypedFunction<valueType> {
        
    public:
        // constructors and destructor
        MemberFunction(const std::string &n, const TypedDagNode<memberObjectType> *o, const std::vector<const DagNode* > &a);
//        MemberFunction(const MemberFunction &f);
        virtual                                    ~MemberFunction(void);

        // overloaded operators
//        MemberFunction&                             operator=(const MemberFunction &d);
        
        // pure virtual public methors
        MemberFunction<memberObjectType,valueType>* clone(void) const;                                                              //!< Clone the function
        void                                        update(void);                                                                   //!< Update the value of the function
        
    protected:
        
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters

    private:
        
        std::string                                 methodName;
        const TypedDagNode<memberObjectType>*       theMemberVariable;
        std::vector<const DagNode* >                args;
    };
    
}

#include "Cloneable.h"
#include "Cloner.h"
#include "IsDerivedFrom.h"


template <class memberObjectType, class valueType>
RevBayesCore::MemberFunction<memberObjectType,valueType>::MemberFunction(const std::string &n, const TypedDagNode<memberObjectType> *o, const std::vector<const DagNode* > &a) : TypedFunction<valueType>( new valueType() ),
    methodName( n ),
    theMemberVariable( o ),
    args( a )
{
    
    this->addParameter( theMemberVariable );
    typename std::vector<const DagNode* >::iterator it;
    for (it = args.begin(); it != args.end(); ++it)
    {
        this->addParameter( *it );
    }
    
}

template <class memberObjectType, class valueType>
RevBayesCore::MemberFunction<memberObjectType,valueType>::~MemberFunction( void )
{
    
}



template <class memberObjectType, class valueType>
RevBayesCore::MemberFunction<memberObjectType,valueType>* RevBayesCore::MemberFunction<memberObjectType, valueType>::clone( void ) const
{
    
    return new MemberFunction<memberObjectType,valueType>(*this);
}



template <class memberObjectType, class valueType>
void RevBayesCore::MemberFunction<memberObjectType,valueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( theMemberVariable == oldP )
    {
        theMemberVariable = static_cast< const TypedDagNode<memberObjectType>* > (newP);
    }
    else
    {
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (oldP == args[i])
            {
                args[i] = newP;
                // we can jump out of the loop now
                break;
            }
        }
    }
    
}



template <class memberObjectType, class valueType>
void RevBayesCore::MemberFunction<memberObjectType,valueType>::update( void )
{
    
    const MemberObject<valueType>& theMemberObject = dynamic_cast<const MemberObject<valueType>& >( theMemberVariable->getValue() );
    theMemberObject.executeMethod(methodName,args,*this->value);
    
}


#endif
