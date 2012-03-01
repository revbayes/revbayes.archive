#import <Cocoa/Cocoa.h>
@class ToolConcatenate;



@interface WindowControllerConcatenate : NSWindowController {

	IBOutlet NSButton*            okButton;
	IBOutlet NSButton*            cancelButton;
    IBOutlet NSPopUpButton*       minOrMaxOverlapButton;
    IBOutlet NSPopUpButton*       seqMatchMethodButton;
    IBOutlet NSTextField*         numAlignments;
    IBOutlet NSTextField*         numSequences;
    ToolConcatenate*              myTool;
}

- (IBAction)cancelButtonAction:(id)sender;
- (id)initWithTool:(ToolConcatenate*)t;
- (IBAction)okButtonAction:(id)sender;

@end
