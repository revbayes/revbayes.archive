#import <Cocoa/Cocoa.h>
#import "iCarousel.h"
@class GuiTree;
@class ToolTreeSet;
@class WindowControllerTreePeek;



@interface WindowControllerTreeViewer : NSWindowController  <iCarouselDataSource, iCarouselDelegate> {

    IBOutlet iCarousel*         carousel;
    ToolTreeSet*                myTool;
    NSMutableArray*             items;
    IBOutlet NSComboBox*        treeSelector;
    IBOutlet NSTextField*       treeSelectorLabel;
    IBOutlet NSNumberFormatter* treeSelectorFormatter;
    IBOutlet NSButton*          closeButton;
    IBOutlet NSMenuItem*        treeDrawingMenuItem;
    IBOutlet NSMenuItem*        outgroupSelectorMenu;
    BOOL                        drawMonophyleticWrOutgroup;
    NSMutableArray*             outgroupMenuItems;
    WindowControllerTreePeek*   treePeeker;

    IBOutlet NSTextField*     fontLabel;
    IBOutlet NSTextField*     fontEntry;
    IBOutlet NSTextField*     treeInfo;
    IBOutlet NSStepper*       fontStepper;
    int                       selectedTree;
    float                     fontSize;
}

@property (nonatomic) int   selectedTree;
@property (nonatomic) float fontSize;
@property (nonatomic) BOOL  drawMonophyleticWrOutgroup;

@property (nonatomic,strong) IBOutlet iCarousel* carousel;
@property (nonatomic,assign) BOOL                wrap;
@property (nonatomic,strong) NSMutableArray*     items;

- (IBAction)closeButtonAction:(id)sender;
- (GuiTree*)currentTree;
- (IBAction)showWindow:(id)sender;
- (IBAction)selectTree:(id)sender;
- (void)showTreePeeker;

- (IBAction)changeFontSize:(id)sender;
- (IBAction)changedDrawMonophyleticTree:(id)sender;
- (IBAction)changeOutgroup:(id)sender;
- (IBAction)incrementFontSize:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (GuiTree*)activeTree;
- (id)initWithTool:(ToolTreeSet*)t;

@end
