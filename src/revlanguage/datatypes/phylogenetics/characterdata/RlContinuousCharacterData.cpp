#include "RlContinuousCharacterData.h"

#include "ConstantNode.h"
#include "Natural.h"
#include "ModelVector.h"
#include "RlContinuousTaxonData.h"
#include "RlString.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
ContinuousCharacterData::ContinuousCharacterData(void) :
    HomologousCharacterData( ),
    dagNode( NULL )
{
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "[]", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "getTaxon", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );

    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );

}

/** Construct from core data type */
ContinuousCharacterData::ContinuousCharacterData(const RevBayesCore::ContinuousCharacterData &d) :
    HomologousCharacterData( ),
    dagNode( new ConstantNode<RevBayesCore::ContinuousCharacterData>("",d.clone()) )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "[]", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "getTaxon", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
}

/** Construct from core data type */
ContinuousCharacterData::ContinuousCharacterData(RevBayesCore::ContinuousCharacterData *d) :
    HomologousCharacterData( ),
    dagNode( new ConstantNode<RevBayesCore::ContinuousCharacterData>("",d) )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "[]", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "getTaxon", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );

    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
}


ContinuousCharacterData::ContinuousCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::ContinuousCharacterData> *d) :
    HomologousCharacterData( ),
    dagNode( d )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // add the DAG node member methods
    // note that this is a sage case because all DAG nodes are member objects
    const MethodTable &dagMethods = dynamic_cast<RevMemberObject*>( dagNode )->getMethods();
    methods.insertInheritedMethods( dagMethods );
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "[]", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "getTaxon", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );

    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
}


ContinuousCharacterData::ContinuousCharacterData(const ContinuousCharacterData &d) :
    HomologousCharacterData( d ),
    dagNode( NULL )
{
    
    if ( d.dagNode != NULL )
    {
        
        dagNode = d.dagNode->clone();
        
        // increment the reference count to the value
        dagNode->incrementReferenceCount();
    }
    
}




ContinuousCharacterData::~ContinuousCharacterData()
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


ContinuousCharacterData& ContinuousCharacterData::operator=(const ContinuousCharacterData &v) {
    
    if ( this != &v )
    {
        // delegate to base class
        HomologousCharacterData::operator=( v );
        
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


ContinuousCharacterData* ContinuousCharacterData::concatenate(const RevObject &d, std::string type) const
{
    const ContinuousCharacterData* tmp = dynamic_cast<const ContinuousCharacterData*>( &d );
    if ( tmp != NULL )
    {
        return concatenate( *tmp, type );
    }
    else
    {
        throw RbException("Cannot add an object of type '" + d.getType() + "' to a ContinuousCharacterData object.");
    }
}



ContinuousCharacterData* ContinuousCharacterData::concatenate(const ContinuousCharacterData &d, std::string type) const
{
    ContinuousCharacterData* cloneObj = clone();
    
    cloneObj->getDagNode()->getValue().concatenate( d.getValue(), type );
    // return the copy
    return cloneObj;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
ContinuousCharacterData* ContinuousCharacterData::clone(void) const
{
    
	return new ContinuousCharacterData(*this);
}


/* Map calls to member methods */
RevPtr<RevVariable> ContinuousCharacterData::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    RevPtr<RevVariable> retVal = dynamic_cast<RevMemberObject *>( dagNode )->executeMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    
    retVal = executeCharacterDataMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    else if (name == "[]" || name == "getTaxon")
    {
        found = true;
        
        if ( args[0].getVariable()->getRevObject().isType( Natural::getClassTypeSpec() ) )
        {
            // get the member with give index
            const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
        
            if (this->dagNode->getValue().getNumberOfTaxa() < (size_t)(index.getValue()) )
            {
                throw RbException("Index out of bounds in []");
            }
        
            const RevBayesCore::ContinuousTaxonData& element = static_cast< RevBayesCore::ContinuousCharacterData& >( this->dagNode->getValue() ).getTaxonData(size_t(index.getValue()) - 1);
        
            return new RevVariable( new ContinuousTaxonData( new RevBayesCore::ContinuousTaxonData( element ) ) );
        }
        else
        {
            // get the member with give index
            const std::string &index = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
            
            const RevBayesCore::ContinuousTaxonData& element = static_cast< RevBayesCore::ContinuousCharacterData& >( this->dagNode->getValue() ).getTaxonData(index);            
            return new RevVariable( new ContinuousTaxonData( new RevBayesCore::ContinuousTaxonData( element ) ) );
        }
    }
    
    
    return HomologousCharacterData::executeMethod( name, args, found );
    
}


/** Get Rev type of object */
const std::string& ContinuousCharacterData::getClassType(void)
{
    
    static std::string rev_type = "ContinuousCharacterData";
    
	return rev_type;
}


/** Get class type spec describing type of object */
const TypeSpec& ContinuousCharacterData::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( HomologousCharacterData::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


RevBayesCore::TypedDagNode<RevBayesCore::ContinuousCharacterData>* ContinuousCharacterData::getDagNode( void ) const
{
    
    return dagNode;
}



/** Get type spec */
const TypeSpec& ContinuousCharacterData::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    return type_spec;
}


const RevBayesCore::ContinuousCharacterData& ContinuousCharacterData::getValue( void ) const
{
    
    if ( dagNode == NULL )
    {
        throw RbException( "Invalid attempt to get value from an object with NULL DAG node" );
    }
    
    return dagNode->getValue();
}


RevBayesCore::ContinuousCharacterData& ContinuousCharacterData::getValue( void )
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
bool ContinuousCharacterData::isAssignable( void ) const
{
    if ( dagNode == NULL )
    {
        return false;
    }
    
    return dagNode->isAssignable();
}


bool ContinuousCharacterData::isConstant( void ) const
{
    
    return dagNode->isConstant();
}


bool ContinuousCharacterData::isModelObject( void ) const
{
    
    return true;
}


void ContinuousCharacterData::makeConstantValue( void )
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
ContinuousCharacterData* ContinuousCharacterData::makeIndirectReference(void)
{
    
    RevBayesCore::IndirectReferenceFunction< valueType > *func = new RevBayesCore::IndirectReferenceFunction<valueType>( this->getDagNode() );
    RevBayesCore::DeterministicNode< valueType >* newNode = new RevBayesCore::DeterministicNode< valueType >( "", func );
    
    ContinuousCharacterData* newObj = this->clone();
    
    const std::vector<RevBayesCore::Move*>& mvs = newObj->getDagNode()->getMoves();
    while ( mvs.empty() == false )
    {
        newObj->getDagNode()->removeMove( *mvs.begin() );
    }
    
    newObj->setDagNode( newNode );
    
    return newObj;
}


/** Convert a model object to a deterministic object, the value of which is determined by a user-defined Rev function */
void ContinuousCharacterData::makeUserFunctionValue( UserFunction* fxn )
{
    UserFunctionNode< ContinuousCharacterData >*  detNode = new UserFunctionNode< ContinuousCharacterData >( "", fxn );
    
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
void ContinuousCharacterData::printValue(std::ostream &o, bool user) const
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
void ContinuousCharacterData::setName(std::string const &n)
{
    if ( dagNode != NULL )
    {
        dagNode->setName( n );
    }
    
}


/**
 * Set dag node. We also accommodate the possibility of setting the DAG node to null.
 */
void ContinuousCharacterData::setDagNode(RevBayesCore::DagNode* newNode)
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


void ContinuousCharacterData::setValue(valueType *x)
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



