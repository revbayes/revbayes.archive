#import <Cocoa/Cocoa.h>
@class ToolSimulate;
@class Tree;



@interface WindowControllerSimulate : NSWindowController {

    IBOutlet NSButton*              okButton;
    IBOutlet NSButton*              cancelButton;
    ToolSimulate*                   myTool;
    Tree*                           myTree;
}

- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolSimulate*)t;
- (IBAction)okButtonAction:(id)sender;

@end
