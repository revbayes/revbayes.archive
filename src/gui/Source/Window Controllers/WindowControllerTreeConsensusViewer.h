#import <Cocoa/Cocoa.h>
@class ConTreeView;
@class GuiTree;
@class ToolTreeConsensus;



@interface WindowControllerTreeConsensusViewer : NSWindowController {

    ToolTreeConsensus*              myTool;
    IBOutlet ConTreeView*           myView;
}

- (IBAction)closeAction:(id)sender;
- (GuiTree*)getConsensusTree;
- (id)initWithTool:(ToolTreeConsensus*)t;
- (void)update;

@end
