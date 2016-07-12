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
    NonHomologousCharacterData( NULL ),
    dagNode( NULL )
{
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    
    
    methods.addFunction( new MemberProcedure( "chartype", RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction( new MemberProcedure( "isHomologous", RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
}


AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData( const RevBayesCore::AbstractNonHomologousDiscreteCharacterData &d) :
    NonHomologousCharacterData( NULL ),
    dagNode( new ConstantNode<valueType>("",d.clone()) )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    
    
    methods.addFunction( new MemberProcedure( "chartype", RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction( new MemberProcedure( "isHomologous", RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
}


AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData( RevBayesCore::AbstractNonHomologousDiscreteCharacterData *d) :
    NonHomologousCharacterData( NULL ),
    dagNode( new ConstantNode<valueType>("",d) )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    
    
    methods.addFunction( new MemberProcedure( "chartype", RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction( new MemberProcedure( "isHomologous", RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
}


AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractNonHomologousDiscreteCharacterData> *d) :
    NonHomologousCharacterData( NULL ),
    dagNode( d )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    // add the DAG node member methods
    // note that this is a sage case because all DAG nodes are member objects
    const MethodTable &dagMethods = dynamic_cast<RevMemberObject*>( dagNode )->getMethods();
    methods.insertInheritedMethods( dagMethods );
    
    ArgumentRules* chartypeArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    
    
    methods.addFunction( new MemberProcedure( "chartype", RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    methods.addFunction( new MemberProcedure( "isHomologous", RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
}


AbstractNonHomologousDiscreteCharacterData::AbstractNonHomologousDiscreteCharacterData(const AbstractNonHomologousDiscreteCharacterData &d) :
    NonHomologousCharacterData( d ),
    dagNode( NULL )
{
    
    if ( d.dagNode != NULL )
    {
        
        dagNode = d.dagNode->clone();
        
        // increment the reference count to the value
        dagNode->incrementReferenceCount();
    }
    
}


AbstractNonHomologousDiscreteCharacterData::~AbstractNonHomologousDiscreteCharacterData()
{
    
    // free the old value
    if ( dagNode != NULL )
    {
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
    }
}


AbstractNonHomologousDiscreteCharacterData& AbstractNonHomologousDiscreteCharacterData::operator=(const AbstractNonHomologousDiscreteCharacterData &v) {
    
    if ( this != &v )
    {
        // delegate to base class
        NonHomologousCharacterData::operator=( v );
        
        // free the old value
        if ( dagNode != NULL )
        {
            if ( dagNode->decrementReferenceCount() == 0 )
            {
                delete dagNode;
            }
            
            dagNode = NULL;
        }
        
        // create own copy
        if ( v.dagNode != NULL )
        {
            dagNode = v.dagNode->clone();
            
            // increment the reference count to the value
            dagNode->incrementReferenceCount();
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
    
    RevPtr<RevVariable> retVal = dynamic_cast<RevMemberObject *>( dagNode )->executeMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    
    if ( this->getDagNode() != NULL )
    {
        // set the internal value pointer
        setCharacterDataObject( &this->getDagNode()->getValue() );
    }
    
    retVal = executeCharacterDataMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    else if (name == "isHomologous")
    {
        found = true;
        
        bool ih = this->dagNode->getValue().isHomologyEstablished();
        
        return new RevVariable( new RlBoolean(ih) );
    }
    
    return NonHomologousCharacterData::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& AbstractNonHomologousDiscreteCharacterData::getClassType(void)
{
    
    static std::string revType = "AbstractNonHomologousDiscreteCharacterData";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractNonHomologousDiscreteCharacterData::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( NonHomologousCharacterData::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


RevBayesCore::TypedDagNode<RevBayesCore::AbstractNonHomologousDiscreteCharacterData>* AbstractNonHomologousDiscreteCharacterData::getDagNode( void ) const
{
    
    return dagNode;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& AbstractNonHomologousDiscreteCharacterData::getTypeSpec(void) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


const RevBayesCore::AbstractNonHomologousDiscreteCharacterData& AbstractNonHomologousDiscreteCharacterData::getValue( void ) const
{
    
    if ( dagNode == NULL )
    {
        throw RbException( "Invalid attempt to get value from an object with NULL DAG node" );
    }
    
    return dagNode->getValue();
}


RevBayesCore::AbstractNonHomologousDiscreteCharacterData& AbstractNonHomologousDiscreteCharacterData::getValue( void )
{
    
    if ( dagNode == NULL )
    {
        throw RbException( "Invalid attempt to get value from an object with NULL DAG node" );
    }
    
    return dagNode->getValue();
}


/**
 * Is the object or any of its upstream members or elements
 * modifiable by the user through assignment? We simply ask
 * our DAG node.
 */
bool AbstractNonHomologousDiscreteCharacterData::isAssignable( void ) const
{
    if ( dagNode == NULL )
    {
        return false;
    }
    
    return dagNode->isAssignable();
}


bool AbstractNonHomologousDiscreteCharacterData::isConstant( void ) const
{
    
    return dagNode->isConstant();
}


bool AbstractNonHomologousDiscreteCharacterData::isModelObject( void ) const
{
    
    return true;
}


void AbstractNonHomologousDiscreteCharacterData::makeConstantValue( void )
{
    
    if ( dagNode == NULL )
    {
        throw RbException("Cannot convert a variable without value to a constant value.");
    }
    else if ( dagNode->getDagNodeType() != RevBayesCore::DagNode::CONSTANT )
    {
        RevBayesCore::ConstantNode<valueType>* newNode = new ConstantNode<valueType>(dagNode->getName(), RevBayesCore::Cloner<valueType, IsDerivedFrom<valueType, RevBayesCore::Cloneable>::Is >::createClone( dagNode->getValue() ) );
        dagNode->replace(newNode);
        
        // delete the value if there are no other references to it.
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
        dagNode = newNode;
        
        // increment the reference counter
        dagNode->incrementReferenceCount();
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
    if ( dagNode != NULL )
    {
        dagNode->replace( detNode );
        if ( dagNode->decrementReferenceCount() == 0 )
            delete dagNode;
    }
    
    // Shift the actual node
    dagNode = detNode;
    
    // Increment the reference counter
    dagNode->incrementReferenceCount();
}


/**
 * Print value for user. The DAG node pointer may be NULL, in which
 * case we print "NA".
 */
void AbstractNonHomologousDiscreteCharacterData::printValue(std::ostream &o, bool user) const
{
    if ( dagNode == NULL )
    {
        o << "NA";
    }
    else
    {
        dagNode->printValue( o, "" );
    }
    
}


/** Copy name of variable onto DAG node, if it is not NULL */
void AbstractNonHomologousDiscreteCharacterData::setName(std::string const &n)
{
    if ( dagNode != NULL )
    {
        dagNode->setName( n );
    }
    
}


/**
 * Set dag node. We also accommodate the possibility of setting the DAG node to null.
 */
void AbstractNonHomologousDiscreteCharacterData::setDagNode(RevBayesCore::DagNode* newNode)
{
    
    // Take care of the old value node
    if ( dagNode != NULL )
    {
        if ( newNode != NULL )
        {
            newNode->setName( dagNode->getName() );
        }
        
        dagNode->replace(newNode);
        
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
    }
    
    // Set the new value node
    dagNode = static_cast< RevBayesCore::TypedDagNode<valueType>* >( newNode );
    
    // Increment the reference count to the new value node
    if ( dagNode != NULL )
    {
        dagNode->incrementReferenceCount();
    }
    
}


void AbstractNonHomologousDiscreteCharacterData::setValue(valueType *x)
{
    
    RevBayesCore::ConstantNode<valueType>* newNode;
    
    if ( dagNode == NULL )
    {
        newNode = new ConstantNode<valueType>("",x);
    }
    else
    {
        newNode = new ConstantNode<valueType>(dagNode->getName(),x);
        dagNode->replace(newNode);
        
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
    }
    
    dagNode = newNode;
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
}
