#import <Cocoa/Cocoa.h>
#import "iCarousel.h"
@class GuiTree;
@class ToolTreeSet;
@class TreeSetView;



@interface WindowControllerTreeViewer : NSWindowController  <iCarouselDataSource, iCarouselDelegate> {

    IBOutlet iCarousel*         carousel;
    ToolTreeSet*                myTool;
    NSMutableArray*             items;
    IBOutlet NSComboBox*        treeSelector;
    IBOutlet NSTextField*       treeSelectorLabel;
    IBOutlet NSNumberFormatter* treeSelectorFormatter;

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
    int                       selectedTree;
    float                     fontSize;
    BOOL                      drawMonophyleticWrOutgroup;
}

@property (nonatomic) int   selectedTree;
@property (nonatomic) float fontSize;
@property (nonatomic) BOOL  drawMonophyleticWrOutgroup;

@property (nonatomic,strong) IBOutlet iCarousel* carousel;
@property (nonatomic,assign) BOOL                wrap;
@property (nonatomic,strong) NSMutableArray*     items;

- (IBAction)closeButtonAction:(id)sender;
- (IBAction)showWindow:(id)sender;
- (IBAction)selectTree:(id)sender;

- (IBAction)changeFontSize:(id)sender;
- (IBAction)changedDrawMonophyleticTreeCheck:(id)sender;
- (IBAction)changeOutgroup:(id)sender;
- (IBAction)incrementFontSize:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (GuiTree*)activeTree;
- (id)initWithTool:(ToolTreeSet*)t;
- (void)initializeTreeInformation;
- (IBAction)leftTreeAction:(id)sender;
- (void)populateOutgroupList;
- (IBAction)rightTreeAction:(id)sender;

@end
