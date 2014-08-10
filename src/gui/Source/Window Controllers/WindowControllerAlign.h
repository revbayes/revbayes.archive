#import <Cocoa/Cocoa.h>
@class ToolAlign;



@interface WindowControllerAlign : NSWindowController {

    IBOutlet NSTabView*   alignmentMethodSelectorTab;         // Window variables
	IBOutlet NSButton*    okButton;
	IBOutlet NSButton*    cancelButton;
    ToolAlign*            myTool;
    
    int                   alignmentMethod;

    NSString*             clustalAlign;                   // CLUSTAL variables
    int                   clustalWordLength;
    int                   clustalWindow;
    NSString*             clustalScoreType;
    int                   clustalNumberDiagonals;
    int                   clustalPairGapPenalty;
    NSString*             clustalMatrix;
    float                 clustalGapOpenPenalty;
    NSString*             clustalEndGaps;
    float                 clustalGapExtensionCost;
    int                   clustalGapSeparationPenalty;
    NSString*             clustalIteration;
    int                   clustalNumberOfIterations;
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

- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolAlign*)t;
- (IBAction)okButtonAction:(id)sender;
- (IBAction)cancelButtonAction:(id)sender;

@end
