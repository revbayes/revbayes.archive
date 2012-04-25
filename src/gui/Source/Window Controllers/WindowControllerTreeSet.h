#import <Cocoa/Cocoa.h>
@class Tree;
@class ToolTreeSet;



@interface WindowControllerTreeSet : NSWindowController {

    IBOutlet NSTextField*           treeCounter;
    IBOutlet NSStepper*             treeStepper;
    IBOutlet NSButton*              closeButton;
    ToolTreeSet*                    myTool;
    int                             selectedTree;
}

@property (readwrite) int selectedTree;

- (IBAction)closeButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (Tree*)activeTree;
- (id)initWithTool:(ToolTreeSet*)t;
- (IBAction)stepAction:(id)sender;

@end
