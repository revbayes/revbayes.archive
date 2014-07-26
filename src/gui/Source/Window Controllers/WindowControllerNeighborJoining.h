#import <Cocoa/Cocoa.h>
@class ToolNeighborJoining;



@interface WindowControllerNeighborJoining : NSWindowController {

    IBOutlet NSPopUpButton*        tieBreakerSelector;
    IBOutlet NSSegmentedControl*   bioNjSelector;
    
    ToolNeighborJoining*           myTool;
}

- (IBAction)cancelButtonAction:(id)sender;
- (id)initWithTool:(ToolNeighborJoining*)t;
- (IBAction)okButtonAction:(id)sender;

@end
