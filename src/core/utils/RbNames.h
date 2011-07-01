/*!
 * @file
 * This file contains const strings naming the DAGNode and RbObject types
 * used in the program.
 *
 * @brief Names of variable types
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-09, version 1.0
 *
 * $Id:$
 */

#ifndef RbNames_H
#define RbNames_H

#include <string>

// Utility string constants
const std::string EmptyString                                   = "";
const std::string pad                                           = "   ";

// Names of types (classes)
//const std::string AlignmentFileType_nexus                       = "Nexus";
//const std::string AlignmentFileType_phylip                      = "phylip";
//const std::string AlignmentFileType_fasta                       = "fasta";
//const std::string AlignmentType_dna                             = "dna";
//const std::string AlignmentType_aa                              = "amino acid";
//const std::string AlignmentType_morph                           = "morphology";
const std::string AminoAcidState_name                           = "Amino Acid";
const std::string Argument_name                                 = "argument";
const std::string ArgumentRule_name                             = "argument rule";
const std::string Boolean_name                                  = "Bool";
const std::string Categorical_name                              = "Categorical";
const std::string CharacterMatrix_name                          = "Character Matrix";
const std::string CharacterObservation_name                     = "Character Observation";
const std::string CharacterObservationContinuous_name           = "Continuous Character Observation";
const std::string CharacterObservationDiscrete_name             = "Discrete Character Observation";
const std::string CharacterState_name                           = "Character State";
const std::string Complex_name                                  = "Complex number";
const std::string ConstantNode_name                             = "Constant Node";
const std::string ConstructorFunction_name                      = "constructor function";
const std::string Container_name                                = "container";
const std::string ContainerNode_name                            = "ContainerNode";
const std::string ConverterNode_name                            = "ConverterNode";
const std::string DAGNode_name                                  = "DAG Node";
const std::string DeterministicNode_name                        = "Deterministic Node";
const std::string Distribution_name                             = "Distribution";
const std::string DistributionCategorical_name                  = "Distribution on Categorical Random Variable";
const std::string DistributionFunction_name                     = "Distribution Function";
const std::string DistributionInterval_name                     = "Distribution on Interval Random Variable";
const std::string DistributionReal_name                         = "Distribution on Real Random Variable";
const std::string Dist_cat_name                                 = "Dist_cat";
const std::string Dist_dirichlet_name                           = "Dist_dirichlet";
const std::string Dist_exp_name                                 = "Dist_exp";
const std::string Dist_multinomial_name                         = "Dist_multinomial";
const std::string Dist_norm_name                                = "Dist_norm";
const std::string Dist_unif_name                                = "Dist_unif";
const std::string DNA_name                                      = "DNA";
const std::string DnaState_name                                 = "DNA";
const std::string Ellipsis_name                                 = "ellipsis";
const std::string Func__add_name                                = "Addition";
const std::string Func__range_name                              = "Range function";
const std::string Func_clamp_name                               = "Clamp function";
const std::string Func_exp_name                                 = "Exponential function";
const std::string Func_gtr_name                                 = "GTR function";
const std::string Func_ln_name                                  = "Natural logarithm function";
const std::string Func_log_name                                 = "Logarithm function";
const std::string Func_ls_name                                  = "Ls function";
const std::string Func_model_name                               = "Model function";
const std::string Func_normalize_name                           = "Normalize function";
const std::string Func_power_name                               = "Power function";
const std::string Func_quit_name                                = "Quit function";
const std::string Func_readAlignment_name                       = "Read alignment function";
const std::string Func_sin_name                                 = "Sin function";
const std::string Func_s_doublevec_name                         = "Simplex constructor function from list of Reals";
const std::string Func_s_int_name                               = "Simplex constructor function from a single integer";
const std::string Func_s_realvec_name                           = "Simplex constructor function from a Real vector";
const std::string Func_source_name                              = "Source function";
const std::string Func_sqrt_name                                = "Square root function";
const std::string Func_str_name                                 = "Str function";
const std::string Func_v_double_name                            = "Vector of reals constructor function";
const std::string Func_v_int_name                               = "Vector of integers constructor function";
const std::string Func_v_vecrealvec_name                        = "Matrix constructor function";
const std::string FunctionNode_name                             = "Function Node";
const std::string Integer_name                                  = "Integer";
const std::string IntegratorNode_name                           = "IntegratorNode";
const std::string List_name                                     = "List";
const std::string LookupNode_name                               = "Lookup Node";
const std::string Matrix_name                                   = "Matrix";
const std::string MatrixComplex_name                            = "Complex Matrix";
const std::string MatrixReal_name                               = "Real Matrix";
const std::string Mcmc_name                                     = "MCMC";
const std::string MemberNode_name                               = "member node";
const std::string MemberObject_name                             = "MemberObject";
const std::string MemberFunction_name                           = "member function";
const std::string Model_name                                    = "Model";
const std::string Monitor_name                                  = "Monitor";
const std::string Move_name                                     = "Move";
const std::string MoveSimple_name                               = "MoveSimple";
const std::string MoveSchedule_name                             = "move schedule";
const std::string Move_mmultinomial_name                        = "mmultinomial move";
const std::string Move_mscale_name                              = "mscale move";
const std::string Move_mslide_name                              = "mslide move";
const std::string Move_msimplex_name                            = "msimplex move";
const std::string Natural_name                                  = "Natural";
const std::string Nucleotide_name                               = "Nucleotide";
const std::string NucleotideState_name                          = "Nucleotide State";
const std::string Node_name                                     = "Node";
const std::string OptionRule_name                               = "OptionRule";
const std::string RandomNumberGenerator_name                    = "Random Number Generator";
const std::string RangeRule_name                                = "RangeRule";
const std::string RateMatrix_name                               = "Rate Matrix";
const std::string RbComplex_name                                = "complex";
const std::string RbException_name                              = "exception";
const std::string RbFunction_name                               = "function";
const std::string RbFunction_ln_name                            = "log_e";
const std::string RbFunction_pow_name                           = "a^b";
const std::string RbInternal_name                               = "internal";
const std::string RbMcmc_name                                   = "mcmc";
const std::string RbMonitor_name                                = "monitor";
const std::string RbMove_name                                   = "move";
const std::string RbMoveSchedule_name                           = "move(schedule)";
const std::string RbNode_name                                   = "node";
const std::string RbNULL_name                                   = "NULL";
const std::string RbVoid_name                                   = "void";
const std::string RbObject_name                                 = "Object";
const std::string RbPrimitive_name                              = "primitive";
const std::string RbString_name                                 = "String";
const std::string RbUndefined_name                              = "Undefined";
const std::string RetValFunction_name                           = "Return value function";
const std::string Real_name                                     = "Real";
const std::string RealPos_name                                  = "+Real";
const std::string ReferenceRule_name                            = "reference rule";
const std::string RNA_name                                      = "RNA";
const std::string RnaState_name                                 = "RNA";
const std::string Scalar_name                                   = "Scalar"; // Used for argument matching of "primitive" types
const std::string Simplex_name                                  = "Simplex";
const std::string StandardState_name                            = "Standard";
const std::string SyntaxAssignExpr_name                         = "Assignment";
const std::string SyntaxBinaryExpr_name                         = "Binary expression";
const std::string SyntaxConstant_name                           = "Syntax element";
const std::string SyntaxElement_name                            = "Syntax element";
const std::string SyntaxFunctionCall_name                       = "Function call";
const std::string SyntaxFunctionDef_name                        = "Function definition";
const std::string SyntaxLabeledExpr_name                        = "Labeled expression";
const std::string SyntaxUnaryExpr_name                          = "Unary expression";
const std::string SyntaxVariable_name                           = "Variable";
const std::string SyntaxVariableDecl_name                       = "Variable declaration";
const std::string StochasticNode_name                           = "Stochastic node";
const std::string Trace_name                                    = "Trace";
const std::string Tree_name                                     = "Tree";
const std::string TreeBinaryUnrooted_name                       = "Binary unrooted tree";
const std::string TreeSimple_name                               = "Simple tree";
const std::string TreeNode_name                                 = "Tree node";
const std::string TreeUnrooted_name                             = "Unrooted tree";
const std::string VariableContainer_name                        = "Variable container";
const std::string ValueContainer_name                           = "Value container";
const std::string ValueRule_name                                = "ValueRule";
const std::string VariableNode_name                             = "Variable node";
const std::string Vector_name                                   = "Vector";
const std::string VectorAminoAcidStates_name                    = "Amino acid vector";
const std::string VectorBoolean_name                            = "Boolean vector";
const std::string VectorCharacterObservations_name              = "Character vector";
const std::string VectorComplex_name                            = "Complex vector";
const std::string VectorContinuousObservations_name             = "Continuous characters vector";
const std::string VectorDnaStates_name                          = "DNA vector";
const std::string VectorIndex_name                              = "Index vector";
const std::string VectorInteger_name                            = "Integer vector";
const std::string VectorNatural_name                            = "Natural vector";
const std::string VectorObject_name                             = "Object vector";
const std::string VectorReal_name                               = "Real vector";
const std::string VectorRealPos_name                            = "+Real vector";
const std::string VectorRnaStates_name                          = "RNA vector";
const std::string VectorStandardStates_name                     = "Standard character vector";
const std::string VectorString_name                             = "String vector";
const std::string UserFunction_name                             = "user function";

#endif
