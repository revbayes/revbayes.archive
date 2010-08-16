/*!
 * \file
 * This file contains commonly used statistics functions that are used
 * in REvBayes. The probability density (pdf), log of the probability
 * density (lnPdf), cumulative probability (cdf), and quantiles of
 * common probability distributions.
 *
 * \brief Namespace containing statistical functions
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date: 2009-11-24 18:02:14 -0800 (Tue, 24 Nov 2009) $
 * \author Sebastian Hoehna (Department of Mathematics, Stockholm University)
 * \author John Huelsenbeck (Department of Integrative Biology, University of California, Berkeley)
 * \author Fredrik Ronquist (Department of Entomology, Swedish Museum of Natural History)
 * \license GPL version 3
 *
 * $Id: RbStatistics.h 100 2009-11-25 02:02:14Z jhuelsenbeck $
 */
#ifndef RbNames_H
#define RbNames_H

#include <string>

const std::string AlignmentFileType_nexus                       = "nexus";
const std::string AlignmentFileType_phylip                      = "phylip";
const std::string AlignmentFileType_fasta                       = "fasta";
const std::string AlignmentType_dna                             = "dna";
const std::string AlignmentType_aa                              = "amino acid";
const std::string AlignmentType_morph                           = "morphology";
const std::string Argument_name                                 = "argument";
const std::string ArgumentRule_name                             = "argument rule";
const std::string ConstantNode_name                             = "constant node";
const std::string ConstructorFunction_name                      = "constructor function";
const std::string Container_name                                = "container";
const std::string DAGNode_name                                  = "dag node";
const std::string DAGNodeContainer_name                         = "dag node container";
const std::string DeterministicNode_name                        = "deterministic node";
const std::string Distribution_name                             = "distribution";
const std::string DistributionFunction_name                     = "distribution function";
const std::string DistributionReal_name                         = "distribution on real";
const std::string Dist_exp_name                                 = "exponential distribution";
const std::string Dist_norm_name                                = "normal distribution";
const std::string Dist_unif_name                                = "uniform distribution";
const std::string Ellipsis_name                                 = "ellipsis";
const std::string Func__lookup_name                             = "_lookup function";
const std::string Func__range_name                              = "_range function";
const std::string Func_clamp_name                               = "clamp function";
const std::string Func_ls_name                                  = "ls function";
const std::string Func_model_name                               = "model function";
const std::string Func_source_name                              = "source function";
const std::string Func_quit_name                                = "quit function";
const std::string IntVector_name                                = "int vector";
const std::string Mcmc_name                                     = "mcmc";
const std::string MemberObject_name                             = "member object";
const std::string MemberFunction_name                           = "member function";
const std::string MinmaxRule_name                               = "minmax rule";
const std::string Model_name                                    = "model";
const std::string Monitor_name                                  = "monitor";
const std::string Move_name                                     = "move";
const std::string MoveSchedule_name                             = "move schedule";
const std::string MoveScale_name                                = "move(scale)";
const std::string MoveSlidingWindow_name                        = "move(sliding window)";
const std::string Move_mscale_name                              = "mscale move";
const std::string Move_mslide_name                              = "mslide move";
const std::string PosReal_name                                  = "posReal";
const std::string PosScalarRule_name                            = "posScalarRule";
const std::string RandomNumberGenerator_name                    = "random number generator";
const std::string RbBool_name                                   = "bool";
const std::string RbComplex_name                                = "complex";
const std::string RbDouble_name                                 = "real";
const std::string RbException_name                              = "exception";
const std::string RbFunction_name                               = "function";
const std::string RbFunction_dchisq_name                        = "chi-square density";
const std::string RbFunction_dbeta_name                         = "beta density";
const std::string RbFunction_dexp_name                          = "exponential density";
const std::string RbFunction_dgamma_name                        = "gamma density";
const std::string RbFunction_dnorm_name                         = "normal density";
const std::string RbFunction_ln_name                            = "log_e";
const std::string RbFunction_pow_name                           = "a^b";
const std::string RbFunction_pbeta_name                         = "beta cdf";
const std::string RbFunction_pchisq_name                        = "chi-square cdf";
const std::string RbFunction_pexp_name                          = "exponential cdf";
const std::string RbFunction_pnorm_name                         = "normal cdf";
const std::string RbFunction_qbeta_name                         = "beta quantile";
const std::string RbFunction_qchisq_name                        = "chi-square quantile";
const std::string RbFunction_qexp_name                          = "exponential quantile";
const std::string RbFunction_qnorm_name                         = "normal quantile";
const std::string RbFunction_readCharacterMatrix_name           = "read alignment";
const std::string RbFunction_readCharacterMatrix_fileType       = "file type";
const std::string RbFunction_readCharacterMatrix_alignmentType  = "alignment type";
const std::string RbFunction_readCharacterMatrix_fileName       = "file name";
const std::string RbInt_name                                    = "int";
const std::string RbInternal_name                               = "internal";
const std::string RbMcmc_name                                   = "mcmc";
const std::string RbMonitor_name                                = "monitor";
const std::string RbMove_name                                   = "move";
const std::string RbMoveSchedule_name                           = "move(schedule)";
const std::string RbNode_name                                   = "node";
const std::string RbNULL_name                                   = "NULL";
const std::string RbObject_name                                 = "object";
const std::string RbPrimitive_name                              = "primitive";
const std::string RbString_name                                 = "string";
const std::string RbTopology_name                               = "topology";
const std::string RbUndefined_name                              = "undefined";
const std::string RbUnrootedTopology_name                       = "unrooted topology";
const std::string RetValFunction_name                           = "return value function";
const std::string SimpleMove_name                               = "simple move";
const std::string Simplex_name                                  = "simplex";
const std::string SyntaxAssignExpr_name                         = "assignment";
const std::string SyntaxElement_name                            = "syntax element";
const std::string SyntaxFunctionCall_name                       = "function call";
const std::string SyntaxVariable_name                           = "variable";
const std::string StochasticNode_name                           = "stochastic node";
const std::string StringVector_name                             = "string vector";
const std::string UserFunction_name                             = "user function";
const std::string Vector_name                                   = "vector";
const std::string VariableNode_name                             = "variable node";
const std::string WrapperRule_name                              = "wrapper rule";

#endif
