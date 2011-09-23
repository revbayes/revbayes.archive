#import <Cocoa/Cocoa.h>
@class ToolAlign;



@interface WindowControllerAlign : NSWindowController {

	IBOutlet NSButton*    okButton;
	IBOutlet NSButton*    cancelButton;
    ToolAlign*            myTool;

// Clustal variables
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

@property (readwrite, assign) NSString *alignClustalAlign;
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


- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolAlign*)t;
- (IBAction)okButtonAction:(id)sender;
- (IBAction)cancelButtonAction:(id)sender;

@end
