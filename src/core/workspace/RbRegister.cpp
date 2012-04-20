/**
 * @file
 * This file contains the Workspace function that adds types and functions
 * to the global workspace, registering them with the interpreter/compiler
 * during the process.
 *
 * @brief Function registering language objects
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "ConstructorFunction.h"
#include "ConstructorFunctionForSimpleObjects.h"
#include "Distribution.h"
#include "DistributionFunction.h"
#include "DistributionContinuous.h"
#include "FunctionTable.h"
#include "ParserDistribution.h"
#include "ParserDistributionContinuous.h"
#include "ParserFunction.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbAbstract.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbObject.h"
#include "UserInterface.h"
#include "Workspace.h"

/* Primitive types (alphabetic order) */
#include "AminoAcidState.h"
#include "RbBoolean.h"
#include "CharacterContinuous.h"
#include "Complex.h"
#include "DnaState.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RateMatrix.h"
#include "RbString.h"
#include "Real.h"
#include "RealPos.h"
#include "RnaState.h"
#include "StandardState.h"
#include "TransitionProbabilityMatrix.h"

/* Container types (alphabetic order) */
#include "DagNodeContainer.h"
#include "Matrix.h"
#include "Set.h"
#include "RbVector.h"

/* MemberObject types without auto-generated constructors(alphabetic order) */
#include "CharacterData.h"
#include "Model.h"
#include "Simplex.h"
#include "TaxonData.h"
#include "Topology.h"
#include "TopologyNode.h"

/* MemberObject types with auto-generated constructors (alphabetic order) */
#include "FileMonitor.h"
#include "Mcmc.h"
#include "Mixture.h"
#include "Move_changeClassProbabilities.h"
#include "Move_mmultinomial.h"
#include "Move_mscale.h"
#include "Move_msimplex.h"
#include "Move_mslide.h"
#include "Move_reallocate.h"
#include "Move_reallocateAll.h"
#include "ObjectMonitor.h"
#include "Plate.h"
#include "Simulate.h"
#include "TreePlate.h"

/* Distributions with distribution constructors and distribution functions (alphabetic order) */
#include "Dist_beta.h"
#include "Dist_birthdeath.h"
#include "Dist_cat.h"
#include "Dist_ctmm.h"
#include "Dist_dtmm.h"
#include "Dist_dirichlet.h"
#include "Dist_exp.h"
#include "Dist_gamma.h"
#include "Dist_logis.h"
#include "Dist_multinomial.h"
#include "Dist_norm.h"
#include "Dist_lnorm.h"
#include "Dist_topologyunif.h"
#include "Dist_unif.h"

/* Basic internal functions (alphabetic order) */
#include "Func__range.h"

/* Basic arithmetic/logic templated functions */
#include "Func__add.h"
#include "Func__and.h"
#include "Func__div.h"
#include "Func__eq.h"
#include "Func__ge.h"
#include "Func__gt.h"
#include "Func__le.h"
#include "Func__lt.h"
#include "Func__mul.h"
#include "Func__ne.h"
#include "Func__or.h"
#include "Func__range.h"
#include "Func__sub.h"
#include "Func__uminus.h"
#include "Func__unot.h"
#include "Func__uplus.h"

/* Builtin functions */
#include "Func_clamp.h"
#include "Func_clear.h"
#include "Func_dppConFromExpNumClusters.h"
#include "Func_dppSeat.h"
#include "Func_dppServe.h"
#include "Func_ls.h"
#include "Func_normalizeVector.h"
#include "Func_print.h"
#include "Func_quit.h"
#include "Func_setval.h"
#include "Func_simplex.h"
#include "Func_sumOver.h"
#include "Func_structure.h"
#include "Func_type.h"
#include "Func_unclamp.h"
#include "Func_unique.h"
#include "Func_size.h"
#include "Func_sort.h"

/* Builtin templated functions */
#include "Func_transpose.h"
#include "Func_vector.h"

/* Math functions */
#include "Func_abs.h"
#include "Func_cos.h"
#include "Func_exp.h"
#include "Func_ln.h"
#include "Func_log.h"
#include "Func_mean.h"
#include "Func_power.h"
#include "Func_sin.h"
#include "Func_sqrt.h"

