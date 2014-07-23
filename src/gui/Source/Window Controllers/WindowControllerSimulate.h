#import <Cocoa/Cocoa.h>
@class GuiTree;
@class ToolSimulate;
@class SimulateView;



@interface WindowControllerSimulate : NSWindowController {

    IBOutlet NSButton*       okButton;
    IBOutlet NSButton*       cancelButton;
    IBOutlet NSButton*       editButton;
    IBOutlet NSButton*       addTaxonButton;
    IBOutlet NSTextField*    treeLengthLabel;
    IBOutlet NSTextField*    treeLengthField;
    IBOutlet SimulateView*   simulateView;
    IBOutlet NSTabView*      theTabView;
    NSRect                   lastTreeWindowSize;
    ToolSimulate*            myTool;
    GuiTree*                 myRemoteTree;
    float                    fontSize;
    BOOL                     editingTree;
    double                   treeLength;
    double                   alpha;
    double                   rAC;
    double                   rAG;
    double                   rAT;
    double                   rCG;
    double                   rCT;
    double                   rGT;
    double                   piA;
    double                   piC;
    double                   piG;
    double                   piT;
    double                   oldPiA;
    double                   oldPiC;
    double                   oldPiG;
    double                   oldPiT;
    int                      sequenceLength;
}

@property (nonatomic) BOOL   editingTree;
@property (nonatomic) float  fontSize;
@property (nonatomic) double treeLength;
@property (nonatomic) double alpha;
@property (nonatomic) double rAC;
@property (nonatomic) double rAG;
@property (nonatomic) double rAT;
@property (nonatomic) double rCG;
@property (nonatomic) double rCT;
@property (nonatomic) double rGT;
@property (nonatomic) double piA;
@property (nonatomic) double piC;
@property (nonatomic) double piG;
@property (nonatomic) double piT;
@property (nonatomic) int    sequenceLength;

- (IBAction)addTaxonAction:(id)sender;
- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)changeBaseFrequencies:(id)sender;
- (IBAction)editButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolSimulate*)t;
- (IBAction)okButtonAction:(id)sender;
- (GuiTree*)myTree;
- (void)setControlWindow;
- (void)setMyTree:(GuiTree*)t;
- (void)windowDidResize:(NSNotification*)notification;

@end
