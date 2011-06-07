#import <Cocoa/Cocoa.h>
@class ToolAlign;



@interface WindowControllerAlign : NSWindowController {

	IBOutlet NSButton*    okButton;
    ToolAlign*            myTool;
	float                 testGapPenalty;
    IBOutlet NSButton*    martysButton;
}

@property (readwrite) float testGapPenalty;

- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolAlign*)t;
- (IBAction)okButtonAction:(id)sender;

@end
