#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class WindowControllerConcatenate;



@interface ToolConcatenate : ToolData <NSCoding> {

    WindowControllerConcatenate*   controlWindow;
    BOOL                           useMinimalSet;
    BOOL                           matchUsingNames;
}

@property (readwrite) BOOL useMinimalSet;
@property (readwrite) BOOL matchUsingNames;

- (void)closeControlPanel;
- (int)numberOfIncomingAlignments;
- (int)mininumNumberOfIncomingSequences;
- (int)maximumNumberOfIncomingSequences;
- (void)showControlPanel;

@end
