#import <Cocoa/Cocoa.h>
#import "ToolData.h"
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
    NSPipe*                  clustalToPipe;
    NSPipe*                  clustalFromPipe;
    NSPipe*                  clustalErrorPipe;
    NSFileHandle*            clustalToClustal;
    NSFileHandle*            clustalFromClustal;
    NSFileHandle*            clustalErrorData;
    
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
    
    int32_t                  taskCount;
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

- (void)alignSequences;
- (void)closeControlPanel;
- (void)decrementTaskCount;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;
- (BOOL)helperRunClustal:(id)sender;
- (void)receiveData:(NSNotification*)aNotification;
- (void)taskCompleted;

@end
