#include "RlAbstractCharacterData.h"
#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Probability.h"
#include "RlBoolean.h"
#include "RlString.h"


using namespace RevLanguage;

AbstractCharacterData::AbstractCharacterData( RevBayesCore::AbstractCharacterData *o ) :
    charDataObject( o )
{

}


MethodTable AbstractCharacterData::getCharacterDataMethods( void ) const
{
    
    MethodTable methods = MethodTable();
    
    ArgumentRules* namesArgRules                = new ArgumentRules();
    ArgumentRules* ntaxaArgRules                = new ArgumentRules();
    ArgumentRules* excludeTaxaArgRules          = new ArgumentRules();
    ArgumentRules* excludeTaxaArgRules2         = new ArgumentRules();
    ArgumentRules* includeTaxaArgRules          = new ArgumentRules();
    ArgumentRules* includeTaxaArgRules2         = new ArgumentRules();
    ArgumentRules* isSequenceMissingArgRules    = new ArgumentRules();
    ArgumentRules* percentageMissingArgRules    = new ArgumentRules();
    ArgumentRules* showdataArgRules             = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules           = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules2          = new ArgumentRules();
    ArgumentRules* setTaxonNameArgRules         = new ArgumentRules();
    
    excludeTaxaArgRules->push_back(         new ArgumentRule("index" , RlString::getClassTypeSpec(), "The index of character.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    excludeTaxaArgRules2->push_back(        new ArgumentRule("indices" , ModelVector<RlString>::getClassTypeSpec(), "The vector of indices of the characters.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    includeTaxaArgRules->push_back(         new ArgumentRule("name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    includeTaxaArgRules2->push_back(        new ArgumentRule("names" , ModelVector<RlString>::getClassTypeSpec(), "The names of the taxa.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    isSequenceMissingArgRules->push_back(   new ArgumentRule("name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    percentageMissingArgRules->push_back(   new ArgumentRule("name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    removeTaxaArgRules->push_back(          new ArgumentRule("name" , RlString::getClassTypeSpec(), "The name of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    removeTaxaArgRules2->push_back(         new ArgumentRule("names" , ModelVector<RlString>::getClassTypeSpec(), "The names of the taxa.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    setTaxonNameArgRules->push_back(        new ArgumentRule("current"    , RlString::getClassTypeSpec(), "The old name.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    setTaxonNameArgRules->push_back(        new ArgumentRule("new"        , RlString::getClassTypeSpec(), "The new name.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    

    methods.addFunction( new MemberProcedure( "excludeTaxa",  RlUtils::Void, excludeTaxaArgRules ) );
    methods.addFunction( new MemberProcedure( "excludeTaxa", RlUtils::Void, excludeTaxaArgRules2 ) );
    methods.addFunction( new MemberProcedure( "includeTaxa", RlUtils::Void, includeTaxaArgRules ) );
    methods.addFunction( new MemberProcedure( "includeTaxa", RlUtils::Void, includeTaxaArgRules2 ) );
    methods.addFunction( new MemberProcedure( "isSequenceMissing", RlBoolean::getClassTypeSpec(), isSequenceMissingArgRules ) );
    methods.addFunction( new MemberProcedure( "names", ModelVector<RlString>::getClassTypeSpec(), namesArgRules ) );
    methods.addFunction( new MemberProcedure( "ntaxa", Natural::getClassTypeSpec(), ntaxaArgRules ) );
    methods.addFunction( new MemberProcedure( "percentageMissing", Probability::getClassTypeSpec(), percentageMissingArgRules ) );
    methods.addFunction( new MemberProcedure( "show", RlUtils::Void, showdataArgRules ) );
    methods.addFunction( new MemberProcedure( "removeTaxa", RlUtils::Void, removeTaxaArgRules ) );
    methods.addFunction( new MemberProcedure( "removeTaxa", RlUtils::Void, removeTaxaArgRules2 ) );
    methods.addFunction( new MemberProcedure( "setTaxonName", RlUtils::Void, setTaxonNameArgRules ) );
    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "size", Natural::getClassTypeSpec(), sizeArgRules) );
    
    return methods;
}

/* Map calls to member methods */
RevPtr<RevVariable> AbstractCharacterData::executeCharacterDataMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "chartype")
    {
        found = true;
        
        return new RevVariable( new RlString( charDataObject->getDataType() ) );
    }
    else if (name == "excludeTaxa")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        if ( argument.isType( RlString::getClassTypeSpec() ) )
        {
            const std::string &n = static_cast<const RlString&>( argument ).getValue();
            // remember that we internally store the character indeces from 0 to n-1
            // but externally represent it as 1 to n
            charDataObject->excludeTaxon( n );
        }
        else if ( argument.isType( ModelVector<RlString>::getClassTypeSpec() ) )
        {
            const ModelVector<RlString>& x = static_cast<const ModelVector<RlString>&>( argument );
            RevBayesCore::AbstractCharacterData &v = *charDataObject;
            for ( size_t i=0; i<x.size(); i++ )
            {
                v.excludeTaxon( x[i] );
            }
        }
        return NULL;
    }
    else if (name == "includeTaxa")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        if ( argument.isType( RlString::getClassTypeSpec() ) )
        {
            const std::string &n = static_cast<const RlString&>( argument ).getValue();
            // remember that we internally store the character indeces from 0 to n-1
            // but externally represent it as 1 to n
            charDataObject->includeTaxon( n );
        }
        else if ( argument.isType( ModelVector<RlString>::getClassTypeSpec() ) )
        {
            const ModelVector<RlString>& x = static_cast<const ModelVector<RlString>&>( argument );
            RevBayesCore::AbstractCharacterData &v = *charDataObject;
            for ( size_t i=0; i<x.size(); i++ )
            {
                // remember that we internally store the character indeces from 0 to n-1
                // but externally represent it as 1 to n
                v.includeTaxon( x[i] );
            }
        }
        
        return NULL;
    }
    else if (name == "isSequenceMissing")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        const std::string &n = static_cast<const RlString&>( argument ).getValue();
        
        bool tf = charDataObject->isSequenceMissing( n );

        return new RevVariable( new RlBoolean(tf) );
    }
    else if (name == "names")
    {
        found = true;
        
        ModelVector<RlString> *n = new ModelVector<RlString>();
        for (size_t i = 0; i < charDataObject->getNumberOfTaxa(); ++i)
        {
            n->push_back( charDataObject->getTaxonNameWithIndex( i ) );
        }
        
        return new RevVariable( n );
    }
    else if (name == "ntaxa") 
    {
        found = true;
        
        int n = (int)charDataObject->getNumberOfTaxa();
        
        return new RevVariable( new Natural(n) );
    }
    else if (name == "percentageMissing")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        const std::string &n = static_cast<const RlString&>( argument ).getValue();
        
        double p = charDataObject->getPercentageMissing( n );
        
        return new RevVariable( new Probability(p) );
    }
    else if (name == "size") 
    {
        found = true;
        
        int n = (int)charDataObject->getNumberOfTaxa();
        
        return new RevVariable( new Natural(n) );
    }
    else if (name == "removeTaxa" )
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        if ( argument.isType( RlString::getClassTypeSpec() ) )
        {
            std::string n = std::string( static_cast<const RlString&>( argument ).getValue() );
            charDataObject->excludeTaxon( n );
        }
        else if ( argument.isType( ModelVector<RlString>::getClassTypeSpec() ) )
        {
            const ModelVector<RlString>& x = static_cast<const ModelVector<RlString>&>( argument );
            RevBayesCore::AbstractCharacterData &v = *charDataObject;
            for ( size_t i=0; i<x.size(); i++ )
            {
                std::string n = std::string( static_cast<const RlString&>( x[i] ).getValue() );
                v.excludeTaxon( n );
            }
        }
        return NULL;
    }
    else if (name == "setTaxonName")
    {
        found = true;
        
        const RevObject& current = args[0].getVariable()->getRevObject();
        if ( current.isType( RlString::getClassTypeSpec() ) )
        {
            std::string n = std::string( static_cast<const RlString&>( current ).getValue() );
            const RevObject& newName = args[1].getVariable()->getRevObject();
            if ( newName.isType( RlString::getClassTypeSpec() ) )
            {
                std::string name = std::string( static_cast<const RlString&>( newName ).getValue() );
                charDataObject->setTaxonName( n ,name );
               // std::cout << "new name: "<< dagNode->getValue().getTaxonData( n ).getTaxonName() << std::endl;
            }
        }
        return NULL;
    }
    else if (name == "show")
    {
        found = true;
        
        charDataObject->show(std::cout);
        
        return NULL;
    }
    
    
    // not found a matching method
    found = false;
    return NULL;
}




/* Get Rev type of object */
const std::string& AbstractCharacterData::getClassType(void)
{
    
    static std::string revType = "AbstractCharacterData";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractCharacterData::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



void AbstractCharacterData::setCharacterDataObject(RevBayesCore::AbstractCharacterData *o)
{
    
    charDataObject = o;
}


