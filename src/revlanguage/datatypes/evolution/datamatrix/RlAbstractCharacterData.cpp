#include "RlAbstractCharacterData.h"
#include "ArgumentRule.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTaxonData.h"
#include "Vector.h"


using namespace RevLanguage;

AbstractCharacterData::AbstractCharacterData(void) : ModelObject<RevBayesCore::AbstractCharacterData>(), methods() {
    
    initMethods();
    
}


AbstractCharacterData::AbstractCharacterData( RevBayesCore::AbstractCharacterData *v) : ModelObject<RevBayesCore::AbstractCharacterData>( v ), methods() {
    
    initMethods();
    
}


AbstractCharacterData::AbstractCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData> *d) : ModelObject<RevBayesCore::AbstractCharacterData>( d ), methods() {
    
    initMethods();
    
}



AbstractCharacterData* AbstractCharacterData::clone() const {
    
    return new AbstractCharacterData( *this );
}


/* Map calls to member methods */
RevObject* AbstractCharacterData::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    
    if (name == "chartype") 
    {
        
        return new RlString( this->dagNode->getValue().getDatatype() );
    }
    else if (name == "excludeCharacter")
    {
        const RevObject& argument = args[0].getVariable()->getRevObject();
        if ( argument.isTypeSpec( Natural::getClassTypeSpec() ) ) 
        {
            size_t n = size_t( static_cast<const Natural&>( argument ).getValue() );
            // remember that we internally store the character indeces from 0 to n-1
            // but externally represent it as 1 to n
            dagNode->getValue().excludeCharacter( n-1 );
        }
        else if ( argument.isTypeSpec( Vector<Natural>::getClassTypeSpec() ) ) 
        {
            const Vector<Natural>& x = static_cast<const Vector<Natural>&>( argument );
            RevBayesCore::AbstractCharacterData &v = dagNode->getValue();
            for ( size_t i=0; i<x.size(); i++ )
            {
                // remember that we internally store the character indeces from 0 to n-1
                // but externally represent it as 1 to n
                v.excludeCharacter( size_t(x[i])-1 );
            }
        }
        return NULL;
    }
    else if (name == "names") 
    {
        Vector<RlString> *n = new Vector<RlString>();
        for (size_t i = 0; i < this->dagNode->getValue().getNumberOfTaxa(); ++i)
        {
            n->push_back( this->dagNode->getValue().getTaxonNameWithIndex( i ) );
        }
        
        return n;
    }
    else if (name == "nchar") 
    {
        
        Vector<Natural> *numChar = new Vector<Natural>();
        for (size_t i=0; i<this->dagNode->getValue().getNumberOfTaxa(); i++)
        {
            
            if ( this->dagNode->getValue().isTaxonExcluded(i) == false )
            {
                
                if (this->dagNode->getValue().isHomologyEstablished() == true)
                    numChar->push_back( Natural( this->dagNode->getValue().getNumberOfIncludedCharacters() ) );
                else
                    numChar->push_back( Natural( this->dagNode->getValue().getNumberOfIncludedCharacters(i) ) );
                
            }
            
        }
        return numChar;
    }
    else if (name == "ntaxa") 
    {
        int n = (int)this->dagNode->getValue().getNumberOfTaxa();
        
        return new Natural(n);
    }
    else if (name == "size") 
    {
        int n = (int)this->dagNode->getValue().getNumberOfTaxa();
        
        return new Natural(n);
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
        
        size_t nt = this->dagNode->getValue().getNumberOfTaxa();
        for (size_t i=0; i<nt; i++)
        {
            
            const RevBayesCore::AbstractTaxonData& taxonData = this->dagNode->getValue().getTaxonData(i);
            std::string taxonName = this->dagNode->getValue().getTaxonNameWithIndex(i);
            size_t nc = taxonData.getNumberOfCharacters();
            std::cout << "   " << taxonName << std::endl;
            std::cout << "   ";
            for (size_t j=0; j<nc; j++)
            {
                
                const RevBayesCore::CharacterState& o = taxonData[j];
                std::string s = o.getStringValue();
                
                std::cout << s;
                if ( (j+1) % 100 == 0 && (j+1) != nc )
                    std::cout << std::endl << "   ";
                
            }
            
            std::cout << std::endl;
        }
        
        return NULL;
    }
    else if (name == "ishomologous")
    {
        bool ih = this->dagNode->getValue().isHomologyEstablished();
    
        return new RlBoolean(ih);
    } 
    
    return ModelObject<RevBayesCore::AbstractCharacterData>::executeMethod( name, args );
}


