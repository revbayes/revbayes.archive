#import <Cocoa/Cocoa.h>
@class ToolAlign;



@interface WindowControllerAlign : NSWindowController {

    IBOutlet NSTabView*      alignmentMethodSelectorTab;         // Window variables
	IBOutlet NSButton*       okButton;
	IBOutlet NSButton*       cancelButton;
    ToolAlign*               myTool;
    
    int                      alignmentMethod;

    NSString*                clustalAlign;                       // CLUSTAL variables
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

    int                      probconsConsistency;                // Probcons variables
    int                      probconsIterativeRefinement;

    float                    tcoffeeGapOpenPenalty;              // T-Coffee variables
    float                    tcoffeeGapExtensionCost;
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

- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolAlign*)t;
- (IBAction)okButtonAction:(id)sender;
- (IBAction)cancelButtonAction:(id)sender;
- (void)resizeAlignmentWindow;

@end
