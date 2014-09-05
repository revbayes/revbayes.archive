#ifndef DistributionMemberFunction_H
#define DistributionMemberFunction_H

#include "TypedFunction.h"

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
    class DistributionMemberFunction : public TypedFunction<valueType> {
        
    public:
        // constructors and destructor
        DistributionMemberFunction(const std::string &n, const memberObjectType *o, const std::vector<const DagNode* > &a);
        //        DistributionMemberFunction(const DistributionMemberFunction &f);
        virtual                                    ~DistributionMemberFunction(void);
        
        // overloaded operators
        //        DistributionMemberFunction&                             operator=(const DistributionMemberFunction &d);
        
        // pure virtual public methors
        DistributionMemberFunction<memberObjectType,valueType>* clone(void) const;                                                              //!< Clone the function
        void                                        update(void);                                                                   //!< Update the value of the function
        
    protected:
        
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        std::string                                 methodName;
        const memberObjectType*                     theMemberVariable;
        std::vector<const DagNode* >                args;
    };
    
}

#include "Cloneable.h"
#include "Cloner.h"
#include "IsDerivedFrom.h"


template <class memberObjectType, class valueType>
RevBayesCore::DistributionMemberFunction<memberObjectType,valueType>::DistributionMemberFunction(const std::string &n, const memberObjectType *o, const std::vector<const DagNode* > &a) : TypedFunction<valueType>( new valueType() ),
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

//template <class memberObjectType, class valueType>
//RevBayesCore::DistributionMemberFunction<memberObjectType,valueType>::DistributionMemberFunction(const DistributionMemberFunction &f) : TypedFunction<valueType>(f),
//    methodName( f.methodName ),
//    theMemberVariable( f.theMemberVariable ),
//    args( f.args )
//{
//
////    if ( f.theMemberObject != NULL )
////    {
////        theMemberObject = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( *f.theMemberObject );
////    }
//
//}

template <class memberObjectType, class valueType>
RevBayesCore::DistributionMemberFunction<memberObjectType,valueType>::~DistributionMemberFunction( void )
{
    
    //    delete theMemberObject;
}


//template <class memberObjectType, class valueType>
//RevBayesCore::DistributionMemberFunction<memberObjectType,valueType>& RevBayesCore::DistributionMemberFunction<memberObjectType,valueType>::operator=(const DistributionMemberFunction &f)
//{
//
//    if ( this != &f )
//    {
//        // call base class
//        TypedFunction<valueType>::operator=( f );
//
////        // make my own copy of the value (we rely on proper implementation of assignment operators)
////        delete theMemberObject;
////        theMemberObject = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( *f.theMemberObject );
//
//    }
//
//    return *this;
//}



template <class memberObjectType, class valueType>
RevBayesCore::DistributionMemberFunction<memberObjectType,valueType>* RevBayesCore::DistributionMemberFunction<memberObjectType, valueType>::clone( void ) const
{
    
    return new DistributionMemberFunction<memberObjectType,valueType>(*this);
}



template <class memberObjectType, class valueType>
void RevBayesCore::DistributionMemberFunction<memberObjectType,valueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( theMemberVariable == oldP )
    {
        theMemberVariable = static_cast< const memberObjectType* > (newP);
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
void RevBayesCore::DistributionMemberFunction<memberObjectType,valueType>::update( void )
{
    
    const MemberObject<valueType>& theMemberObject = dynamic_cast<const MemberObject<valueType>& >( theMemberVariable );
    theMemberObject.executeMethod(methodName,args,*this->value);
    
}


#endif
