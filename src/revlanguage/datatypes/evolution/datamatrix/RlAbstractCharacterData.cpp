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
    
    ArgumentRules* namesArgRules               = new ArgumentRules();
    ArgumentRules* ntaxaArgRules               = new ArgumentRules();
    ArgumentRules* showdataArgRules            = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules          = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules2         = new ArgumentRules();
    ArgumentRules* setTaxonNameArgRules        = new ArgumentRules();
    
    
    removeTaxaArgRules->push_back(         new ArgumentRule(""           , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE) );
    removeTaxaArgRules2->push_back(        new ArgumentRule(""           , ModelVector<RlString>::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    setTaxonNameArgRules->push_back(       new ArgumentRule("current"    , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE) );
    setTaxonNameArgRules->push_back(       new ArgumentRule("new"        , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE) );
    
    
    methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
    
    methods.addFunction("ntaxa",               new MemberProcedure(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
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


