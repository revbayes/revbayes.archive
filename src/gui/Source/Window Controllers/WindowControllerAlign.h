#import <Cocoa/Cocoa.h>
@class ToolAlign;



@interface WindowControllerAlign : NSWindowController {

    IBOutlet NSTabView*     alignmentMethodSelectorTab;         // Window variables
	IBOutlet NSButton*      okButton;
	IBOutlet NSButton*      cancelButton;
    ToolAlign*              myTool;
    
    int                     alignmentMethod;

    NSString*               clustalAlign;                   // CLUSTAL variables
    int                     clustalWordLength;
    int                     clustalWindow;
    NSString*               clustalScoreType;
    int                     clustalNumberDiagonals;
    int                     clustalPairGapPenalty;
    NSString*               clustalMatrix;
    float                   clustalGapOpenPenalty;
    NSString*               clustalEndGaps;
    float                   clustalGapExtensionCost;
    int                     clustalGapSeparationPenalty;
    NSString*               clustalIteration;
    int                     clustalNumberOfIterations;
}

@property (readwrite)        int       alignmentMethod;
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

- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolAlign*)t;
- (IBAction)okButtonAction:(id)sender;
- (IBAction)cancelButtonAction:(id)sender;

@end
