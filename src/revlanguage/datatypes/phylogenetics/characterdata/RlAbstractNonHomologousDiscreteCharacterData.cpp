#include "RlAbstractNonHomologousDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlDiscreteTaxonData.h"
#include "RlSimplex.h"


using namespace RevLanguage;

AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData(void) :
    NonHomologousCharacterData( ),
<<<<<<< HEAD:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
    dagNode( NULL )
=======
    dag_node( NULL )
>>>>>>> development:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
{
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    
    
    methods.addFunction( new MemberProcedure( "chartype", RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction( new MemberProcedure( "isHomologous", RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
}


AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData( const RevBayesCore::AbstractNonHomologousDiscreteCharacterData &d) :
    NonHomologousCharacterData( ),
<<<<<<< HEAD:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
    dagNode( new ConstantNode<valueType>("",d.clone()) )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
=======
    dag_node( new ConstantNode<valueType>("",d.clone()) )
{
    
    // increment the reference count to the value
    dag_node->incrementReferenceCount();
>>>>>>> development:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    
    
    methods.addFunction( new MemberProcedure( "chartype", RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction( new MemberProcedure( "isHomologous", RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
}


AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData( RevBayesCore::AbstractNonHomologousDiscreteCharacterData *d) :
    NonHomologousCharacterData( ),
<<<<<<< HEAD:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
    dagNode( new ConstantNode<valueType>("",d) )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
=======
    dag_node( new ConstantNode<valueType>("",d) )
{
    
    // increment the reference count to the value
    dag_node->incrementReferenceCount();
>>>>>>> development:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    
    
    methods.addFunction( new MemberProcedure( "chartype", RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction( new MemberProcedure( "isHomologous", RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
}


AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractNonHomologousDiscreteCharacterData> *d) :
    NonHomologousCharacterData( ),
<<<<<<< HEAD:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
    dagNode( d )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
=======
    dag_node( d )
{
    
    // increment the reference count to the value
    dag_node->incrementReferenceCount();
>>>>>>> development:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    // add the DAG node member methods
    // note that this is a sage case because all DAG nodes are member objects
    const MethodTable &dagMethods = dynamic_cast<RevMemberObject*>( dag_node )->getMethods();
    methods.insertInheritedMethods( dagMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    
    
    methods.addFunction( new MemberProcedure( "chartype", RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction( new MemberProcedure( "isHomologous", RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
}


AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData(const AbstractNonHomologousDiscreteCharacterData &d) :
    NonHomologousCharacterData( d ),
    dag_node( NULL )
{
    
    if ( d.dag_node != NULL )
    {
        
        dag_node = d.dag_node->clone();
        
        // increment the reference count to the value
        dag_node->incrementReferenceCount();
    }
    
}


AbstractNonHomologousDiscreteCharacterData::~AbstractNonHomologousDiscreteCharacterData()
{
    
    // free the old value
    if ( dag_node != NULL )
    {
        if ( dag_node->decrementReferenceCount() == 0 )
        {
            delete dag_node;
        }
    }
}


AbstractNonHomologousDiscreteCharacterData& AbstractNonHomologousDiscreteCharacterData::operator=(const AbstractNonHomologousDiscreteCharacterData &v) {
    
    if ( this != &v )
    {
        // delegate to base class
        NonHomologousCharacterData::operator=( v );
        
        // free the old value
        if ( dag_node != NULL )
        {
            if ( dag_node->decrementReferenceCount() == 0 )
            {
                delete dag_node;
            }
            
            dag_node = NULL;
        }
        
        // create own copy
        if ( v.dag_node != NULL )
        {
            dag_node = v.dag_node->clone();
            
            // increment the reference count to the value
            dag_node->incrementReferenceCount();
        }
    }
    
    return *this;
}



AbstractNonHomologousDiscreteCharacterData* AbstractNonHomologousDiscreteCharacterData::clone() const
{
    return new AbstractNonHomologousDiscreteCharacterData( *this );
}


/* Map calls to member methods */
RevPtr<RevVariable> AbstractNonHomologousDiscreteCharacterData::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    RevPtr<RevVariable> retVal = dynamic_cast<RevMemberObject *>( dag_node )->executeMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    
    retVal = executeCharacterDataMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    else if (name == "isHomologous")
    {
        found = true;
        
        bool ih = this->dag_node->getValue().isHomologyEstablished();
        
        return new RevVariable( new RlBoolean(ih) );
    }
    
    return NonHomologousCharacterData::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& AbstractNonHomologousDiscreteCharacterData::getClassType(void)
{
    
    static std::string rev_type = "AbstractNonHomologousDiscreteCharacterData";
    
    return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractNonHomologousDiscreteCharacterData::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( NonHomologousCharacterData::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


RevBayesCore::TypedDagNode<RevBayesCore::AbstractNonHomologousDiscreteCharacterData>* AbstractNonHomologousDiscreteCharacterData::getDagNode( void ) const
{
    
    return dag_node;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& AbstractNonHomologousDiscreteCharacterData::getTypeSpec(void) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    return type_spec;
}


const RevBayesCore::AbstractNonHomologousDiscreteCharacterData& AbstractNonHomologousDiscreteCharacterData::getValue( void ) const
{
    
    if ( dag_node == NULL )
    {
        throw RbException( "Invalid attempt to get value from an object with NULL DAG node" );
    }
    
    return dag_node->getValue();
}


RevBayesCore::AbstractNonHomologousDiscreteCharacterData& AbstractNonHomologousDiscreteCharacterData::getValue( void )
{
    
    if ( dag_node == NULL )
    {
        throw RbException( "Invalid attempt to get value from an object with NULL DAG node" );
    }
    
    return dag_node->getValue();
}


/**
 * Is the object or any of its upstream members or elements
 * modifiable by the user through assignment? We simply ask
 * our DAG node.
 */
bool AbstractNonHomologousDiscreteCharacterData::isAssignable( void ) const
{
    if ( dag_node == NULL )
    {
        return false;
    }
    
    return dag_node->isAssignable();
}


bool AbstractNonHomologousDiscreteCharacterData::isConstant( void ) const
{
    
    return dag_node->isConstant();
}


bool AbstractNonHomologousDiscreteCharacterData::isModelObject( void ) const
{
    
    return true;
}


void AbstractNonHomologousDiscreteCharacterData::makeConstantValue( void )
{
    
    if ( dag_node == NULL )
    {
        throw RbException("Cannot convert a variable without value to a constant value.");
    }
    else if ( dag_node->getDagNodeType() != RevBayesCore::DagNode::CONSTANT )
    {
        RevBayesCore::ConstantNode<valueType>* newNode = new ConstantNode<valueType>(dag_node->getName(), RevBayesCore::Cloner<valueType, IsDerivedFrom<valueType, RevBayesCore::Cloneable>::Is >::createClone( dag_node->getValue() ) );
        dag_node->replace(newNode);
        
        // delete the value if there are no other references to it.
        if ( dag_node->decrementReferenceCount() == 0 )
        {
            delete dag_node;
        }
        
        dag_node = newNode;
        
        // increment the reference counter
        dag_node->incrementReferenceCount();
    }
    
}


/**
 * Make an indirect reference to the variable. This is appropriate for the contexts
 * where the object occurs on the righ-hand side of expressions like a := b
 */
AbstractNonHomologousDiscreteCharacterData* AbstractNonHomologousDiscreteCharacterData::makeIndirectReference(void)
{
    
    RevBayesCore::IndirectReferenceFunction< valueType > *func = new RevBayesCore::IndirectReferenceFunction<valueType>( this->getDagNode() );
    RevBayesCore::DeterministicNode< valueType >* newNode = new RevBayesCore::DeterministicNode< valueType >( "", func );
    
    AbstractNonHomologousDiscreteCharacterData* newObj = this->clone();
    
    const std::vector<RevBayesCore::Move*>& mvs = newObj->getDagNode()->getMoves();
    while ( mvs.empty() == false )
    {
        newObj->getDagNode()->removeMove( *mvs.begin() );
    }
    
    newObj->setDagNode( newNode );
    
    return newObj;
}


/** Convert a model object to a deterministic object, the value of which is determined by a user-defined Rev function */
void AbstractNonHomologousDiscreteCharacterData::makeUserFunctionValue( UserFunction* fxn )
{
    UserFunctionNode< AbstractNonHomologousDiscreteCharacterData >*  detNode = new UserFunctionNode< AbstractNonHomologousDiscreteCharacterData >( "", fxn );
    
    // Signal replacement and delete the value if there are no other references to it.
    if ( dag_node != NULL )
    {
        dag_node->replace( detNode );
        if ( dag_node->decrementReferenceCount() == 0 )
            delete dag_node;
    }
    
    // Shift the actual node
    dag_node = detNode;
    
    // Increment the reference counter
    dag_node->incrementReferenceCount();
}


/**
 * Print value for user. The DAG node pointer may be NULL, in which
 * case we print "NA".
 */
void AbstractNonHomologousDiscreteCharacterData::printValue(std::ostream &o, bool user) const
{
    if ( dag_node == NULL )
    {
        o << "NA";
    }
    else
    {
<<<<<<< HEAD:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
        dagNode->printValue( o, "" );
=======
        dag_node->printValue( o, "" );
>>>>>>> development:src/revlanguage/datatypes/phylogenetics/characterdata/RlAbstractNonHomologousDiscreteCharacterData.cpp
    }
    
}


/** Copy name of variable onto DAG node, if it is not NULL */
void AbstractNonHomologousDiscreteCharacterData::setName(std::string const &n)
{
    if ( dag_node != NULL )
    {
        dag_node->setName( n );
    }
    
}


/**
 * Set dag node. We also accommodate the possibility of setting the DAG node to null.
 */
void AbstractNonHomologousDiscreteCharacterData::setDagNode(RevBayesCore::DagNode* newNode)
{
    
    // Take care of the old value node
    if ( dag_node != NULL )
    {
        if ( newNode != NULL )
        {
            newNode->setName( dag_node->getName() );
        }
        
        dag_node->replace(newNode);
        
        if ( dag_node->decrementReferenceCount() == 0 )
        {
            delete dag_node;
        }
        
    }
    
    // Set the new value node
    dag_node = static_cast< RevBayesCore::TypedDagNode<valueType>* >( newNode );
    
    // Increment the reference count to the new value node
    if ( dag_node != NULL )
    {
        dag_node->incrementReferenceCount();
    }
    
}


void AbstractNonHomologousDiscreteCharacterData::setValue(valueType *x)
{
    
    RevBayesCore::ConstantNode<valueType>* newNode;
    
    if ( dag_node == NULL )
    {
        newNode = new ConstantNode<valueType>("",x);
    }
    else
    {
        newNode = new ConstantNode<valueType>(dag_node->getName(),x);
        dag_node->replace(newNode);
        
        if ( dag_node->decrementReferenceCount() == 0 )
        {
            delete dag_node;
        }
        
    }
    
    dag_node = newNode;
    
    // increment the reference count to the value
    dag_node->incrementReferenceCount();
    
}
