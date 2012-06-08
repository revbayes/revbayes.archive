#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerSimulate;



@interface ToolSimulate : Tool <NSCoding> {

    WindowControllerSimulate*          controlWindow;
}

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (void)execute;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;

@end
