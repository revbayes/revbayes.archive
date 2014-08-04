#ifndef ModelVectorAbstractElement_H
#define ModelVectorAbstractElement_H

#include "ModelContainer.h"
#include "RbVector.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    /**
     * @brief ModelVectorAbstractElement: templated class for Rev vectors of abstract model objects
     *
     * We can rely on the template class specialization of ModelContainer for
     * vectors to provide most of the functionality we need.
     *
     * The class is based on a value type of RbVector, which is a vector of pointers
     * to objects. This means that it can be used for abstract internal value types,
     * but it is less efficient for non-abstract value types and should not be used
     * for those.
     */
    template <typename rlType>
    class ModelVectorAbstractElement : public ModelContainer< rlType, 1, RevBayesCore::RbVector<typename rlType::valueType> > {
        
    public:
        
        typedef typename rlType::valueType                                      elementType;
        typedef typename RevBayesCore::RbVector<typename rlType::valueType>     valueType;
        
                                                        ModelVectorAbstractElement(void);                                          //!< Default constructor
                                                        ModelVectorAbstractElement(const valueType& v);                            //!< Constructor from vector of values
                                                        ModelVectorAbstractElement(RevBayesCore::TypedDagNode<valueType>* n);      //!< Constructor from value node
        
        // Basic utility functions you have to override
        virtual ModelVectorAbstractElement<rlType>*     clone(void) const;                                                          //!< Clone object
        static const std::string&                       getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                     //!< Get class type spec
        virtual const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get the object type spec of the instance
        
        // Container functions
        RevObject*                                      makeIndirectReference(void);                                                //!< Make an object referencing the dag node of this object
        void                                            setElements(std::vector<RevObject*> elems, const std::vector<size_t> lengths);  //!< Set elements from Rev objects
        
    protected:
        
        // Container help function
        RevPtr<Variable>                                getElementFromValue(const std::vector<size_t>& oneOffsetIndex);                 //!< Get element from value
        
    };
    
}


#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RevPtr.h"
#include "TypeSpec.h"
#include "Variable.h"

#include <algorithm>

using namespace RevLanguage;


/**
 * Default constructor: construct an empty vector. We do not need to store the element type
 * because this can be accessed from our Rev type specification.
 */
template <typename rlType>
ModelVectorAbstractElement<rlType>::ModelVectorAbstractElement( void ) :
    ModelContainer< rlType, 1, RevBayesCore::RbVector<typename rlType::valueType> >()
{
}


/**
 * Constructor from a valueType object, that is, from an RbVector containing pointers
 * to the internal value type instances.
 */
template <typename rlType>
ModelVectorAbstractElement<rlType>::ModelVectorAbstractElement( const valueType &v ) :
    ModelContainer< rlType, 1, RevBayesCore::RbVector<typename rlType::valueType> >( v )
{
}


/**
 * Constructor from a DAG node appropriate for the model vector
 * object.
 */
template <typename rlType>
ModelVectorAbstractElement<rlType>::ModelVectorAbstractElement( RevBayesCore::TypedDagNode<valueType> *n ) :
    ModelContainer< rlType, 1, RevBayesCore::RbVector<typename rlType::valueType> >( rlType::getClassTypeSpec(), n )
{
}


/** Get a type-safe clone of the object */
template <typename rlType>
ModelVectorAbstractElement<rlType>* ModelVectorAbstractElement<rlType>::clone() const
{
    return new ModelVectorAbstractElement<rlType>( *this );
}


/**
 * Get Rev type of object. This is the Rev object element type followed by
 * a single set of square brackets. This provides a nice and convenient way
 * of specifying generic types of vectors for all Rev object types.
 */
template <typename rlType>
const std::string& ModelVectorAbstractElement<rlType>::getClassType(void)
{
    static std::string revType = rlType::getClassType() + "[]";
    
	return revType;
}


/**
 * Get class type spec describing type of object. Note that we have to
 * use the special version of the TypeSpec constructor because we derive
 * directly from ModelContainer and have the special "rlType[]" type
 * specification.
 */
template <typename rlType>
const RevLanguage::TypeSpec& ModelVectorAbstractElement<rlType>::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &ModelContainer<rlType, 1, valueType>::getClassTypeSpec(), &rlType::getClassTypeSpec() );
    
	return revTypeSpec;
}


/**
 * Get element(s) from value vector. Note that we use the language default of
 * one-offset indices, so remember to subtract one before accessing internal
 * objects.
 *
 * Note also that a zero or missing first index means that we want the entire vector.
 */
template <typename rlType>
RevPtr<Variable> ModelVectorAbstractElement<rlType>::getElementFromValue( const std::vector<size_t>& oneOffsetIndices )
{
    RevPtr<Variable> retVariable;
    
    if ( oneOffsetIndices.size() == 0 || oneOffsetIndices[0] == 0 )
    {
        ModelVectorAbstractElement<rlType>* newVector = this->clone();
        newVector->makeConstantValue();
        
        retVariable = new Variable( newVector );
    }
    else
        retVariable = new Variable( new rlType( *( this->getValue()[ oneOffsetIndices[0] - 1 ] ) ) );
    
    return retVariable;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. (Testing not to below) */
template <typename rlType>
const RevLanguage::TypeSpec& ModelVectorAbstractElement<rlType>::getTypeSpec(void) const
{
    //    static TypeSpec typeSpec = getClassTypeSpec();
    
    //    return typeSpec;
    
    return getClassTypeSpec();  // This should do the trick; there should be a separate version of the function for each template type
}


/**
 * Make indirect reference. This is relevant when we try to make an indirect reference
 * to the object in a dynamic evaluation context. These are statements of the type
 *
 *    a := b
 *
 * where this function is called if we are variable b.
 */
template <typename rlType>
RevObject* ModelVectorAbstractElement<rlType>::makeIndirectReference(void) {
    
    IndirectReferenceNode< ModelVectorAbstractElement<rlType> >* newNode =
    new IndirectReferenceNode< ModelVectorAbstractElement<rlType> >( "", this->getDagNode() );
    
    ModelVectorAbstractElement<rlType>* newObj = this->clone();
    
    newObj->setDagNode( newNode );
    
    return newObj;
}


/**
 * Set elements from a vector of Rev objects. We assume that we want
 * a composite container.
 */
template <typename rlType>
void ModelVectorAbstractElement<rlType>::setElements( std::vector<RevObject*> elems, const std::vector<size_t> lengths )
{
    setDagNode( new ContainerNode<rlType, valueType>( "", elems, lengths ) );
}


#endif
