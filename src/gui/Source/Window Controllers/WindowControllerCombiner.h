#import <Cocoa/Cocoa.h>
@class RbData;
@class ToolCombiner;



@interface WindowControllerCombiner : NSWindowController {

    IBOutlet NSPopUpButton*  numberOfInletsPopUpButton;
	IBOutlet NSButton*       okButton;
	IBOutlet NSButton*       cancelButton;
	int                      numberOfInlets;
    ToolCombiner*            myTool;
}

@property (nonatomic) int    numberOfInlets;

- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolCombiner*)t;
- (IBAction)okButtonAction:(id)sender;

@end
