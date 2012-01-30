#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class WindowControllerAlign;
class CharacterData;



@interface ToolAlign : ToolData <NSCoding> {

    WindowControllerAlign*             controlWindow;                    // a pointer to the control window for this tool
    NSString*                          currentHelper;                    // lets Notification Center know which tools is being used
    
    NSTask*                            clustalTask;                      // clustal task
    NSPipe*                            clustalToPipe;
    NSPipe*                            clustalFromPipe;
    NSPipe*                            clustalErrorPipe;
    NSFileHandle*                      clustalToClustal;
    NSFileHandle*                      clustalFromClustal;
    NSFileHandle*                      clustalErrorData;
    
    NSString*                          clustalAlign;                     // Clustal variables received from window controller
    int                                clustalWordLength;
    int                                clustalWindow;
    NSString*                          clustalScoreType;
    int                                clustalNumberDiagonals;
    int                                clustalPairGapPenalty;
    NSString*                          clustalMatrix;
    float                              clustalGapOpenPenalty;
    NSString*                          clustalEndGaps;
    float                              clustalGapExtensionCost;
    int                                clustalGapSeparationPenalty;
    NSString*                          clustalIteration;
    int                                clustalNumberOfIterations;
    
    int32_t                            taskCount;
}

@property (readwrite,assign) NSString* clustalAlign;
@property (readwrite)        int       clustalWordLength;
@property (readwrite)        int       clustalWindow;
@property (readwrite,assign) NSString* clustalScoreType;
@property (readwrite)        int       clustalNumberDiagonals;
@property (readwrite)        int       clustalPairGapPenalty;
@property (readwrite,assign) NSString* clustalMatrix;
@property (readwrite)        float     clustalGapOpenPenalty;
@property (readwrite,assign) NSString* clustalEndGaps;
@property (readwrite)        float     clustalGapExtensionCost;
@property (readwrite)        int       clustalGapSeparationPenalty;
@property (readwrite,assign) NSString* clustalIteration;
@property (readwrite)        int       clustalNumberOfIterations;

- (void)closeControlPanel;
- (void)decrementTaskCount;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;
- (void)helperRunClustal:(id)sender;
- (void)receiveData:(NSNotification*)aNotification;
- (void)taskCompleted;

@end
