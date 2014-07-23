#import <Cocoa/Cocoa.h>
@class GuiTree;
@class ToolTreeSet;
@class TreeSetView;




@interface WindowControllerTreeViewer : NSWindowController {

    IBOutlet NSButton*        leftTree;
    IBOutlet NSButton*        rightTree;
    IBOutlet NSTextField*     treeStepLabel;
    IBOutlet NSTextField*     treeCounter;
    IBOutlet NSButton*        closeButton;
    IBOutlet TreeSetView*     treeView;
    IBOutlet NSTextField*     fontLabel;
    IBOutlet NSTextField*     fontEntry;
    IBOutlet NSTextField*     treeInfo;
    IBOutlet NSPopUpButton*   outgroupList;
    IBOutlet NSTextField*     outgroupLabel;
    IBOutlet NSStepper*       fontStepper;
    IBOutlet NSButton*        drawMonophyleticTreeCheck;
    ToolTreeSet*              myTool;
    int                       selectedTree;
    float                     fontSize;
    BOOL                      drawMonophyleticWrOutgroup;
}

@property (nonatomic) int   selectedTree;
@property (nonatomic) float fontSize;
@property (nonatomic) BOOL  drawMonophyleticWrOutgroup;

- (IBAction)changeFontSize:(id)sender;
- (IBAction)changedDrawMonophyleticTreeCheck:(id)sender;
- (IBAction)changeOutgroup:(id)sender;
- (IBAction)closeButtonAction:(id)sender;
- (IBAction)incrementFontSize:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (GuiTree*)activeTree;
- (id)initWithTool:(ToolTreeSet*)t;
- (void)initializeTreeInformation;
- (IBAction)leftTreeAction:(id)sender;
- (void)populateOutgroupList;
- (IBAction)rightTreeAction:(id)sender;

@end
