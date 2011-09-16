#import <Cocoa/Cocoa.h>
@class ToolAlign;



@interface WindowControllerAlign : NSWindowController {

	IBOutlet NSButton*    okButton;
    ToolAlign*            myTool;
    
    // variables for CLUSTAL
    float                 gapPenalty;
    float                 wordLength;
    float                 windowVal;
    
	float                 testGapPenalty;
    IBOutlet NSButton*    martysButton;
}

@property (readwrite) float testGapPenalty;
@property (readwrite) float gapPenalty;
@property (readwrite) float wordLength;
@property (readwrite) float windowVal;

- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolAlign*)t;
- (IBAction)okButtonAction:(id)sender;

@end
