#import <Cocoa/Cocoa.h>
@class ToolAlign;



@interface WindowControllerAlign : NSWindowController {

	IBOutlet NSButton*    okButton;
    ToolAlign*            myTool;
    
    // variables for CLUSTAL
    NSTask			*alignClustalTask;
    NSPipe			*alignClustalToPipe;
    NSPipe			*alignClustalFromPipe;
    NSPipe			*alignClustalErrorPipe;
    NSFileHandle	*alignClustalToClustal;
    NSFileHandle	*alignClustalFromClustal;
    NSFileHandle	*alignClustalErrorData;
}


- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolAlign*)t;
- (IBAction)okButtonAction:(id)sender;

@end
