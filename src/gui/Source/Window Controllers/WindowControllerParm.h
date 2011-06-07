#import <Cocoa/Cocoa.h>



@interface WindowControllerParm : NSWindowController {

	IBOutlet NSButton*       cancelButton;
	IBOutlet NSButton*       helpButton;
	IBOutlet NSButton*       okButton;
    id                       myParm;
}

- (IBAction)cancelAction:(id)sender;
- (IBAction)helpAction:(id)sender;
- (IBAction)okAction:(id)sender;
- (void)setWindowVariables;

@end
