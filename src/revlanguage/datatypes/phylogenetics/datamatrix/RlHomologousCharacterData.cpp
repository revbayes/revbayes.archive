#include "RlHomologousCharacterData.h"
#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"


using namespace RevLanguage;

HomologousCharacterData::HomologousCharacterData( RevBayesCore::HomologousCharacterData *o ) : AbstractCharacterData( o ),
    charDataObject( o )
{
    
}


MethodTable HomologousCharacterData::getCharacterDataMethods( void ) const
{
    
    MethodTable methods = AbstractCharacterData::getCharacterDataMethods();
    
    ArgumentRules* ncharArgRules                = new ArgumentRules();
    ArgumentRules* excludeallArgRules           = new ArgumentRules();
    ArgumentRules* includeallArgRules           = new ArgumentRules();
    ArgumentRules* excludecharArgRules          = new ArgumentRules();
    ArgumentRules* excludecharArgRules2         = new ArgumentRules();
    ArgumentRules* includecharArgRules          = new ArgumentRules();
    ArgumentRules* includecharArgRules2         = new ArgumentRules();
    ArgumentRules* isResolvedArgRules           = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules    = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2   = new ArgumentRules();
    
    
    excludecharArgRules->push_back(         new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    excludecharArgRules2->push_back(        new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    includecharArgRules->push_back(         new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    includecharArgRules2->push_back(        new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    isResolvedArgRules->push_back(          new ArgumentRule("txIdx"      , Natural::getClassTypeSpec()                 , ArgumentRule::BY_VALUE) );
    isResolvedArgRules->push_back(          new ArgumentRule("chIdx"      , Natural::getClassTypeSpec()                 , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules->push_back(   new ArgumentRule(""           , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules2->push_back(  new ArgumentRule(""           , ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    
    
    methods.addFunction("nchar",               new MemberProcedure(Natural::getClassTypeSpec(),               ncharArgRules           ) );
    methods.addFunction("excludeCharacter",     new MemberProcedure(RlUtils::Void,        excludecharArgRules        ) );
    methods.addFunction("excludeCharacter",     new MemberProcedure(RlUtils::Void,        excludecharArgRules2       ) );
    methods.addFunction("excludeAll",           new MemberProcedure(RlUtils::Void,        excludeallArgRules         ) );
    methods.addFunction("includeCharacter",     new MemberProcedure(RlUtils::Void,        includecharArgRules        ) );
    methods.addFunction("includeCharacter",     new MemberProcedure(RlUtils::Void,        includecharArgRules2       ) );
    methods.addFunction("includeAll",           new MemberProcedure(RlUtils::Void,        includeallArgRules         ) );
    methods.addFunction("isResolved",           new MemberProcedure(RlBoolean::getClassTypeSpec(), isResolvedArgRules           ) );
    
    
    return methods;
}

/* Map calls to member methods */
RevPtr<RevVariable> HomologousCharacterData::executeCharacterDataMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    RevPtr<RevVariable> retVal = AbstractCharacterData::executeCharacterDataMethod(name, args, found);
    
    if (name == "excludeCharacter")
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
            RevBayesCore::HomologousCharacterData &v = *charDataObject;
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
        
        RevBayesCore::HomologousCharacterData &v = *charDataObject;
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
            RevBayesCore::HomologousCharacterData &v = *charDataObject;
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
        
        RevBayesCore::HomologousCharacterData &v = *charDataObject;
        size_t nChars = v.getNumberOfCharacters();
        
        for (size_t i = 0; i < nChars; i++)
        {
            v.includeCharacter(i);
        }
        
        return NULL;
        
    }
    else if (name == "isResolved")
    {
        found = true;
        
        const RevObject& argument1 = args[0].getVariable()->getRevObject();
        const RevObject& argument2 = args[1].getVariable()->getRevObject();
        size_t tIdx = size_t( static_cast<const Natural&>( argument1 ).getValue() );
        size_t cIdx = size_t( static_cast<const Natural&>( argument2 ).getValue() );
        // remember that we internally store the character indeces from 0 to n-1
        // but externally represent it as 1 to n
        bool tf = charDataObject->isCharacterResolved( tIdx-1, cIdx-1 );
        return new RevVariable( new RlBoolean(tf) );
    }
    else if (name == "nchar")
    {
        found = true;
        
        Natural *numChar = new Natural( charDataObject->getNumberOfIncludedCharacters() );
        
        return new RevVariable( numChar );
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
    
    
    // not found a matching method
//    found = false;
    
    return retVal;
}



void HomologousCharacterData::setCharacterDataObject(RevBayesCore::AbstractCharacterData *o)
{
    AbstractCharacterData::setCharacterDataObject(o);
}


void HomologousCharacterData::setCharacterDataObject(RevBayesCore::HomologousCharacterData *o)
{
    AbstractCharacterData::setCharacterDataObject(o);
    
    charDataObject = o;
}


