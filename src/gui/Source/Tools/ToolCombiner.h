#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class WindowControllerCombiner;



@interface ToolCombiner : ToolData <NSCoding> {

    WindowControllerCombiner*      controlWindow;
    int                            numberOfInlets;
    int                            startingNumberOfInlets;
}

@property (readwrite) int numberOfInlets;
@property (readwrite) int startingNumberOfInlets;

- (void)closeControlPanel;
- (void)setInletsAndOutlets;
- (void)showControlPanel;

@end
