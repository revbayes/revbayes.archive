#import <Cocoa/Cocoa.h>
#include <vector>
@class RbData;
@class ToolDataEntry;



@interface WindowControllerDataEntry : NSWindowController {

	IBOutlet NSButton*                  okButton;
    IBOutlet NSPopUpButton*             changeDimensionsButton;

    ToolDataEntry*                      myTool;
}

- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolDataEntry*)t;
- (IBAction)okButtonAction:(id)sender;

@end
