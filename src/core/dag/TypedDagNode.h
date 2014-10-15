/**
 * @file
 * This file contains the declaration of the typed DAG node class, which is our base class for all DAG node with a specific type.
 * This class is used as the typed base class for DAG nodes as in cases as when parameters (parents) of other DAG nodes are set.
 *
 * @brief Declaration of the typed base class DagNode.
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



#ifndef TypedDagNode_H
#define TypedDagNode_H

#include "DagNode.h"
#include "RbUtil.h"

#include <ostream>
#include <string>

namespace RevBayesCore {
    
    template<class valueType>
    class TypedDagNode : public DagNode {
    
    public:
        TypedDagNode(const std::string &n);
        virtual                                            ~TypedDagNode(void);                                                                         //!< Virtual destructor
    
        // pure virtual methods
        virtual TypedDagNode<valueType>*                    clone(void) const = 0;

        // member functions
        virtual size_t                                      getNumberOfElements(void) const;                                                            //!< Get the number of elements for this value
        virtual bool                                        isSimpleNumeric(void) const;                                                                //!< Is this variable a simple numeric variable? Currently only integer and real number are.
        virtual void                                        printName(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const;         //!< Monitor/Print this variable
        virtual void                                        printValue(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const;        //!< Monitor/Print this variable
//        virtual void                                        printValue(std::ostream &o, size_t i, size_t l=12, bool left=false) const;                  //!< Monitor/Print the i-th element of this variable

        // getters and setters
        virtual valueType&                                  getValue(void) = 0;
        virtual const valueType&                            getValue(void) const = 0;

        
    };
    
    
    template<>
    inline bool                                  TypedDagNode<int>::isSimpleNumeric(void) const { return true; } 
    
    template<>
    inline bool                                  TypedDagNode<double>::isSimpleNumeric(void) const { return true; }

    template<>
    inline bool                                  TypedDagNode<RbVector<int> >::isSimpleNumeric(void) const { return true; }
    
    template<>
    inline bool                                  TypedDagNode<RbVector<double> >::isSimpleNumeric(void) const { return true; }

}

#include "RbUtil.h"
#include "StringUtilities.h"

#include <vector>


template<class valueType>
RevBayesCore::TypedDagNode<valueType>::TypedDagNode(const std::string &n) : DagNode( n ) {
    
}


template<class valueType>
RevBayesCore::TypedDagNode<valueType>::~TypedDagNode( void ) {
}



template<class valueType>
size_t RevBayesCore::TypedDagNode<valueType>::getNumberOfElements( void ) const
{
    
    size_t numElements = RbUtils::sub_vector<valueType>::size( getValue() );
    
    return numElements;
}



template<class valueType>
bool RevBayesCore::TypedDagNode<valueType>::isSimpleNumeric( void ) const
{
    return false;
}


template<class valueType>
void RevBayesCore::TypedDagNode<valueType>::printName(std::ostream &o, const std::string &sep, int l, bool left) const
{
    
    if ( RbUtils::is_vector<valueType>::value ) 
    {
        size_t numElements = RbUtils::sub_vector<valueType>::size( getValue() );
        for (size_t i = 0; i < numElements; ++i) 
        {
            if ( i > 0 ) 
            {
                o << sep;
            }
            std::stringstream ss;
            ss << getName() << "[" << (i+1) << "]";
            std::string n = ss.str();
            if ( l > 0)
            {
                StringUtilities::fillWithSpaces(n, l, left);
            }
            o << n;
        }
    } 
    else 
    {
        std::string n = getName();
        if ( l > 0 )
        {
            StringUtilities::fillWithSpaces(n, l, left);
        }
        o << n;
    }
}


template<class valueType>
void RevBayesCore::TypedDagNode<valueType>::printValue(std::ostream &o, const std::string &sep, int l, bool left) const
{
    
    if ( RbUtils::is_vector<valueType>::value ) 
    {
        size_t numElements = RbUtils::sub_vector<valueType>::size( getValue() );
        for (size_t i = 0; i < numElements; ++i) 
        {
            if ( i > 0 ) 
            {
                o << sep;
            }
            std::stringstream ss;
            RbUtils::sub_vector<valueType>::printElement( ss, getValue(), i );
            std::string s = ss.str();
            if ( l > 0 )
            {
                StringUtilities::fillWithSpaces(s, l, left);
            }
            o << s;
        }
    } 
    else 
    {
        std::stringstream ss;
        ss << getValue();
        std::string s = ss.str();
        if ( l > 0 )
        {
            StringUtilities::fillWithSpaces(s, l, left);
        }
        o << s;
    }
}


//template<class valueType>
//void RevBayesCore::TypedDagNode<valueType>::printValue(std::ostream &o, size_t index) const {
//    
//    if ( RbUtils::is_vector<valueType>::value ) 
//    {
//        o << RbUtils::sub_vector<valueType>::getElement( getValue(), index );
//    } 
//    else 
//    {
//        o << getValue();
//    }
//    
//}


//template <typename T, typename Alloc>
//void RevBayesCore::TypedDagNode<std::vector<T,Alloc> >::printValue(std::ostream &o, size_t index) const {
////    if ( is_vector<valueType>::value ) {
//        o << getValue()[index];
////    } else {
////        o << getValue();
////    }
//}

#endif

