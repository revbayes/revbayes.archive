#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class WindowControllerCombiner;



@interface ToolCombiner : ToolData <NSCoding> {

    WindowControllerCombiner*      controlWindow;
    int                            numberOfInlets;
    int                            startingNumberOfInlets;
}

@property (nonatomic) int          numberOfInlets;
@property (nonatomic) int          startingNumberOfInlets;

- (void)closeControlPanel;
- (void)setInletsAndOutlets;
- (void)showControlPanel;

@end