/* Constructor functions */
#include "ConstructorTaxonData.h"

/* Phylogeny functions */
#include "Func_concatenate.h"
#include "Func_distance.h"
#include "Func_gtr.h"
#include "Func_nj.h"
#include "Func_readCharacterData.h"
#include "Func_readTrees.h"
#include "Func_CtmmTransitionProbabilities.h"

#include <sstream>
#include <vector>
#include <cstdlib>

/** Initialize global workspace */
void Workspace::initializeGlobalWorkspace(void) {

    try {
        /* Add types: add a dummy variable which we use for type checking, conversion checking and other tasks. */

        /* Add special abstract types that do not correspond directly to classes */
        addType( new RbAbstract( TypeSpec(RbVoid_name) ) );

        /* Add abstract types */
        addType( new RbAbstract( RbObject::getClassTypeSpec() ) );
        addType( new RbAbstract( RbLanguageObject::getClassTypeSpec() ) );
        addType( new RbAbstract( RbInternal::getClassTypeSpec() ) );
        addType( new RbAbstract( MemberObject::getClassTypeSpec() ) );
        addType( new RbAbstract( Move::getClassTypeSpec() ) );
        addType( new RbAbstract( Distribution::getClassTypeSpec() ) );

        /* Add primitive types (alphabetic order) */
        addType( new AminoAcidState()                 );
        addType( new RbBoolean()                      );
        addType( new Complex()                        );
        addType( new CharacterContinuous()            );
        addType( new Integer()                        );
        addType( new Natural()                        );
        addType( new Probability()                    );
        addType( new RateMatrix()                     );
        addType( new RbString()                       );
        addType( new Real()                           );
        addType( new RealPos()                        );
        addType( new RnaState()                       );
        addType( new StandardState()                  );
        addType( new TransitionProbabilityMatrix()    );
        
        /* Add MemberObject types with auto-generated constructors (alphabetic order) */
        addTypeWithConstructor( "dna",         new DnaState()              );

        /* Add container types (alphabetic order) */
        addType( new DagNodeContainer()             );
        addType( new Matrix<Complex>()              );
        addType( new Matrix<Real>()                 );
        addTypeWithConstructor( "set",         new Set<Integer>()          );
        addTypeWithConstructor( "set",         new Set<Natural>()          );
        addTypeWithConstructor( "set",         new Set<NucleotideState>()  );
        addTypeWithConstructor( "set",         new Set<Real>()             );
        addTypeWithConstructor( "set",         new Set<RealPos>()          );
        addType( new RbVector<RbLanguageObject>()   );
        addType( new RbVector<RbBoolean>()          );
        addType( new RbVector<Integer>()            );
        addType( new RbVector<Natural>()            );
        addType( new RbVector<Real>()               );
        addType( new RbVector<RealPos>()            );
        addType( new RbVector<RbString>()           );

        /* Add MemberObject types without auto-generated constructors (alphabetic order) */
        addType( new Simplex()                      );
        addType( new Topology()                     );
        addType( new TopologyNode()                 );

        /* Add MemberObject types with auto-generated constructors (alphabetic order) */
        addTypeWithConstructor( "mcmc",          new Mcmc()              );
        addTypeWithConstructor( "fileMonitor",   new FileMonitor()       );
        addTypeWithConstructor( "mixture",       new Mixture()           );
        addTypeWithConstructor( "mmultinomial",  new Move_mmultinomial() );
        addTypeWithConstructor( "model",         new Model()             );
        addTypeWithConstructor( "msimplex",      new Move_msimplex()     );
        addTypeWithConstructor( "mslide",        new Move_mslide()       );
        addTypeWithConstructor( "mscale",        new Move_mscale()       );
        addTypeWithConstructor( "mreallocate",    new Move_reallocate()       );
        addTypeWithConstructor( "mreallocateAll",    new Move_reallocateAll()       );
        addTypeWithConstructor( "mchangeClassProbabilities",    new Move_changeClassProbabilities()       );
        addTypeWithConstructor( "plate",         new Plate()             );
        addTypeWithConstructor( "objectMonitor", new ObjectMonitor()     );
        addTypeWithConstructor( "simulate",      new Simulate()          );
        addTypeWithConstructor( "treeplate",     new TreePlate()         );
        
        /* Add phylogenetic types with auto-generated constructors (alphabetic order) */
        // \TODO: Does this really make sense to use the general character type?! (Sebastian)
        addTypeWithConstructor( "taxonData",     new TaxonData( Character::getClassName() ) );
        addTypeWithConstructor( "characterData", new CharacterData(DnaState::getClassName())   );


        /* Add Distribution types with auto-generated constructors and distribution functions (alphabetic order) */
        addDistribution( "beta",         new Dist_beta()        );
        addDistribution( "birthdeath",   new Dist_birthdeath()  );
        addDistribution( "cat",          new Dist_cat<Integer>());
        addDistribution( "cat",          new Dist_cat<Natural>());
        addDistribution( "cat",          new Dist_cat<NucleotideState>());
        addDistribution( "cat",          new Dist_cat<Real>()   );
        addDistribution( "cat",          new Dist_cat<RealPos>());
        addDistribution( "ctmc",         new Dist_ctmm()        );
        addDistribution( "dtmm",         new Dist_dtmm()        );
        addDistribution( "dirichlet",    new Dist_dirichlet()   );
        addDistribution( "exponential",  new Dist_exp()         );
        addDistribution( "gamma",        new Dist_gamma()       );
        addDistribution( "logis",        new Dist_logis()       );
        addDistribution( "multinomial",  new Dist_multinomial() );
        addDistribution( "lnorm",        new Dist_lnorm()       );
        addDistribution( "unifTopology", new Dist_topologyunif());
        addDistribution( "unif",         new Dist_unif()        );
        
        MemberRules distNormMemberRules;
        distNormMemberRules.push_back( new ValueRule( "mean", Real::getClassTypeSpec()   , new Real(0.0)    ) );
        distNormMemberRules.push_back( new ValueRule( "sd"  , RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        addDistribution( "norm",         new ParserDistributionContinuous( new Dist_norm(), distNormMemberRules, new Real() ) );

        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;

        /* Add basic internal functions (alphabetic order) */
        addFunction( "_range",    new Func__range()       );

        /* Add basic unary arithmetic and logical templated functions */
        addFunction( "_uplus",    new Func__uplus <         Integer,        Integer >() );
        addFunction( "_uplus",    new Func__uplus <            Real,           Real >() );
        addFunction( "_uplus",    new Func__uplus <    Matrix<Real>,   Matrix<Real> >() );
        addFunction( "_uminus",   new Func__uminus<         Integer,        Integer >() );
        addFunction( "_uminus",   new Func__uminus<            Real,           Real >() );
        addFunction( "_uminus",   new Func__uminus<    Matrix<Real>,   Matrix<Real> >() );
        addFunction( "_unot",     new Func__unot  <       RbBoolean                 >() );
        addFunction( "_unot",     new Func__unot  <         Integer                 >() );
        addFunction( "_unot",     new Func__unot  <            Real                 >() );

        /* Add basic arithmetic templated functions */
        ArgumentRules addNaturalFuncArgRules;
        addNaturalFuncArgRules.push_back( new ValueRule("first", Natural::getClassTypeSpec() ) );
        addNaturalFuncArgRules.push_back( new ValueRule("second", Natural::getClassTypeSpec() ) );
        Natural* funcAddNaturalRetVar = new Natural();
        addFunction( "_add",      new ParserFunction( new Func__add<int, int, int>(), "+", addNaturalFuncArgRules, funcAddNaturalRetVar ) );
        
        ArgumentRules addIntFuncArgRules;
        addIntFuncArgRules.push_back( new ValueRule("first", Integer::getClassTypeSpec() ) );
        addIntFuncArgRules.push_back( new ValueRule("second", Integer::getClassTypeSpec() ) );
        Integer* funcAddIntRetVar = new Integer();
        addFunction( "_add",      new ParserFunction( new Func__add<int, int, int>(), "+", addIntFuncArgRules, funcAddIntRetVar ) );
        
        ArgumentRules addRealPosFuncArgRules;
        addRealPosFuncArgRules.push_back( new ValueRule("first", RealPos::getClassTypeSpec() ) );
        addRealPosFuncArgRules.push_back( new ValueRule("second", RealPos::getClassTypeSpec() ) );
        RealPos* funcAddRealPosRetVar = new RealPos();
        addFunction( "_add",      new ParserFunction( new Func__add<double, double, double>(), "+", addRealPosFuncArgRules, funcAddRealPosRetVar ) );
        
        ArgumentRules addRealFuncArgRules;
        addRealFuncArgRules.push_back( new ValueRule("first", Real::getClassTypeSpec() ) );
        addRealFuncArgRules.push_back( new ValueRule("second", Real::getClassTypeSpec() ) );
        Real* funcAddRealRetVar = new Real();
        addFunction( "_add",      new ParserFunction( new Func__add<double, double, double>(), "+", addRealFuncArgRules, funcAddRealRetVar ) );
        
        ArgumentRules addStringFuncArgRules;
        addStringFuncArgRules.push_back( new ValueRule("first", RbString::getClassTypeSpec() ) );
        addStringFuncArgRules.push_back( new ValueRule("second", RbString::getClassTypeSpec() ) );
        RbString* funcAddStringRetVar = new RbString();
        addFunction( "_add",      new ParserFunction( new Func__add<std::string, std::string, std::string>(), "+", addStringFuncArgRules, funcAddStringRetVar ) );

        ArgumentRules subNaturalFuncArgRules;
        subNaturalFuncArgRules.push_back( new ValueRule("first", Natural::getClassTypeSpec() ) );
        subNaturalFuncArgRules.push_back( new ValueRule("second", Natural::getClassTypeSpec() ) );
        Natural* funcSubNaturalRetVar = new Natural();
        addFunction( "_sub",      new ParserFunction( new Func__sub<int, int, int>(), "-", subNaturalFuncArgRules, funcSubNaturalRetVar ) );
        
        ArgumentRules subIntFuncArgRules;
        subIntFuncArgRules.push_back( new ValueRule("first", Integer::getClassTypeSpec() ) );
        subIntFuncArgRules.push_back( new ValueRule("second", Integer::getClassTypeSpec() ) );
        Integer* funcSubIntRetVar = new Integer();
        addFunction( "_sub",      new ParserFunction( new Func__sub<int, int, int>(), "-", subIntFuncArgRules, funcSubIntRetVar ) );
        
        ArgumentRules subRealPosFuncArgRules;
        subRealPosFuncArgRules.push_back( new ValueRule("first", RealPos::getClassTypeSpec() ) );
        subRealPosFuncArgRules.push_back( new ValueRule("second", RealPos::getClassTypeSpec() ) );
        RealPos* funcSubRealPosRetVar = new RealPos();
        addFunction( "_sub",      new ParserFunction( new Func__sub<double, double, double>(), "-", subRealPosFuncArgRules, funcSubRealPosRetVar ) );
        
        ArgumentRules subRealFuncArgRules;
        subRealFuncArgRules.push_back( new ValueRule("first", Real::getClassTypeSpec() ) );
        subRealFuncArgRules.push_back( new ValueRule("second", Real::getClassTypeSpec() ) );
        Real* funcSubRealRetVar = new Real();
        addFunction( "_sub",      new ParserFunction( new Func__sub<double, double, double>(), "-", subRealFuncArgRules, funcSubRealRetVar ) );
        


        ArgumentRules ltNaturalFuncArgRules;
        ltNaturalFuncArgRules.push_back( new ValueRule("first", Natural::getClassTypeSpec() ) );
        ltNaturalFuncArgRules.push_back( new ValueRule("second", Natural::getClassTypeSpec() ) );
        Natural* funcLTNaturalRetVar = new Natural();
        addFunction( "_lt",      new ParserFunction( new Func__lt<int, int, int>(), "<", ltNaturalFuncArgRules, funcLTNaturalRetVar ) );
        
//        addFunction( "_add",      new Func__add<            Integer,           Real,         Real >() );
//        addFunction( "_add",      new Func__add<               Real,        Integer,         Real >() );
//        addFunction( "_add",      new Func__add<       Matrix<Real>,   Matrix<Real>, Matrix<Real> >() );
        addFunction( "_div",      new Func__div<            Natural,        Natural,      RealPos >() );
        addFunction( "_div",      new Func__div<            Integer,        Integer,         Real >() );
        addFunction( "_div",      new Func__div<            RealPos,        RealPos,      RealPos >() );
        addFunction( "_div",      new Func__div<               Real,           Real,         Real >() );
        addFunction( "_div",      new Func__div<            Integer,           Real,         Real >() );
        addFunction( "_div",      new Func__div<               Real,        Integer,         Real >() );
        addFunction( "_div",      new Func__div<       Matrix<Real>,   Matrix<Real>, Matrix<Real> >() );
        addFunction( "_exp",      new Func_power()                                                    );
        addFunction( "_mul",      new Func__mul<            Natural,        Natural,      Natural >() );
        addFunction( "_mul",      new Func__mul<            Integer,        Integer,      Integer >() );
        addFunction( "_mul",      new Func__mul<            RealPos,        RealPos,      RealPos >() );
        addFunction( "_mul",      new Func__mul<               Real,           Real,         Real >() );
        addFunction( "_mul",      new Func__mul<            Real,           RealPos,         Real >() );
        addFunction( "_mul",      new Func__mul<            RealPos,           Real,         Real >() );
        addFunction( "_mul",      new Func__mul<            Integer,           Real,         Real >() );
        addFunction( "_mul",      new Func__mul<               Real,        Integer,         Real >() );
        addFunction( "_mul",      new Func__mul<       Matrix<Real>,   Matrix<Real>, Matrix<Real> >() );
        addFunction( "_mul",      new Func__mul<       Matrix<Real>,           Real, Matrix<Real> >() );
        addFunction( "_mul",      new Func__mul<               Real,   Matrix<Real>, Matrix<Real> >() );
//        addFunction( "_sub",      new Func__sub<            Integer,        Integer,      Integer >() );
//        addFunction( "_sub",      new Func__sub<               Real,           Real,         Real >() );
//        addFunction( "_sub",      new Func__sub<            Integer,           Real,         Real >() );
//        addFunction( "_sub",      new Func__sub<               Real,        Integer,         Real >() );
//        addFunction( "_sub",      new Func__sub<       Matrix<Real>,   Matrix<Real>, Matrix<Real> >() );

        /* Add basic logic templated functions */
        addFunction( "_and",      new Func__and<            Integer,        Integer >()             );
        addFunction( "_and",      new Func__and<               Real,           Real >()             );
        addFunction( "_and",      new Func__and<            Integer,           Real >()             );
        addFunction( "_and",      new Func__and<               Real,        Integer >()             );
        addFunction( "_and",      new Func__and<          RbBoolean,      RbBoolean >()             );
        addFunction( "_and",      new Func__and<          RbBoolean,        Integer >()             );
        addFunction( "_and",      new Func__and<          RbBoolean,           Real >()             );
        addFunction( "_and",      new Func__and<            Integer,      RbBoolean >()             );
        addFunction( "_and",      new Func__and<               Real,      RbBoolean >()             );
        addFunction( "_eq",       new Func__eq<             Integer,        Integer >()             );
        addFunction( "_eq",       new Func__eq<                Real,           Real >()             );
        addFunction( "_eq",       new Func__eq<             Integer,           Real >()             );
        addFunction( "_eq",       new Func__eq<                Real,        Integer >()             );
        addFunction( "_eq",       new Func__eq<           RbBoolean,      RbBoolean >()             );
        addFunction( "_ge",       new Func__ge<             Integer,        Integer >()             );
        addFunction( "_ge",       new Func__ge<                Real,           Real >()             );
        addFunction( "_ge",       new Func__ge<             Integer,           Real >()             );
        addFunction( "_ge",       new Func__ge<                Real,        Integer >()             );
        addFunction( "_ge",       new Func__ge<           RbBoolean,      RbBoolean >()             );
        addFunction( "_gt",       new Func__gt<             Integer,        Integer >()             );
        addFunction( "_gt",       new Func__gt<                Real,           Real >()             );
        addFunction( "_gt",       new Func__gt<             Integer,           Real >()             );
        addFunction( "_gt",       new Func__gt<                Real,        Integer >()             );
        addFunction( "_gt",       new Func__gt<           RbBoolean,      RbBoolean >()             );
//        addFunction( "_lt",       new Func__lt<             Integer,        Integer >()             );
//        addFunction( "_lt",       new Func__lt<                Real,           Real >()             );
//        addFunction( "_lt",       new Func__lt<             Integer,           Real >()             );
//        addFunction( "_lt",       new Func__lt<                Real,        Integer >()             );
//        addFunction( "_lt",       new Func__lt<           RbBoolean,      RbBoolean >()             );
        addFunction( "_le",       new Func__le<             Integer,        Integer >()             );
        addFunction( "_le",       new Func__le<                Real,           Real >()             );
        addFunction( "_le",       new Func__le<             Integer,           Real >()             );
        addFunction( "_le",       new Func__le<                Real,        Integer >()             );
        addFunction( "_le",       new Func__le<           RbBoolean,      RbBoolean >()             );
        addFunction( "_ne",       new Func__ne<             Integer,        Integer >()             );
        addFunction( "_ne",       new Func__ne<                Real,           Real >()             );
        addFunction( "_ne",       new Func__ne<             Integer,           Real >()             );
        addFunction( "_ne",       new Func__ne<                Real,        Integer >()             );
        addFunction( "_ne",       new Func__ne<           RbBoolean,      RbBoolean >()             );
        addFunction( "_or",       new Func__or<             Integer,        Integer >()             );
        addFunction( "_or",       new Func__or<                Real,           Real >()             );
        addFunction( "_or",       new Func__or<             Integer,           Real >()             );
        addFunction( "_or",       new Func__or<                Real,        Integer >()             );
        addFunction( "_or",       new Func__or<           RbBoolean,      RbBoolean >()             );

        /* Add builtin functions (alphabetical order) */
        addFunction( "clamp",                    new Func_clamp()                    );
        addFunction( "clear",                    new Func_clear()                    );
        addFunction( "dppConFromExpNumClusters", new Func_dppConFromExpNumClusters() );
        addFunction( "dppSeat",                  new Func_dppSeat()                  );
        addFunction( "dppServe",                 new Func_dppServe()                 );
        addFunction( "ls",                       new Func_ls()                       );
        addFunction( "normalize",                new Func_normalizeVector()          );
        addFunction( "print",                    new Func_print()                    );
        addFunction( "q",                        new Func_quit()                     );
        addFunction( "quit",                     new Func_quit()                     );
        addFunction( "setval",                   new Func_setval()                   );
        addFunction( "simplex",                  new Func_simplex<Integer>()         );
        addFunction( "simplex",                  new Func_simplex<RealPos>()         );
        addFunction( "simplex",                  new Func_simplex<RbVector<RealPos> >() );
        addFunction( "sumOver",                  new Func_sumOver()                  );
        addFunction( "structure",                new Func_structure()                );
        addFunction( "type",                     new Func_type()                     );
        addFunction( "unclamp",                  new Func_unclamp()                  );
        addFunction( "unique",                   new Func_unique<RbVector<RbBoolean> >()  );
        addFunction( "unique",                   new Func_unique<RbVector<Integer> >()    );
        addFunction( "unique",                   new Func_unique<RbVector<Natural> >()    );
        addFunction( "unique",                   new Func_unique<RbVector<Real> >()       );
        addFunction( "unique",                   new Func_unique<RbVector<RealPos> >()    );
        addFunction( "unique",                   new Func_unique<RbVector<Complex> >()    );
        addFunction( "unique",                   new Func_unique<RbVector<RbString> >()   );
        addFunction( "size",                     new Func_size<DagNodeContainer>()   );
        addFunction( "size",                     new Func_size<RbVector<RbBoolean> >()    );
        addFunction( "size",                     new Func_size<RbVector<Integer> >()      );
        addFunction( "size",                     new Func_size<RbVector<Natural> >()      );
        addFunction( "size",                     new Func_size<RbVector<Real> >()         );
        addFunction( "size",                     new Func_size<RbVector<RealPos> >()      );
        addFunction( "size",                     new Func_size<RbVector<Complex> >()      );
        addFunction( "size",                     new Func_size<RbVector<RbString> >()     );
        addFunction( "sort",                     new Func_sort<RbVector<RbBoolean> >()    );
        addFunction( "sort",                     new Func_sort<RbVector<Integer> >()      );
        addFunction( "sort",                     new Func_sort<RbVector<Natural> >()      );
        addFunction( "sort",                     new Func_sort<RbVector<Real> >()         );
        addFunction( "sort",                     new Func_sort<RbVector<RealPos> >()      );
        addFunction( "sort",                     new Func_sort<RbVector<Complex> >()      );
        addFunction( "sort",                     new Func_sort<RbVector<RbString> >()     );
        addFunction( "sort",                     new Func_sort<RbVector<RbLanguageObject> >());

        /* Add math functions (alphabetical order) */
        addFunction( "abs",       new Func_abs()   );
        addFunction( "cos",       new Func_cos()   );
        
        ArgumentRules expFuncArgRules;
        expFuncArgRules.push_back( new ValueRule("x", Real::getClassTypeSpec() ) );
        RbLanguageObject* expFuncRetArg = new RealPos();
        addFunction( "exp",       new ParserFunction( new Func_exp(), "exponential function", expFuncArgRules, expFuncRetArg, false )  );
        
        addFunction( "ln",        new Func_ln()    );
        addFunction( "log",       new Func_log()   );
        addFunction( "mean",      new Func_mean()  );
        addFunction( "power",     new Func_power() );
        addFunction( "sin",       new Func_sin()   );
        addFunction( "sqrt",      new Func_sqrt()  );
        
        /* Add constructuor functions (alphabetical order) */
        addFunction( "taxonData", new ConstructorTaxonData() );
        
        /* Add phylogeny-related functions (alphabetical order) */
        addFunction( "concatenate",                 new Func_concatenate()                 );
        addFunction( "distances",                   new Func_distance()                    );
        addFunction( "gtr",                         new Func_gtr()                         );
        addFunction( "nj",                          new Func_nj()                          );
        addFunction( "read",                        new Func_readCharacterData()           );
        addFunction( "readTrees",                   new Func_readTrees()                   );
        addFunction( "ctmmtp",                      new Func_CtmmTransitionProbabilities() );
        addFunction( "ctmmTransitionProbabilities", new Func_CtmmTransitionProbabilities() );

        /* Add builtin templated functions */
        addFunction( "transpose", new Func_transpose<       Matrix<Real>                                                    >() );
        addFunction( "v",         new Func_vector<          RbBoolean,                      RbVector<RbBoolean>             >() );
        addFunction( "v",         new Func_vector<          Integer,                        RbVector<Integer>               >() );
        addFunction( "v",         new Func_vector<          Natural,                        RbVector<Natural>               >() );
        addFunction( "v",         new Func_vector<          Real,                           RbVector<Real>                  >() );
        addFunction( "v",         new Func_vector<          RealPos,                        RbVector<RealPos>               >() );
        addFunction( "v",         new Func_vector<          Complex,                        RbVector<Complex>               >() );
        addFunction( "v",         new Func_vector<          RbString,                       RbVector<RbString>              >() );
        addFunction( "v",         new Func_vector<          RbVector<Integer>,              Matrix<Integer>                 >() );
        addFunction( "v",         new Func_vector<          RbVector<Natural>,              Matrix<Natural>                 >() );
        addFunction( "v",         new Func_vector<          RbVector<Real>,                 Matrix<Real>                    >() );
        addFunction( "v",         new Func_vector<          RbVector<RealPos>,              Matrix<RealPos>                 >() );
        addFunction( "v",         new Func_vector<          RbVector<Complex>,              Matrix<Complex>                 >() );
    }
    catch(RbException& rbException) {

        RBOUT("Caught an exception while initializing the workspace\n");
        std::ostringstream msg;
        rbException.printValue(msg);
        msg << std::endl;
        RBOUT(msg.str());

        RBOUT("Please report this bug to the RevBayes Development Core Team");

        RBOUT("Press any character to exit the program.");
        getchar();
        exit(1);
    }
}