/* Get class name of object */
const std::string& AbstractCharacterData::getClassName(void) { 
    
    static std::string rbClassName = "AbstractCharacterData";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractCharacterData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get method specifications */
const MethodTable& AbstractCharacterData::getMethods(void) const {
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& AbstractCharacterData::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/* Get method specifications */
void AbstractCharacterData::initMethods(void) {
    
    ArgumentRules* ncharArgRules               = new ArgumentRules();
    ArgumentRules* namesArgRules               = new ArgumentRules();
    ArgumentRules* ntaxaArgRules               = new ArgumentRules();
    ArgumentRules* chartypeArgRules            = new ArgumentRules();    
//    ArgumentRules* nexcludedtaxaArgRules       = new ArgumentRules();    
//    ArgumentRules* nexcludedcharsArgRules      = new ArgumentRules();    
//    ArgumentRules* nincludedtaxaArgRules       = new ArgumentRules();    
//    ArgumentRules* nincludedcharsArgRules      = new ArgumentRules();    
//    ArgumentRules* excludedtaxaArgRules        = new ArgumentRules();    
//    ArgumentRules* excludedcharsArgRules       = new ArgumentRules();    
//    ArgumentRules* includedtaxaArgRules        = new ArgumentRules();    
//    ArgumentRules* includedcharsArgRules       = new ArgumentRules();    
//    ArgumentRules* nconstantpatternsArgRules   = new ArgumentRules();    
//    ArgumentRules* ncharswithambiguityArgRules = new ArgumentRules();
    ArgumentRules* excludecharArgRules         = new ArgumentRules();
    ArgumentRules* excludecharArgRules2        = new ArgumentRules();
    ArgumentRules* showdataArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();   
    
        
    excludecharArgRules->push_back(        new ArgumentRule(     "", true, Natural::getClassTypeSpec()       ) );
    excludecharArgRules2->push_back(       new ArgumentRule(     "", true, TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ) ) );
        
    methods.addFunction("names",               new MemberFunction(TypeSpec(Vector<RlString>::getClassTypeSpec(), new TypeSpec( RlString::getClassTypeSpec() ) ),  namesArgRules              ) );
    methods.addFunction("nchar",               new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), ncharArgRules              ) );
    methods.addFunction("ntaxa",               new MemberFunction(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
    methods.addFunction("chartype",            new MemberFunction(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
//    methods.addFunction("nexcludedtaxa",       new MemberFunction(Natural::getClassTypeSpec(),       nexcludedtaxaArgRules      ) );
//    methods.addFunction("nexcludedchars",      new MemberFunction(Natural::getClassTypeSpec(),       nexcludedcharsArgRules     ) );
//    methods.addFunction("nincludedtaxa",       new MemberFunction(Natural::getClassTypeSpec(),       nincludedtaxaArgRules      ) );
//    methods.addFunction("nincludedchars",      new MemberFunction(Natural::getClassTypeSpec(),       nincludedcharsArgRules     ) );
//    methods.addFunction("excludedtaxa",        new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedtaxaArgRules       ) );
//    methods.addFunction("excludedchars",       new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedcharsArgRules      ) );
//    methods.addFunction("includedtaxa",        new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedtaxaArgRules       ) );
//    methods.addFunction("includedchars",       new MemberFunction(TypeSpec(Vector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedcharsArgRules      ) );
//    methods.addFunction("nconstantpatterns",   new MemberFunction(Natural::getClassTypeSpec(),       nconstantpatternsArgRules  ) );
//    methods.addFunction("ncharswithambiguity", new MemberFunction(Natural::getClassTypeSpec(),       ncharswithambiguityArgRules) );
    methods.addFunction("excludeCharacter",    new MemberFunction(RlUtils::Void,        excludecharArgRules        ) );
    methods.addFunction("excludeCharacter",    new MemberFunction(RlUtils::Void,        excludecharArgRules2       ) );
    methods.addFunction("show",                new MemberFunction(RlUtils::Void,        showdataArgRules           ) );
    methods.addFunction("ishomologous",        new MemberFunction(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    
        
    
    // add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
    // necessary call for proper inheritance
    methods.setParentTable( &ModelObject<RevBayesCore::AbstractCharacterData>::getMethods() );
    
    
}


