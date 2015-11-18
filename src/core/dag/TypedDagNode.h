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
#include "NexusWriter.h"
#include "RbFileManager.h"
#include "RbUtil.h"
#include "StringUtilities.h"
#include "TraceNumeric.h"
#include "TraceTree.h"
#include "TraceVectorNumeric.h"

#include <ostream>
#include <string>

namespace RevBayesCore {
    
    class AbstractHomologousDiscreteCharacterData;
    
    template<class valueType>
    class TypedDagNode : public DagNode {
    
    public:
        TypedDagNode(const std::string &n);
        virtual                                            ~TypedDagNode(void);                                                                                         //!< Virtual destructor
    
        // pure virtual methods
        virtual TypedDagNode<valueType>*                    clone(void) const = 0;

        // member functions
        virtual Trace*                                      createTraceObject(void) const;                                                          //!< Create an empty trace object of the right trace type
        virtual size_t                                      getNumberOfElements(void) const;                                                                            //!< Get the number of elements for this value
        virtual std::string                                 getValueAsString(void) const;
        virtual bool                                        isSimpleNumeric(void) const;                                                                                //!< Is this variable a simple numeric variable? Currently only integer and real number are.
        virtual void                                        printName(std::ostream &o, const std::string &sep, int l=-1, bool left=true, bool fv=true) const;           //!< Monitor/Print this variable
        virtual void                                        printValue(std::ostream &o, int l=-1, bool left=true) const;                                                //!< Monitor/Print this variable
        virtual void                                        printValueElements(std::ostream &o, const std::string &sep, int l=-1, bool left=true, bool fl=true) const;  //!< Monitor/Print this variable
        virtual void                                        writeToFile(const std::string &dir) const;                                              //!< Write the value of this node to a file within the given directory.

        // getters and setters
        virtual valueType&                                  getValue(void) = 0;
        virtual const valueType&                            getValue(void) const = 0;
        virtual void                                        setValueFromString(const std::string &v) = 0;                                                               //!< Set value from string.

        
    };
    

    ///////////////////////
    // createTraceObject //
    ///////////////////////
    template<>
    inline Trace*                                TypedDagNode<int>::createTraceObject(void) const { return new TraceNumeric(); }

    template<>
    inline Trace*                                TypedDagNode<double>::createTraceObject(void) const { return new TraceNumeric(); }

    template<>
    inline Trace*                                TypedDagNode< RbVector<double> >::createTraceObject(void) const { return new TraceVectorNumeric(); }
    
    template<>
    inline Trace*                                TypedDagNode<Tree>::createTraceObject(void) const { return new TraceTree( getValue().isRooted() ); }

    
    /////////////////////
    // isSimpleNumeric //
    /////////////////////
    template<>
    inline bool                                  TypedDagNode<int>::isSimpleNumeric(void) const { return true; } 
    
    template<>
    inline bool                                  TypedDagNode<double>::isSimpleNumeric(void) const { return true; }

    template<>
    inline bool                                  TypedDagNode<RbVector<int> >::isSimpleNumeric(void) const { return true; }
    
    template<>
    inline bool                                  TypedDagNode<RbVector<double> >::isSimpleNumeric(void) const { return true; }

    
    ////////////////
    // printValue //
    ////////////////
    template<>
    inline void                                  TypedDagNode<unsigned int>::printValue(std::ostream &o, int l, bool left) const {
                                                        std::stringstream ss;
                                                        if ( this->getValue() == true ) ss << "TRUE"; else ss << "FALSE";
                                                        std::string s = ss.str();
                                                        if ( l > 0 ) StringUtilities::fillWithSpaces(s, l, left);
                                                        o << s;
                                                    }
    
    
    
    
    ////////////////////////
    // printValueElements //
    ////////////////////////
    template<>
    inline void TypedDagNode<double>::printValueElements(std::ostream &o, const std::string &sep, int l, bool left, bool flatten) const
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

    
    template<>
    inline void TypedDagNode<int>::printValueElements(std::ostream &o, const std::string &sep, int l, bool left, bool flatten) const
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
    
    
    template<>
    inline void TypedDagNode<unsigned int>::printValueElements(std::ostream &o, const std::string &sep, int l, bool left, bool flatten) const
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
    
    
    template<>
    inline void TypedDagNode<std::string>::printValueElements(std::ostream &o, const std::string &sep, int l, bool left, bool flatten) const
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

#include "RbContainer.h"
#include "RbFileManager.h"
#include "RbUtil.h"
#include "StringUtilities.h"

#include <vector>


template<class valueType>
RevBayesCore::TypedDagNode<valueType>::TypedDagNode(const std::string &n) : DagNode( n )
{
    
}


template<class valueType>
RevBayesCore::TypedDagNode<valueType>::~TypedDagNode( void )
{
}


template<class valueType>
RevBayesCore::Trace* RevBayesCore::TypedDagNode<valueType>::createTraceObject(void) const
{
    throw RbException("Cannot create a trace for variable '" + this->getName() + "' because there are not trace objects implemented for this value type.");
    
    return NULL;
}


template<class valueType>
size_t RevBayesCore::TypedDagNode<valueType>::getNumberOfElements( void ) const
{
    
    size_t numElements = RbUtils::sub_vector<valueType>::size( getValue() );
    
    return numElements;
}


template<class valueType>
std::string RevBayesCore::TypedDagNode<valueType>::getValueAsString( void ) const
{
    
    std::stringstream ss;
    ss << getValue();
    
    return ss.str();    
}



template<class valueType>
bool RevBayesCore::TypedDagNode<valueType>::isSimpleNumeric( void ) const
{
    return false;
}


template<class valueType>
void RevBayesCore::TypedDagNode<valueType>::printName(std::ostream &o, const std::string &sep, int l, bool left, bool flattenVector) const
{
    
    if ( RbUtils::is_vector<valueType>::value && flattenVector )
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
                StringUtilities::formatFixedWidth(n, l, left);
            }
            o << n;
        }
    } 
    else 
    {
        std::string n = getName();
        if ( l > 0 )
        {
            StringUtilities::formatFixedWidth(n, l, left);
        }
        o << n;
    }
}


template<class valueType>
void RevBayesCore::TypedDagNode<valueType>::printValue(std::ostream &o, int l, bool left) const
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


template<class valueType>
void RevBayesCore::TypedDagNode<valueType>::printValueElements(std::ostream &o, const std::string &sep, int l, bool left, bool flatten) const
{
    
    // check if this is a container
    const Container *c = dynamic_cast< const Container *>( &getValue() );
    if ( c == NULL || flatten == false )
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
    else
    {
        for (size_t i=0; i<c->size(); ++i)
        {
            c->printElement(o, i, sep, l, left);
            if ( i < (c->size()-1) )
            {
                o << sep;
            }
            
        }
    }
}


template<class valueType>
void RevBayesCore::TypedDagNode<valueType>::writeToFile(const std::string &dir) const
{

    // delegate to the type specific write function
    Serializer<valueType, IsDerivedFrom<valueType, Serializable>::Is >::writeToFile( this->getValue(), dir, this->getName() );

}

#endif

