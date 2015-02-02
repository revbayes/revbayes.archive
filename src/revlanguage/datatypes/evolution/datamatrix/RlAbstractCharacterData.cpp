#include "RlAbstractCharacterData.h"
#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "Natural.h"
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
    
    ArgumentRules* ncharArgRules               = new ArgumentRules();
    ArgumentRules* namesArgRules               = new ArgumentRules();
    ArgumentRules* ntaxaArgRules               = new ArgumentRules();
    ArgumentRules* excludeallArgRules          = new ArgumentRules();
    ArgumentRules* includeallArgRules          = new ArgumentRules();
    ArgumentRules* excludecharArgRules         = new ArgumentRules();
    ArgumentRules* excludecharArgRules2        = new ArgumentRules();
    ArgumentRules* includecharArgRules         = new ArgumentRules();
    ArgumentRules* includecharArgRules2        = new ArgumentRules();
    ArgumentRules* showdataArgRules            = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules          = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules2         = new ArgumentRules();
    ArgumentRules* setTaxonNameArgRules        = new ArgumentRules();
    
    
    excludecharArgRules->push_back(        new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    excludecharArgRules2->push_back(       new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    includecharArgRules->push_back(        new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    includecharArgRules2->push_back(       new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules->push_back(  new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules2->push_back( new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    removeTaxaArgRules->push_back(         new ArgumentRule(""           , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE) );
    removeTaxaArgRules2->push_back(        new ArgumentRule(""           , ModelVector<RlString>::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    setTaxonNameArgRules->push_back(       new ArgumentRule("current"    , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE) );
    setTaxonNameArgRules->push_back(       new ArgumentRule("new"        , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE) );
    
    
    methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
    methods.addFunction("nchar",               new MemberProcedure(Natural::getClassTypeSpec(),               ncharArgRules           ) );
    
    methods.addFunction("ntaxa",               new MemberProcedure(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules        ) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules2       ) );
    methods.addFunction("excludeAll",          new MemberProcedure(RlUtils::Void,        excludeallArgRules         ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules        ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules2       ) );
    methods.addFunction("includeAll",          new MemberProcedure(RlUtils::Void,        includeallArgRules         ) );
    methods.addFunction("show",                new MemberProcedure(RlUtils::Void,        showdataArgRules           ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("setTaxonName",        new MemberProcedure(RlUtils::Void,        setTaxonNameArgRules       ) );
    
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
    else if (name == "excludeCharacter")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        if ( argument.isType( Natural::getClassTypeSpec() ) ) 
        {
            size_t n = size_t( static_cast<const Natural&>( argument ).getValue() );
            // remember that we internally store the character indeces from 0 to n-1
            // but externally represent it as 1 to n
            charDataObject->excludeCharacter( n-1 );
        }
        else if ( argument.isType( ModelVector<Natural>::getClassTypeSpec() ) ) 
        {
            const ModelVector<Natural>& x = static_cast<const ModelVector<Natural>&>( argument );
            RevBayesCore::AbstractCharacterData &v = *charDataObject;
            for ( size_t i=0; i<x.size(); i++ )
            {
                // remember that we internally store the character indeces from 0 to n-1
                // but externally represent it as 1 to n
                v.excludeCharacter( size_t(x[i])-1 );
            }
        }
        return NULL;
    }
    else if (name == "excludeAll")
    {
        found = true;
        
        RevBayesCore::AbstractCharacterData &v = *charDataObject;
        size_t nChars = v.getNumberOfCharacters();
        
        for (size_t i = 0; i < nChars; i++)
        {
            v.excludeCharacter(i);
        }
        
        return NULL;
    }
    else if (name == "includeCharacter")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        if ( argument.isType( Natural::getClassTypeSpec() ) )
        {
            size_t n = size_t( static_cast<const Natural&>( argument ).getValue() );
            // remember that we internally store the character indeces from 0 to n-1
            // but externally represent it as 1 to n
            charDataObject->includeCharacter( n-1 );
        }
        else if ( argument.isType( ModelVector<Natural>::getClassTypeSpec() ) )
        {
            const ModelVector<Natural>& x = static_cast<const ModelVector<Natural>&>( argument );
            RevBayesCore::AbstractCharacterData &v = *charDataObject;
            for ( size_t i=0; i<x.size(); i++ )
            {
                // remember that we internally store the character indeces from 0 to n-1
                // but externally represent it as 1 to n
                v.includeCharacter( size_t(x[i])-1 );
            }
        }
        
        return NULL;
    }
    else if (name == "includeAll")
    {
        found = true;
        
        RevBayesCore::AbstractCharacterData &v = *charDataObject;
        size_t nChars = v.getNumberOfCharacters();
        
        for (size_t i = 0; i < nChars; i++)
        {
            v.includeCharacter(i);
        }
        
        return NULL;
        
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
    else if (name == "nchar") 
    {
        found = true;
        
        Natural *numChar = new Natural( charDataObject->getNumberOfIncludedCharacters() );
 
        return new RevVariable( numChar );
    }
    else if (name == "ntaxa") 
    {
        found = true;
        
        int n = (int)charDataObject->getNumberOfTaxa();
        
        return new RevVariable( new Natural(n) );
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

    //    else if (name == "nexcludedtaxa")
    //    {
    //        int n = (int)deletedTaxa.size();
    //        return RevPtr<RevObject>( new Natural(n) );
    //    }
    //    else if (name == "nexcludedchars")
    //    {
    //        int n = (int)deletedCharacters.size();
    //        return RevPtr<RevObject>( new Natural(n) );
    //    }
    //    else if (name == "nincludedtaxa")
    //    {
    //        int n = (int)(data.getNumberOfTaxa() - deletedTaxa.size());
    //        return RevPtr<RevObject>( new Natural(n) );
    //    }
    //    else if (name == "nincludedchars")
    //    {
    //        int n = (int)(data.getNumberOfCharacters() - deletedCharacters.size());
    //        return RevPtr<RevObject>( new Natural(n) );
    //    }
    //    else if (name == "excludedtaxa")
    //    {
    //        RlVector<std::string, RbString> *excludedTaxa = new RlVector<std::string, RbString>();
    //        for (std::set<size_t>::iterator it = data.deletedTaxa.begin(); it != deletedTaxa.end(); it++)
    //        {
    //            const std::string& tn = getTaxonNameWithIndex(*it);
    //            excludedTaxa->push_back( new RbString( tn ) );
    //        }
    //        return excludedTaxa;
    //    }
    //    else if (name == "excludedchars")
    //    {
    //        RlVector *excludedChars = new RlVector(Natural::getClassTypeSpec());
    //        for (std::set<size_t>::iterator it = deletedCharacters.begin(); it != deletedCharacters.end(); it++)
    //            excludedChars->push_back( new Natural(*it) );
    //        return excludedChars;
    //    }
    //    else if (name == "includedtaxa")
    //    {
    //        RlVector *includedTaxa = new RlVector(RbString::getClassTypeSpec());
    //        for (size_t i=0; i<getNumberOfTaxa(); i++)
    //        {
    //            if ( isTaxonExcluded(i) == false )
    //                includedTaxa->push_back( new RbString( getTaxonNameWithIndex(i) ) );
    //        }
    //        return includedTaxa;
    //    }
    //    else if (name == "includedchars")
    //    {
    //        RlVector *includedChars = new RlVector(Natural::getClassTypeSpec());
    //        for (size_t i=0; i<getNumberOfCharacters(); i++)
    //        {
    //            if ( isCharacterExcluded(i) == false )
    //                includedChars->push_back( new Natural(i+1) );
    //        }
    //        return includedChars;
    //    }
    //    else if (name == "nconstantpatterns")
    //    {
    //        int n = (int)numConstantPatterns();
    //        return RevPtr<RevObject>( new Natural(n) );
    //    }
    //    else if (name == "ncharswithambiguity")
    //    {
    //        int n = (int)numMissAmbig();
    //        return RevPtr<RevObject>( new Natural(n) );
    //    }
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


