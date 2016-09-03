#import <Cocoa/Cocoa.h>
#import "Tool.h"
#include <vector>
@class GuiTree;
@class ToolTreeSet;
@class WindowControllerParsimony;




@interface ToolParsimony : Tool <NSCoding> {

    WindowControllerParsimony*          controlWindow;
    int                                 numberErrors;
    NSMutableArray*                     treeSetContainers;
    int                                 treeLength;

    int                                 searchMethod;
    NSString*                           hsSwap;
    NSString*                           hsKeep;
    NSString*                           hsMulTrees;
    NSString*                           hsRearrLimit;
    NSString*                           hsReconLimit;
    NSString*                           hsNBest;
    NSString*                           hsRetain;
    NSString*                           hsAllSwap;
    NSString*                           hsUseNonMin;
    NSString*                           hsSteepest;
    int                                 hsNChuck;
    NSString*                           hsChuckScore;
    NSString*                           hsAbortRep;
    NSString*                           hsRandomize;
    NSString*                           hsAddSeq;
    int                                 hsNReps;
    NSString*                           hsHold;
    NSString*                           bbKeep;
    NSString*                           bbMulTrees;
    double                              bbUpBound;
    NSString*                           bbAddSeq;
    NSString*                           exKeep;
}

@property (nonatomic)        int        searchMethod;
@property (nonatomic,strong) NSString*  hsSwap;
@property (nonatomic,strong) NSString*  hsKeep;
@property (nonatomic,strong) NSString*  hsMulTrees;
@property (nonatomic,strong) NSString*  hsRearrLimit;
@property (nonatomic,strong) NSString*  hsReconLimit;
@property (nonatomic,strong) NSString*  hsNBest;
@property (nonatomic,strong) NSString*  hsRetain;
@property (nonatomic,strong) NSString*  hsAllSwap;
@property (nonatomic,strong) NSString*  hsUseNonMin;
@property (nonatomic,strong) NSString*  hsSteepest;
@property (nonatomic)        int        hsNChuck;
@property (nonatomic,strong) NSString*  hsChuckScore;
@property (nonatomic,strong) NSString*  hsAbortRep;
@property (nonatomic,strong) NSString*  hsRandomize;
@property (nonatomic,strong) NSString*  hsAddSeq;
@property (nonatomic)        int        hsNReps;
@property (nonatomic,strong) NSString*  hsHold;
@property (nonatomic,strong) NSString*  bbKeep;
@property (nonatomic,strong) NSString*  bbMulTrees;
@property (nonatomic)        double     bbUpBound;
@property (nonatomic,strong) NSString*  bbAddSeq;
@property (nonatomic,strong) NSString*  exKeep;

- (void)closeControlPanelWithCancel;
- (void)closeControlPanelWithOK;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (BOOL)paupSearch;
- (BOOL)sendTree:(GuiTree*)t toTreeSet:(ToolTreeSet*)treeSet;
- (void)showControlPanel;

@end
