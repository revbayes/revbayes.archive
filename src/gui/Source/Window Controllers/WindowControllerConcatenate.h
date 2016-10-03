#import <Cocoa/Cocoa.h>
@class ToolConcatenate;



@interface WindowControllerConcatenate : NSWindowController {

	IBOutlet NSButton*              okButton;
	IBOutlet NSButton*              cancelButton;
    IBOutlet NSPopUpButton*         matchMethodButton;
    IBOutlet NSPopUpButton*         mergeMethodButton;
    IBOutlet NSTextField*           numAlignments;
    IBOutlet NSTextField*           numSequences;
    
    ToolConcatenate*                myTool;
    int                             matchMethod;
    int                             mergeMethod;
}

@property (nonatomic) int           matchMethod;
@property (nonatomic) int           mergeMethod;

- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)changeMatchMethod:(id)sender;
- (IBAction)changeMergeMethod:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolConcatenate*)t;
- (IBAction)okButtonAction:(id)sender;

@end
