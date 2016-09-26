#import <Cocoa/Cocoa.h>
@class ToolLoop;



@interface WindowControllerLoop : NSWindowController {

    ToolLoop*                   myTool;
    NSString*                   index;
    int                         upperLimit;
    IBOutlet NSPopUpButton*     possibleIndicesButton;
}

@property (nonatomic,strong) NSString* index;
@property (atomic          ) int       upperLimit;

- (IBAction)cancelButtonAction:(id)sender;
- (id)initWithTool:(ToolLoop*)t;
- (IBAction)okButtonAction:(id)sender;
- (void)setIndices;

@end
