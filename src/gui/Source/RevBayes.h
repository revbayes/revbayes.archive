
#define VERSION_NUMBER                            1.0

#define ITEM_IMAGE_SIZE                          75.0
#define IN_OUT_SIZE                              15.0
#define BURY_FRACTION                             0.7

#undef SHOW_ONLY_BIOLOGY_1B_TOOLS
#define NUM_TOOLS                                14
#define TOOL_LOOP                                 0
#define TOOL_READDATA                             1
#define TOOL_DATAENTRY                            2
#define TOOL_ALIGN                                3
#define TOOL_COMBINER                             4
#define TOOL_CONCATENATE                          5
#define TOOL_MATRIXFILTER                         6
#define TOOL_BOOTSTRAP                            7
#define TOOL_MODEL                                8
#define TOOL_MCMC                                 9
#define TOOL_SIMULATE                            10
#define TOOL_PARSIMONY                           11
#define TOOL_TREECONTAINER                       12
#define TOOL_TREECONSENSUS                       13
#define TOOL_DISTANCEMATRIX                      14
#define TOOL_NEIGHBORJOINING                     15
#define TOOL_NUMBERSET                           16
#define TOOL_TREEDIAGNOSIS                       17
#define TOOL_NUMBERDIAGNOSIS                     18

#define DISTRIBUTION_CONSTANT                     0
#define DISTRIBUTION_FIXED                        0
#define DISTRIBUTION_EXPONENTIAL                  1
#define DISTRIBUTION_GAMMA                        2
#define DISTRIBUTION_LOGNORMAL                    3
#define DISTRIBUTION_CHISQUARE                    4
#define DISTRIBUTION_UNIFORM                      5
#define DISTRIBUTION_NORMAL                       6
#define DISTRIBUTION_STUDENTT                     7
#define DISTRIBUTION_CAUCHY                       8
#define DISTRIBUTION_LOGISTIC                     9
#define DISTRIBUTION_NEG                         10
#define DISTRIBUTION_SKEWNORMAL                  11
#define DISTRIBUTION_HYPERBOLIC                  12
#define DISTRIBUTION_POISSON                     13
#define DISTRIBUTION_DISCRETEUNIFORM             14
#define DISTRIBUTION_HYPERGEOMETRIC              15
#define DISTRIBUTION_BINOMIAL                    16
#define DISTRIBUTION_BETABINOMIAL                17
#define DISTRIBUTION_BIRTHDEATH                  18
#define DISTRIBUTION_COALESCENCE                 19
#define DISTRIBUTION_CTMC                        20
#define DISTRIBUTION_NONE                        21
#define DISTRIBUTION_UNIFORMPOS                  22
#define DISTRIBUTION_DISCRETEUNIFORMPOS          23
#define DISTRIBUTION_NEGATIVEBINOMIAL            24
#define DISTRIBUTION_UNIFORM_UNROOTED_TOPOLOGY   25
#define DISTRIBUTION_UNIFORM_ROOTED_TOPOLOGY     26
#define DISTRIBUTION_UNIFORM_HISTORIES           27
#define DISTRIBUTION_BIRTH_DEATH                 28
#define DISTRIBUTION_DIRICHLET                   29
#define DISTRIBUTION_MULTINOMIAL                 30

#define REAL_NUMBER                               1
#define POSITIVE_REAL_NUMBER                      2
#define INTEGER                                   4
#define POSITIVE_INTEGER                          8
#define SIMPLEX                                  16
#define VECTOR_POSITIVE_REAL_NUMBER              32
#define VECTOR_POSITIVE_INTEGER                  64
#define VECTOR_REAL_NUMBER                      128
#define VECTOR_INTEGER                          256

#define ROOTED_TREE                               0
#define UNROOTED_TREE                             1

#define PARM_REAL                                 0
#define PARM_REALPOS                              1
#define PARM_INTEGER                              2
#define PARM_INTEGERPOS                           3
#define PARM_TREE                                 4
#define PARM_SIMPLEX                              5
#define PARM_PLATE                                6
#define PARM_TREE_PLATE                           7
#define PARM_QMATRIX                              8
#define PARM_TOREALPOS                            9
#define PARM_VECTOR_INTEGERPOS                   10
#define PARM_VECTOR_REAL                         11
#define PARM_VECTOR_INTEGER                      12
#define PARM_VECTOR_REALPOS                      13
#define PARM_CONSTANT_REAL                       14
#define PARM_CONSTANT_REALPOS                    15
#define PARM_CONSTANT_INTEGER                    16
#define PARM_CONSTANT_INTEGERPOS                 17
#define PARM_CONSTANT_VECTOR_REAL                18

#define ITEM_SELECTION                            0
#define INLET_SELECTION                           1
#define OUTLET_SELECTION                          2
#define LINK_SELECTION                            3
#define INFO_SELECTION                            4
#define INSP_SELECTION                            5
#define TL_PLATE_RESIZE                           6
#define BR_PLATE_RESIZE                           7

typedef struct {

	id    selectedItem;
	int   selectionType;
    id    child;
    id    parent;
} ItemSelector;

