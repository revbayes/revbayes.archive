#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class WindowControllerBootstrap;



@interface ToolBootstrap : ToolData <NSCoding> {

    WindowControllerBootstrap*      controlWindow;
}

- (void)closeControlPanel;
- (void)showControlPanel;

@end
