#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class ToolReadData;
@class WindowControllerAlign;
class CharacterData;

#define ALN_CLUSTAL     0
#define ALN_MAFFT       1
#define ALN_DIALIGN     2
#define ALN_MUSCLE      3
#define ALN_TCOFFEE     4
#define ALN_DCA         5
#define ALN_PROBCONS    6




@interface ToolAlign : ToolData <NSCoding> {

    WindowControllerAlign*   controlWindow;                    // a pointer to the control window for this tool
    NSString*                currentHelper;                    // lets Notification Center know which tools is being used
    int                      alignmentMethod;
    
    NSTask*                  clustalTask;                      // clustal task
    NSFileHandle*            clustalFromClustal;
    
    NSString*                clustalAlign;                     // Clustal variables received from window controller
    int                      clustalWordLength;
    int                      clustalWindow;
    NSString*                clustalScoreType;
    int                      clustalNumberDiagonals;
    int                      clustalPairGapPenalty;
    NSString*                clustalMatrix;
    float                    clustalGapOpenPenalty;
    NSString*                clustalEndGaps;
    float                    clustalGapExtensionCost;
    int                      clustalGapSeparationPenalty;
    NSString*                clustalIteration;
    int                      clustalNumberOfIterations;
    
    int                      muscleAnchorSpacing;              // Muscle variables received from window controller
    float                    muscleCenter;
    NSString*                muscleCluster1;
    NSString*                muscleCluster2;
    int                      muscleDiagLength;
    int                      muscleDiagMargin;
    NSString*                muscleDistance1;
    NSString*                muscleDistance2;
    float                    muscleGapOpen;
    int                      muscleHydro;
    float                    muscleHydroFactor;
    int                      muscleMaxDiagBreak;
    int                      muscleMaxIters;
    int                      muscleMaxTrees;
    float                    muscleMinBestColScore;
    float                    muscleMinSmoothScore;
    NSString*                muscleObjScore;
    NSString*                muscleRoot1;
    NSString*                muscleRoot2;
    float                    muscleSmoothScoreCeil;
    int                      muscleSmoothWindow;
    float                    muscleSUEFF;
    NSString*                muscleWeight1;
    NSString*                muscleWeight2;
    
    int                      probconsConsistency;                // Probcons variables received from window controller
    int                      probconsIterativeRefinement;

    float                    tcoffeeGapOpenPenalty;              // T-Coffee variables received from window controller
    float                    tcoffeeGapExtensionCost;
    
    int32_t                  taskCount;
    int32_t                  numberErrors;
}

@property (nonatomic)        int       alignmentMethod;
@property (nonatomic,strong) NSString* clustalAlign;
@property (nonatomic)        int       clustalWordLength;
@property (nonatomic)        int       clustalWindow;
@property (nonatomic,strong) NSString* clustalScoreType;
@property (nonatomic)        int       clustalNumberDiagonals;
@property (nonatomic)        int       clustalPairGapPenalty;
@property (nonatomic,strong) NSString* clustalMatrix;
@property (nonatomic)        float     clustalGapOpenPenalty;
@property (nonatomic,strong) NSString* clustalEndGaps;
@property (nonatomic)        float     clustalGapExtensionCost;
@property (nonatomic)        int       clustalGapSeparationPenalty;
@property (nonatomic,strong) NSString* clustalIteration;
@property (nonatomic)        int       clustalNumberOfIterations;
@property (nonatomic)        int       muscleAnchorSpacing;
@property (nonatomic)        float     muscleCenter;
@property (nonatomic,strong) NSString* muscleCluster1;
@property (nonatomic,strong) NSString* muscleCluster2;
@property (nonatomic)        int       muscleDiagLength;
@property (nonatomic)        int       muscleDiagMargin;
@property (nonatomic,strong) NSString* muscleDistance1;
@property (nonatomic,strong) NSString* muscleDistance2;
@property (nonatomic)        float     muscleGapOpen;
@property (nonatomic)        int       muscleHydro;
@property (nonatomic)        float     muscleHydroFactor;
@property (nonatomic)        int       muscleMaxDiagBreak;
@property (nonatomic)        int       muscleMaxIters;
@property (nonatomic)        int       muscleMaxTrees;
@property (nonatomic)        float     muscleMinBestColScore;
@property (nonatomic)        float     muscleMinSmoothScore;
@property (nonatomic,strong) NSString* muscleObjScore;
@property (nonatomic,strong) NSString* muscleRoot1;
@property (nonatomic,strong) NSString* muscleRoot2;
@property (nonatomic)        float     muscleSmoothScoreCeil;
@property (nonatomic)        int       muscleSmoothWindow;
@property (nonatomic)        float     muscleSUEFF;
@property (nonatomic,strong) NSString* muscleWeight1;
@property (nonatomic,strong) NSString* muscleWeight2;
@property (nonatomic)        int       probconsConsistency;
@property (nonatomic)        int       probconsIterativeRefinement;
@property (nonatomic)        float     tcoffeeGapOpenPenalty;
@property (nonatomic)        float     tcoffeeGapExtensionCost;

- (void)alignmentFinished:(NSString*)alnDirectory;
- (void)alignSequences;
- (void)closeControlPanelWithOK;
- (void)closeControlPanelWithCancel;
- (void)decrementTaskCount;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (ToolReadData*)findDataParent;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)incrementErrorCount;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;
- (BOOL)helperRunClustal:(id)sender;
- (BOOL)helperRunMuscle:(id)sender;
- (BOOL)helperRunProbcons:(id)sender;
- (BOOL)helperRunTcoffee:(id)sender;
- (BOOL)readAlignmentsInTemporaryFolder:(NSString*)alnDirectory;
- (void)taskCompleted;

@end
