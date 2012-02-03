#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerNeighborJoining;




@interface ToolNeighborJoining : Tool <NSCoding> {

    WindowControllerNeighborJoining*   controlWindow;
}

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)showControlPanel;

@end
