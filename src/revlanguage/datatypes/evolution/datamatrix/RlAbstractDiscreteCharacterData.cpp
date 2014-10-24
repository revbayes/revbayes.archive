#include "RlAbstractDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTaxonData.h"


using namespace RevLanguage;

AbstractDiscreteCharacterData::AbstractDiscreteCharacterData(void) : ModelObject<RevBayesCore::AbstractDiscreteCharacterData>()
{
    
    ArgumentRules* ncharArgRules               = new ArgumentRules();
    ArgumentRules* ncharArgRules2              = new ArgumentRules();
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
    ArgumentRules* excludeallArgRules          = new ArgumentRules();
    ArgumentRules* includeallArgRules          = new ArgumentRules();
    ArgumentRules* excludecharArgRules         = new ArgumentRules();
    ArgumentRules* excludecharArgRules2        = new ArgumentRules();
    ArgumentRules* includecharArgRules         = new ArgumentRules();
    ArgumentRules* includecharArgRules2        = new ArgumentRules();
    ArgumentRules* showdataArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules          = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules2         = new ArgumentRules();
    ArgumentRules* setTaxonNameArgRules        = new ArgumentRules();
    
    
    ncharArgRules2->push_back(             new ArgumentRule("taxon_index", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
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
    methods.addFunction("nchar",               new MemberProcedure(ModelVector<Natural>::getClassTypeSpec(),  ncharArgRules           ) );
    methods.addFunction("nchar",               new MemberProcedure(TypeSpec(Natural::getClassTypeSpec() ),    ncharArgRules2          ) );
    
    methods.addFunction("ntaxa",               new MemberProcedure(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
    methods.addFunction("chartype",            new MemberProcedure(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    //    methods.addFunction("nexcludedtaxa",       new MemberProcedure(Natural::getClassTypeSpec(),       nexcludedtaxaArgRules      ) );
    //    methods.addFunction("nexcludedchars",      new MemberProcedure(Natural::getClassTypeSpec(),       nexcludedcharsArgRules     ) );
    //    methods.addFunction("nincludedtaxa",       new MemberProcedure(Natural::getClassTypeSpec(),       nincludedtaxaArgRules      ) );
    //    methods.addFunction("nincludedchars",      new MemberProcedure(Natural::getClassTypeSpec(),       nincludedcharsArgRules     ) );
    //    methods.addFunction("excludedtaxa",        new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedtaxaArgRules       ) );
    //    methods.addFunction("excludedchars",       new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedcharsArgRules      ) );
    //    methods.addFunction("includedtaxa",        new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedtaxaArgRules       ) );
    //    methods.addFunction("includedchars",       new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedcharsArgRules      ) );
    //    methods.addFunction("nconstantpatterns",   new MemberProcedure(Natural::getClassTypeSpec(),       nconstantpatternsArgRules  ) );
    //    methods.addFunction("ncharswithambiguity", new MemberProcedure(Natural::getClassTypeSpec(),       ncharswithambiguityArgRules) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules        ) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules2       ) );
    methods.addFunction("excludeAll",          new MemberProcedure(RlUtils::Void,        excludeallArgRules         ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules        ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules2       ) );
    methods.addFunction("includeAll",          new MemberProcedure(RlUtils::Void,        includeallArgRules         ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules  ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules2 ) );
    methods.addFunction("show",                new MemberProcedure(RlUtils::Void,        showdataArgRules           ) );
    methods.addFunction("ishomologous",        new MemberProcedure(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("removeTaxon",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxon",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("setTaxonName",        new MemberProcedure(RlUtils::Void,        setTaxonNameArgRules       ) );
    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );

}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( RevBayesCore::AbstractDiscreteCharacterData *d) :ModelObject<RevBayesCore::AbstractDiscreteCharacterData>( d )
{
    
    ArgumentRules* ncharArgRules               = new ArgumentRules();
    ArgumentRules* ncharArgRules2              = new ArgumentRules();
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
    ArgumentRules* excludeallArgRules          = new ArgumentRules();
    ArgumentRules* includeallArgRules          = new ArgumentRules();
    ArgumentRules* excludecharArgRules         = new ArgumentRules();
    ArgumentRules* excludecharArgRules2        = new ArgumentRules();
    ArgumentRules* includecharArgRules         = new ArgumentRules();
    ArgumentRules* includecharArgRules2        = new ArgumentRules();
    ArgumentRules* showdataArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules          = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules2         = new ArgumentRules();
    ArgumentRules* setTaxonNameArgRules        = new ArgumentRules();
    
    
    ncharArgRules2->push_back(             new ArgumentRule("taxon_index", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
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
    methods.addFunction("nchar",               new MemberProcedure(ModelVector<Natural>::getClassTypeSpec(),  ncharArgRules           ) );
    methods.addFunction("nchar",               new MemberProcedure(TypeSpec(Natural::getClassTypeSpec() ),    ncharArgRules2          ) );
    
    methods.addFunction("ntaxa",               new MemberProcedure(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
    methods.addFunction("chartype",            new MemberProcedure(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    //    methods.addFunction("nexcludedtaxa",       new MemberProcedure(Natural::getClassTypeSpec(),       nexcludedtaxaArgRules      ) );
    //    methods.addFunction("nexcludedchars",      new MemberProcedure(Natural::getClassTypeSpec(),       nexcludedcharsArgRules     ) );
    //    methods.addFunction("nincludedtaxa",       new MemberProcedure(Natural::getClassTypeSpec(),       nincludedtaxaArgRules      ) );
    //    methods.addFunction("nincludedchars",      new MemberProcedure(Natural::getClassTypeSpec(),       nincludedcharsArgRules     ) );
    //    methods.addFunction("excludedtaxa",        new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedtaxaArgRules       ) );
    //    methods.addFunction("excludedchars",       new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedcharsArgRules      ) );
    //    methods.addFunction("includedtaxa",        new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedtaxaArgRules       ) );
    //    methods.addFunction("includedchars",       new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedcharsArgRules      ) );
    //    methods.addFunction("nconstantpatterns",   new MemberProcedure(Natural::getClassTypeSpec(),       nconstantpatternsArgRules  ) );
    //    methods.addFunction("ncharswithambiguity", new MemberProcedure(Natural::getClassTypeSpec(),       ncharswithambiguityArgRules) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules        ) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules2       ) );
    methods.addFunction("excludeAll",          new MemberProcedure(RlUtils::Void,        excludeallArgRules         ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules        ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules2       ) );
    methods.addFunction("includeAll",          new MemberProcedure(RlUtils::Void,        includeallArgRules         ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules  ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules2 ) );
    methods.addFunction("show",                new MemberProcedure(RlUtils::Void,        showdataArgRules           ) );
    methods.addFunction("ishomologous",        new MemberProcedure(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("removeTaxon",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxon",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("setTaxonName",        new MemberProcedure(RlUtils::Void,        setTaxonNameArgRules       ) );
    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );

}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( const RevBayesCore::AbstractDiscreteCharacterData &d) : ModelObject<RevBayesCore::AbstractDiscreteCharacterData>( d.clone() )
{
    
    ArgumentRules* ncharArgRules               = new ArgumentRules();
    ArgumentRules* ncharArgRules2              = new ArgumentRules();
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
    ArgumentRules* excludeallArgRules          = new ArgumentRules();
    ArgumentRules* includeallArgRules          = new ArgumentRules();
    ArgumentRules* excludecharArgRules         = new ArgumentRules();
    ArgumentRules* excludecharArgRules2        = new ArgumentRules();
    ArgumentRules* includecharArgRules         = new ArgumentRules();
    ArgumentRules* includecharArgRules2        = new ArgumentRules();
    ArgumentRules* showdataArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules          = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules2         = new ArgumentRules();
    ArgumentRules* setTaxonNameArgRules        = new ArgumentRules();
    
    
    ncharArgRules2->push_back(             new ArgumentRule("taxon_index", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
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
    methods.addFunction("nchar",               new MemberProcedure(ModelVector<Natural>::getClassTypeSpec(),  ncharArgRules           ) );
    methods.addFunction("nchar",               new MemberProcedure(TypeSpec(Natural::getClassTypeSpec() ),    ncharArgRules2          ) );
    
    methods.addFunction("ntaxa",               new MemberProcedure(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
    methods.addFunction("chartype",            new MemberProcedure(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    //    methods.addFunction("nexcludedtaxa",       new MemberProcedure(Natural::getClassTypeSpec(),       nexcludedtaxaArgRules      ) );
    //    methods.addFunction("nexcludedchars",      new MemberProcedure(Natural::getClassTypeSpec(),       nexcludedcharsArgRules     ) );
    //    methods.addFunction("nincludedtaxa",       new MemberProcedure(Natural::getClassTypeSpec(),       nincludedtaxaArgRules      ) );
    //    methods.addFunction("nincludedchars",      new MemberProcedure(Natural::getClassTypeSpec(),       nincludedcharsArgRules     ) );
    //    methods.addFunction("excludedtaxa",        new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedtaxaArgRules       ) );
    //    methods.addFunction("excludedchars",       new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedcharsArgRules      ) );
    //    methods.addFunction("includedtaxa",        new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedtaxaArgRules       ) );
    //    methods.addFunction("includedchars",       new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedcharsArgRules      ) );
    //    methods.addFunction("nconstantpatterns",   new MemberProcedure(Natural::getClassTypeSpec(),       nconstantpatternsArgRules  ) );
    //    methods.addFunction("ncharswithambiguity", new MemberProcedure(Natural::getClassTypeSpec(),       ncharswithambiguityArgRules) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules        ) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules2       ) );
    methods.addFunction("excludeAll",          new MemberProcedure(RlUtils::Void,        excludeallArgRules         ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules        ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules2       ) );
    methods.addFunction("includeAll",          new MemberProcedure(RlUtils::Void,        includeallArgRules         ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules  ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules2 ) );
    methods.addFunction("show",                new MemberProcedure(RlUtils::Void,        showdataArgRules           ) );
    methods.addFunction("ishomologous",        new MemberProcedure(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("removeTaxon",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxon",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("setTaxonName",        new MemberProcedure(RlUtils::Void,        setTaxonNameArgRules       ) );
    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );

}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData> *d) : ModelObject<RevBayesCore::AbstractDiscreteCharacterData>( d )
{
    
    ArgumentRules* ncharArgRules               = new ArgumentRules();
    ArgumentRules* ncharArgRules2              = new ArgumentRules();
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
    ArgumentRules* excludeallArgRules          = new ArgumentRules();
    ArgumentRules* includeallArgRules          = new ArgumentRules();
    ArgumentRules* excludecharArgRules         = new ArgumentRules();
    ArgumentRules* excludecharArgRules2        = new ArgumentRules();
    ArgumentRules* includecharArgRules         = new ArgumentRules();
    ArgumentRules* includecharArgRules2        = new ArgumentRules();
    ArgumentRules* showdataArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules        = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules   = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2  = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules          = new ArgumentRules();
    ArgumentRules* removeTaxaArgRules2         = new ArgumentRules();
    ArgumentRules* setTaxonNameArgRules        = new ArgumentRules();
    
    
    ncharArgRules2->push_back(             new ArgumentRule("taxon_index", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
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
    methods.addFunction("nchar",               new MemberProcedure(ModelVector<Natural>::getClassTypeSpec(),  ncharArgRules           ) );
    methods.addFunction("nchar",               new MemberProcedure(TypeSpec(Natural::getClassTypeSpec() ),    ncharArgRules2          ) );
    
    methods.addFunction("ntaxa",               new MemberProcedure(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
    methods.addFunction("chartype",            new MemberProcedure(RlString::getClassTypeSpec(),      chartypeArgRules           ) );
    //    methods.addFunction("nexcludedtaxa",       new MemberProcedure(Natural::getClassTypeSpec(),       nexcludedtaxaArgRules      ) );
    //    methods.addFunction("nexcludedchars",      new MemberProcedure(Natural::getClassTypeSpec(),       nexcludedcharsArgRules     ) );
    //    methods.addFunction("nincludedtaxa",       new MemberProcedure(Natural::getClassTypeSpec(),       nincludedtaxaArgRules      ) );
    //    methods.addFunction("nincludedchars",      new MemberProcedure(Natural::getClassTypeSpec(),       nincludedcharsArgRules     ) );
    //    methods.addFunction("excludedtaxa",        new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedtaxaArgRules       ) );
    //    methods.addFunction("excludedchars",       new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), excludedcharsArgRules      ) );
    //    methods.addFunction("includedtaxa",        new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedtaxaArgRules       ) );
    //    methods.addFunction("includedchars",       new MemberProcedure(TypeSpec(ModelVector<Natural>::getClassTypeSpec(), new TypeSpec( Natural::getClassTypeSpec() ) ), includedcharsArgRules      ) );
    //    methods.addFunction("nconstantpatterns",   new MemberProcedure(Natural::getClassTypeSpec(),       nconstantpatternsArgRules  ) );
    //    methods.addFunction("ncharswithambiguity", new MemberProcedure(Natural::getClassTypeSpec(),       ncharswithambiguityArgRules) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules        ) );
    methods.addFunction("excludeCharacter",    new MemberProcedure(RlUtils::Void,        excludecharArgRules2       ) );
    methods.addFunction("excludeAll",          new MemberProcedure(RlUtils::Void,        excludeallArgRules         ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules        ) );
    methods.addFunction("includeCharacter",    new MemberProcedure(RlUtils::Void,        includecharArgRules2       ) );
    methods.addFunction("includeAll",          new MemberProcedure(RlUtils::Void,        includeallArgRules         ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules  ) );
    methods.addFunction("setCodonPartition",   new MemberProcedure(RlUtils::Void,        setCodonPartitionArgRules2 ) );
    methods.addFunction("show",                new MemberProcedure(RlUtils::Void,        showdataArgRules           ) );
    methods.addFunction("ishomologous",        new MemberProcedure(RlBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxa",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("removeTaxon",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules       ) );
    methods.addFunction("removeTaxon",          new MemberProcedure(RlUtils::Void,        removeTaxaArgRules2        ) );
    methods.addFunction("setTaxonName",        new MemberProcedure(RlUtils::Void,        setTaxonNameArgRules       ) );
    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );

}



AbstractDiscreteCharacterData* AbstractDiscreteCharacterData::add(const RevObject &d) const
{
    const AbstractDiscreteCharacterData* tmp = dynamic_cast<const AbstractDiscreteCharacterData*>( &d );
    if ( tmp != NULL )
    {
        return add( *tmp );
    }
    else
    {
        throw RbException("Cannot add an object of type '" + d.getType() + "' to a character data object.");
    }
}



AbstractDiscreteCharacterData* AbstractDiscreteCharacterData::add(const AbstractDiscreteCharacterData &d) const
{
    AbstractDiscreteCharacterData* cloneObj = clone();
    cloneObj->getDagNode()->getValue().add( d.getValue() );
    
    // return the copy
    return cloneObj;
}



AbstractDiscreteCharacterData* AbstractDiscreteCharacterData::clone() const
{
    return new AbstractDiscreteCharacterData( *this );
}


/* Map calls to member methods */
RevPtr<Variable> AbstractDiscreteCharacterData::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    if (name == "chartype")
    {
        found = true;
        
        return new Variable( new RlString( this->dagNode->getValue().getDatatype() ) );
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
            dagNode->getValue().excludeCharacter( n-1 );
        }
        else if ( argument.isType( ModelVector<Natural>::getClassTypeSpec() ) )
        {
            const ModelVector<Natural>& x = static_cast<const ModelVector<Natural>&>( argument );
            RevBayesCore::AbstractDiscreteCharacterData &v = dagNode->getValue();
            for ( size_t i=0; i<x.size(); i++ )
            {
                // remember that we internally store the character indeces from 0 to n-1
                // but externally represent it as 1 to n
                v.excludeCharacter( size_t(x[i])-1 );
            }
        }
        return NULL;
    }
    else if (name == "setCodonPartition")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        RevBayesCore::AbstractDiscreteCharacterData &v = dagNode->getValue();
        size_t nChars = v.getNumberOfCharacters();
        
        // e.g. data.setCodonPartition(sites=v(3))
        if ( argument.isType( Natural::getClassTypeSpec() ) )
        {
            size_t n = size_t( static_cast<const Natural&>( argument ).getValue() );
            for (size_t i = 0; i < nChars; i++)
            {
                
                if (i % 3 == (n-1))
                {
                    v.includeCharacter(i);
                }
                else
                {
                    v.excludeCharacter(i);
                }
                
            }
        }
        
        // e.g. data.setCodonPartition(sites=v(1,2))
        else if ( argument.isType( ModelVector<Natural>::getClassTypeSpec() ) )
        {
            const ModelVector<Natural>& x = static_cast<const ModelVector<Natural>&>( argument );
            if (x.size() == 0)
            {
                return NULL;
            }
            
            for (size_t i = 0; i < nChars; i++)
            {
                v.excludeCharacter(i);
            }
            
            for (size_t i = 0; i < x.size(); i++)
            {
                size_t n = x[i];
                for (size_t j = 0; j < nChars; j++)
                {
                    
                    if (j % 3 == (n-1))
                    {
                        v.includeCharacter(j);
                    }
                    
                }
            }
        }
        return NULL;
    }
    else if (name == "excludeAll")
    {
        found = true;
        
        RevBayesCore::AbstractDiscreteCharacterData &v = dagNode->getValue();
        size_t nChars = v.getNumberOfCharacters();
        
        for (size_t i = 0; i < nChars; i++)
            v.excludeCharacter(i);
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
            dagNode->getValue().includeCharacter( n-1 );
        }
        else if ( argument.isType( ModelVector<Natural>::getClassTypeSpec() ) )
        {
            const ModelVector<Natural>& x = static_cast<const ModelVector<Natural>&>( argument );
            RevBayesCore::AbstractDiscreteCharacterData &v = dagNode->getValue();
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
        
        RevBayesCore::AbstractDiscreteCharacterData &v = dagNode->getValue();
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
        for (size_t i = 0; i < this->dagNode->getValue().getNumberOfTaxa(); ++i)
        {
            n->push_back( this->dagNode->getValue().getTaxonNameWithIndex( i ) );
        }
        
        return new Variable( n );
    }
    else if (name == "nchar")
    {
        found = true;
        
        // no arguments, return vector of number of chars per taxon
        if ( args.size() == 0 )
        {
            ModelVector<Natural> *numChar = new ModelVector<Natural>();
            for (size_t i=0; i<this->dagNode->getValue().getNumberOfTaxa(); i++)
            {
                
                if ( this->dagNode->getValue().isTaxonExcluded(i) == false )
                {
                    
                    if (this->dagNode->getValue().isHomologyEstablished() == true)
                    {
                        numChar->push_back( int(this->dagNode->getValue().getNumberOfIncludedCharacters()) );
                    }
                    else
                    {
                        numChar->push_back( int(this->dagNode->getValue().getNumberOfIncludedCharacters(i)) );
                    }
                    
                }
            }
            return new Variable( numChar );
        }
        else
        {
            const RevObject& argument = args[0].getVariable()->getRevObject();
            
            if ( argument.isType( Natural::getClassTypeSpec() ) )
            {
                Natural *numChar = NULL;
                const Natural& n = static_cast<const Natural&>( argument );
                size_t i = n.getValue() - 1; // index offset
                
                size_t numTaxa = this->dagNode->getValue().getNumberOfTaxa();
                if (i >= numTaxa)
                {
                    std::cout << "Warning: Returned value for taxon_index=1 since taxon_index > num_taxa (" << n.getValue() << " > " << numTaxa << ").\n";
                    i = 0;
                }
                
                if ( this->dagNode->getValue().isTaxonExcluded(i) == false )
                {
                    
                    if (this->dagNode->getValue().isHomologyEstablished() == true)
                    {
                        numChar = new Natural( this->dagNode->getValue().getNumberOfIncludedCharacters() );
                    }
                    else
                    {
                        numChar = new Natural( this->dagNode->getValue().getNumberOfIncludedCharacters(i) );
                    }
                    
                }
                return new Variable( numChar );
            }
        }
    }
    else if (name == "ntaxa")
    {
        found = true;
        
        int n = (int)this->dagNode->getValue().getNumberOfTaxa();
        
        return new Variable( new Natural(n) );
    }
    else if (name == "size")
    {
        found = true;
        
        int n = (int)this->dagNode->getValue().getNumberOfTaxa();
        
        return new Variable( new Natural(n) );
    }
    else if (name == "removeTaxa" || name == "removeTaxon" )
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        if ( argument.isType( RlString::getClassTypeSpec() ) )
        {
            std::string n = std::string( static_cast<const RlString&>( argument ).getValue() );
            dagNode->getValue().excludeTaxon( n );
        }
        else if ( argument.isType( ModelVector<RlString>::getClassTypeSpec() ) )
        {
            const ModelVector<RlString>& x = static_cast<const ModelVector<RlString>&>( argument );
            RevBayesCore::AbstractDiscreteCharacterData &v = dagNode->getValue();
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
                dagNode->getValue().setTaxonName( n ,name );
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
                {
                    std::cout << std::endl << "   ";
                }
                
            }
            
            std::cout << std::endl;
        }
        
        return NULL;
    }
    else if (name == "ishomologous")
    {
        found = true;
        
        bool ih = this->dagNode->getValue().isHomologyEstablished();
        
        return new Variable( new RlBoolean(ih) );
    }
    
    return ModelObject<RevBayesCore::AbstractDiscreteCharacterData>::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& AbstractDiscreteCharacterData::getClassType(void)
{
    
    static std::string revType = "AbstractDiscreteCharacterData";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractDiscreteCharacterData::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& AbstractDiscreteCharacterData::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


