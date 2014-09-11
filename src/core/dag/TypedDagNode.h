#ifndef TypedDagNode_H
#define TypedDagNode_H

#include "DagNode.h"
#include "RbUtil.h"

#include <ostream>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief Template base class for DAG nodes
     *
     * The TypedDagNode class is used as the templated base class for DAG nodes. It is templated on the value type, which can be
     * a core C++ data type or an STL container of a core C++ data type. It can also be a datatype implemented in the core layer,
     * and derived from the Cloneable base class.
     */
    template<class valueType>
    class TypedDagNode : public DagNode {
    
    public:
        TypedDagNode(const std::string &n);                                                                                                             //!< Construct from name
        virtual                                            ~TypedDagNode(void);                                                                         //!< Virtual destructor
    
        // Pure virtual function
        virtual TypedDagNode<valueType>*                    clone(void) const = 0;                                                                      //!< Type-safe clone (abstract)

        // Member functions
        virtual size_t                                      getNumberOfElements(void) const;                                                            //!< Get the number of elements for this value
        virtual bool                                        isSimpleNumeric(void) const;                                                                //!< Is this variable a simple numeric variable?
        virtual void                                        printName(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const;         //!< Monitor/Print the name of this variable
        virtual void                                        printValue(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const;        //!< Monitor/Print the value this variable

        // Getters (only stochastic nodes with setter)
        virtual valueType&                                  getValue(void) = 0;                                                                         //!< Get the value (non-const version)
        virtual const valueType&                            getValue(void) const = 0;                                                                   //!< Get the value (const version)

        
    };
    
    
    template<>
    inline bool                                  TypedDagNode<int>::isSimpleNumeric(void) const { return true; } 
    
    template<>
    inline bool                                  TypedDagNode<double>::isSimpleNumeric(void) const { return true; }

    template<>
    inline bool                                  TypedDagNode<std::vector<int> >::isSimpleNumeric(void) const { return true; } 
    
    template<>
    inline bool                                  TypedDagNode<std::vector<double> >::isSimpleNumeric(void) const { return true; }

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
            ss << RbUtils::sub_vector<valueType>::getElement( getValue(), i );
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


#endif

