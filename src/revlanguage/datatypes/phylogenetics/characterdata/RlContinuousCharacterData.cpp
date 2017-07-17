#include "RlContinuousCharacterData.h"

#include "ConstantNode.h"
#include "RlMemberFunction.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RlContinuousTaxonData.h"
#include "RlString.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
ContinuousCharacterData::ContinuousCharacterData(void) : ModelObject<RevBayesCore::ContinuousCharacterData>(),
    HomologousCharacterData( )
{
    
    initMethods();

}

/** Construct from core data type */
ContinuousCharacterData::ContinuousCharacterData(const RevBayesCore::ContinuousCharacterData &d) : ModelObject<RevBayesCore::ContinuousCharacterData>( d.clone() ),
    HomologousCharacterData( )
{
    
    
    initMethods();
    
}

/** Construct from core data type */
ContinuousCharacterData::ContinuousCharacterData(RevBayesCore::ContinuousCharacterData *d) : ModelObject<RevBayesCore::ContinuousCharacterData>( d ),
    HomologousCharacterData( )
{
    
    
    initMethods();
}


ContinuousCharacterData::ContinuousCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::ContinuousCharacterData> *d) : ModelObject<RevBayesCore::ContinuousCharacterData>( d ),
    HomologousCharacterData( )
{
    
    initMethods();
    
}



ContinuousCharacterData::~ContinuousCharacterData()
{
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
    
    RevPtr<RevVariable> retVal = dynamic_cast<RevMemberObject *>( dag_node )->executeMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    
    retVal = executeCharacterDataMethod(name, args, found, &this->getValue());
    
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
        
            if (this->dag_node->getValue().getNumberOfTaxa() < (size_t)(index.getValue()) )
            {
                throw RbException("Index out of bounds in []");
            }
        
            const RevBayesCore::ContinuousTaxonData& element = static_cast< RevBayesCore::ContinuousCharacterData& >( this->dag_node->getValue() ).getTaxonData(size_t(index.getValue()) - 1);
        
            return new RevVariable( new ContinuousTaxonData( new RevBayesCore::ContinuousTaxonData( element ) ) );
        }
        else
        {
            // get the member with give index
            const std::string &index = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
            
            const RevBayesCore::ContinuousTaxonData& element = static_cast< RevBayesCore::ContinuousCharacterData& >( this->dag_node->getValue() ).getTaxonData(index);            
            return new RevVariable( new ContinuousTaxonData( new RevBayesCore::ContinuousTaxonData( element ) ) );
        }
    }
    
    
    return ModelObject<RevBayesCore::ContinuousCharacterData>::executeMethod( name, args, found );
    
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
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( ModelObject<RevBayesCore::ContinuousCharacterData>::getClassTypeSpec() ) );
    
	return rev_type_spec;
}



/** Get type spec */
const TypeSpec& ContinuousCharacterData::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    return type_spec;
}


void ContinuousCharacterData::initMethods( void )
{
    
    // add the DAG node member methods
    // note that this is a sage case because all DAG nodes are member objects
    if ( dag_node != NULL )
    {
        const MethodTable &dagMethods = dynamic_cast<RevMemberObject*>( dag_node )->getMethods();
        methods.insertInheritedMethods( dagMethods );
    }
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "[]", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    this->methods.addFunction( new MemberProcedure( "getTaxon", ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );
    
    // member functions
    ArgumentRules* get_element_arg_rules = new ArgumentRules();
    get_element_arg_rules->push_back( new ArgumentRule( "i", Natural::getClassTypeSpec(), "The index of the taxon.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    get_element_arg_rules->push_back( new ArgumentRule( "j", Natural::getClassTypeSpec(), "The index of the character.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<ContinuousCharacterData, Real>( "get", this, get_element_arg_rules   ) );

}



