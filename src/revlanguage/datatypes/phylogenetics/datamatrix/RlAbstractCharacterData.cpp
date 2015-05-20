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
    
    excludeTaxaArgRules->push_back(         new ArgumentRule(""           , RlString::getClassTypeSpec()                , ArgumentRule::BY_VALUE) );
    excludeTaxaArgRules2->push_back(        new ArgumentRule(""           , ModelVector<RlString>::getClassTypeSpec()   , ArgumentRule::BY_VALUE) );
    includeTaxaArgRules->push_back(         new ArgumentRule(""           , RlString::getClassTypeSpec()                , ArgumentRule::BY_VALUE) );
    includeTaxaArgRules2->push_back(        new ArgumentRule(""           , ModelVector<RlString>::getClassTypeSpec()   , ArgumentRule::BY_VALUE) );
    isSequenceMissingArgRules->push_back(   new ArgumentRule(""           , RlString::getClassTypeSpec()                , ArgumentRule::BY_VALUE) );
    percentageMissingArgRules->push_back(   new ArgumentRule(""           , RlString::getClassTypeSpec()                , ArgumentRule::BY_VALUE) );
    removeTaxaArgRules->push_back(          new ArgumentRule(""           , RlString::getClassTypeSpec()                , ArgumentRule::BY_VALUE) );
    removeTaxaArgRules2->push_back(         new ArgumentRule(""           , ModelVector<RlString>::getClassTypeSpec()   , ArgumentRule::BY_VALUE) );
    setTaxonNameArgRules->push_back(        new ArgumentRule("current"    , RlString::getClassTypeSpec()                , ArgumentRule::BY_VALUE) );
    setTaxonNameArgRules->push_back(        new ArgumentRule("new"        , RlString::getClassTypeSpec()                , ArgumentRule::BY_VALUE) );
    

    methods.addFunction("excludeTaxa",          new MemberProcedure(RlUtils::Void,                 excludeTaxaArgRules          ) );
    methods.addFunction("excludeTaxa",          new MemberProcedure(RlUtils::Void,                 excludeTaxaArgRules2         ) );
    methods.addFunction("includeTaxa",          new MemberProcedure(RlUtils::Void,                 includeTaxaArgRules          ) );
    methods.addFunction("includeTaxa",          new MemberProcedure(RlUtils::Void,                 includeTaxaArgRules2         ) );
    methods.addFunction("isSequenceMissing",    new MemberProcedure(RlBoolean::getClassTypeSpec(), isSequenceMissingArgRules    ) );
    methods.addFunction("names",                new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
    methods.addFunction("ntaxa",                new MemberProcedure(Natural::getClassTypeSpec(),   ntaxaArgRules                ) );
    methods.addFunction("percentageMissing",    new MemberProcedure(Probability::getClassTypeSpec(),            percentageMissingArgRules   ) );
    methods.addFunction("show",                 new MemberProcedure(RlUtils::Void,                 showdataArgRules             ) );
    methods.addFunction("removeTaxa",           new MemberProcedure(RlUtils::Void,                 removeTaxaArgRules           ) );
    methods.addFunction("removeTaxa",           new MemberProcedure(RlUtils::Void,                 removeTaxaArgRules2          ) );
    methods.addFunction("setTaxonName",         new MemberProcedure(RlUtils::Void,                 setTaxonNameArgRules         ) );
    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );
    
    return methods;
}

/* Map calls to member methods */
RevPtr<RevVariable> AbstractCharacterData::executeCharacterDataMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    if (name == "chartype")
    {
        found = true;
        
        return new RevVariable( new RlString( charDataObject->getDatatype() ) );
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



void AbstractCharacterData::setCharacterDataObject(RevBayesCore::AbstractCharacterData *o)
{
    
    charDataObject = o;
}


