#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerParsimony;




@interface ToolParsimony : Tool <NSCoding> {

    WindowControllerParsimony*   controlWindow;
}

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)showControlPanel;

@end
