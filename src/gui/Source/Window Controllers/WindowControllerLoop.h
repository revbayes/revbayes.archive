#import <Cocoa/Cocoa.h>
@class ToolLoop;



@interface WindowControllerLoop : NSWindowController {

    
    ToolLoop*                myTool;
}

- (IBAction)cancelButtonAction:(id)sender;
- (id)initWithTool:(ToolLoop*)t;
- (IBAction)okButtonAction:(id)sender;

@end
