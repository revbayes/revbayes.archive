#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerAlign;



@interface ToolAlign : Tool <NSCoding> {

    WindowControllerAlign*  controlWindow;
    
// Clustal task
    NSTask			*alignClustalTask;
    NSPipe			*alignClustalToPipe;
    NSPipe			*alignClustalFromPipe;
    NSPipe			*alignClustalErrorPipe;
    NSFileHandle	*alignClustalToClustal;
    NSFileHandle	*alignClustalFromClustal;
    NSFileHandle	*alignClustalErrorData;
    
// Clustal variables set in tool
    NSString *alignClustalReduceConsoleOutputAr;
    NSString *alignClustalMultipleAlignAr;
    NSString *alignClustalInfileAr;
    NSString *alignClustalOutfileAr;
    NSString *alignClustalGuideTreeAr;
    NSMutableArray *alignClustalArguments;

// Clustal variables received from window controller
    NSString *alignClustalAlign;
    int alignClustalWordLength;
    int alignClustalWindow;
    NSString *alignClustalScoreType;
    int alignClustalNumberDiagonals;
    int alignClustalPairGapPenalty;
    NSString *alignClustalMatrix;
    float alignClustalGapOpenPenalty;
    NSString *alignClustalEndGaps;
    float alignClustalGapExtensionCost;
    int alignClustalGapSeparationPenalty;
    NSString *alignClustalIteration;
    int alignClustalNumberOfIterations;
    

}

@property (readwrite,assign) NSString *alignClustalAlign;
@property (readwrite) int alignClustalWordLength;
@property (readwrite) int alignClustalWindow;
@property (readwrite,assign) NSString *alignClustalScoreType;
@property (readwrite) int alignClustalNumberDiagonals;
@property (readwrite) int alignClustalPairGapPenalty;
@property (readwrite,assign) NSString *alignClustalMatrix;
@property (readwrite) float alignClustalGapOpenPenalty;
@property (readwrite, assign) NSString *alignClustalEndGaps;
@property (readwrite) float alignClustalGapExtensionCost;
@property (readwrite) int alignClustalGapSeparationPenalty;
@property (readwrite,assign) NSString *alignClustalIteration;
@property (readwrite) int alignClustalNumberOfIterations;

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)setInletsAndOutlets;
- (void)showControlPanel;

- (void)helperRunClustal;

@end
