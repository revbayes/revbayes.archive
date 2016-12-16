#import <Cocoa/Cocoa.h>
@class ConTreeView;
@class GuiTree;
@class ToolTreeConsensus;



@interface WindowControllerTreeConsensusViewer : NSWindowController {

    ToolTreeConsensus*          myTool;
    IBOutlet ConTreeView*       myView;
    NSMenuItem*                 treeMenu;
    NSMutableArray*             outgroupMenuItems;
    BOOL                        drawMonophyleticWrOutgroup;
}

- (IBAction)closeAction:(id)sender;
- (GuiTree*)getConsensusTree;
- (void)initializeTreeMenu;
- (id)initWithTool:(ToolTreeConsensus*)t;
- (void)update;

@end
