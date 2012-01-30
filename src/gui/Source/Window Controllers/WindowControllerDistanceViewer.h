#import <Cocoa/Cocoa.h>
@class ToolDistanceMatrix;




@interface WindowControllerDistanceViewer : NSWindowController {

    ToolDistanceMatrix*           myTool;
	IBOutlet NSButton*            okButton;
}

- (id)initWithTool:(ToolDistanceMatrix*)t;
- (IBAction)okButtonAction:(id)sender;

@end
