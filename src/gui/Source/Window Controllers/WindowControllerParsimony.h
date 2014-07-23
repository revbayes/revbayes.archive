#import <Cocoa/Cocoa.h>
@class ToolParsimony;



@interface WindowControllerParsimony : NSWindowController {

    IBOutlet NSPopUpButton*        tieBreakerSelector;
    IBOutlet NSSegmentedControl*   bioNjSelector;
    ToolParsimony*                 myTool;
}

- (IBAction)cancelButtonAction:(id)sender;
- (id)initWithTool:(ToolParsimony*)t;
- (IBAction)okButtonAction:(id)sender;

@end
