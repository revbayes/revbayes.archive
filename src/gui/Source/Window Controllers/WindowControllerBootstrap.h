#import <Cocoa/Cocoa.h>
@class ToolBootstrap;



@interface WindowControllerBootstrap : NSWindowController {

    
    ToolBootstrap*                myTool;
}

- (IBAction)cancelButtonAction:(id)sender;
- (id)initWithTool:(ToolBootstrap*)t;
- (IBAction)okButtonAction:(id)sender;

@end
