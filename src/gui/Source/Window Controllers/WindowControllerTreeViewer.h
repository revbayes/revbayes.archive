#import <Cocoa/Cocoa.h>
@class Tree;
@class ToolTreeSet;
@class TreeSetView;




@interface WindowControllerTreeViewer : NSWindowController {

    IBOutlet NSTextField*           treeCounter;
    IBOutlet NSStepper*             treeStepper;
    IBOutlet NSButton*              closeButton;
    IBOutlet TreeSetView*           treeView;
    IBOutlet NSTextField*           fontLabel;
    IBOutlet NSTextField*           fontEntry;
    ToolTreeSet*                    myTool;
    int                             selectedTree;
    float                           fontSize;
}

@property (readwrite) int selectedTree;
@property (readwrite) float fontSize;

- (IBAction)changeFontSize:(id)sender;
- (IBAction)closeButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (Tree*)activeTree;
- (id)initWithTool:(ToolTreeSet*)t;
- (void)initializeTreeInformation;
- (IBAction)stepAction:(id)sender;

@end
