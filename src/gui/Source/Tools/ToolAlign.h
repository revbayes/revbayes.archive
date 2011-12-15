#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerAlign;



@interface ToolAlign : Tool <NSCoding> {

    WindowControllerAlign*      controlWindow;                         // a pointer to the control window for this tool
    NSString*                   currentHelper;                         // lets Notification Center know which tools is being used
    
    NSTask*                     alignClustalTask;                      // clustal task
    NSPipe*                     alignClustalToPipe;
    NSPipe*                     alignClustalFromPipe;
    NSPipe*                     alignClustalErrorPipe;
    NSFileHandle*               alignClustalToClustal;
    NSFileHandle*               alignClustalFromClustal;
    NSFileHandle*               alignClustalErrorData;
    
    NSString*                   alignClustalAlign;                    // Clustal variables received from window controller
    int                         alignClustalWordLength;
    int                         alignClustalWindow;
    NSString*                   alignClustalScoreType;
    int                         alignClustalNumberDiagonals;
    int                         alignClustalPairGapPenalty;
    NSString*                   alignClustalMatrix;
    float                       alignClustalGapOpenPenalty;
    NSString*                   alignClustalEndGaps;
    float                       alignClustalGapExtensionCost;
    int                         alignClustalGapSeparationPenalty;
    NSString*                   alignClustalIteration;
    int                         alignClustalNumberOfIterations;
}

@property (readwrite, assign) NSString* alignClustalAlign;
@property (readwrite) int alignClustalWordLength;
@property (readwrite) int alignClustalWindow;
@property (readwrite,assign) NSString* alignClustalScoreType;
@property (readwrite) int alignClustalNumberDiagonals;
@property (readwrite) int alignClustalPairGapPenalty;
@property (readwrite,assign) NSString* alignClustalMatrix;
@property (readwrite) float alignClustalGapOpenPenalty;
@property (readwrite, assign) NSString* alignClustalEndGaps;
@property (readwrite) float alignClustalGapExtensionCost;
@property (readwrite) int alignClustalGapSeparationPenalty;
@property (readwrite,assign) NSString* alignClustalIteration;
@property (readwrite) int alignClustalNumberOfIterations;

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)setInletsAndOutlets;
- (void)showControlPanel;
- (void)helperRunClustal:(id)sender;
- (void)receiveData:(NSNotification*)aNotification;
- (void)taskCompleted;

@end
