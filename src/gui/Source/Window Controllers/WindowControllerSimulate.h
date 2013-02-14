#import <Cocoa/Cocoa.h>
@class GuiTree;
@class ToolSimulate;
@class SimulateView;



@interface WindowControllerSimulate : NSWindowController {

    IBOutlet NSButton*              okButton;
    IBOutlet NSButton*              cancelButton;
    IBOutlet NSButton*              editButton;
    IBOutlet NSButton*              addTaxonButton;
    IBOutlet NSTextField*           treeLengthLabel;
    IBOutlet NSTextField*           treeLengthField;
    IBOutlet SimulateView*          simulateView;
    IBOutlet NSTabView*             theTabView;
    NSRect                          lastTreeWindowSize;
    ToolSimulate*                   myTool;
    GuiTree*                        myRemoteTree;
    float                           fontSize;
    BOOL                            editingTree;
    double                          treeLength;
    double                          alpha;
    double                          rAC;
    double                          rAG;
    double                          rAT;
    double                          rCG;
    double                          rCT;
    double                          rGT;
    double                          piA;
    double                          piC;
    double                          piG;
    double                          piT;
    double                          oldPiA;
    double                          oldPiC;
    double                          oldPiG;
    double                          oldPiT;
    int                             sequenceLength;
}

@property (readwrite) BOOL   editingTree;
@property (readwrite) float  fontSize;
@property (readwrite) double treeLength;
@property (readwrite) double alpha;
@property (readwrite) double rAC;
@property (readwrite) double rAG;
@property (readwrite) double rAT;
@property (readwrite) double rCG;
@property (readwrite) double rCT;
@property (readwrite) double rGT;
@property (readwrite) double piA;
@property (readwrite) double piC;
@property (readwrite) double piG;
@property (readwrite) double piT;
@property (readwrite) int    sequenceLength;

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
