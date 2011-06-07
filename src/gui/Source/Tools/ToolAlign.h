#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerAlign;



@interface ToolAlign : Tool <NSCoding> {

    WindowControllerAlign*  controlWindow;
}

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)setInletsAndOutlets;
- (void)showControlPanel;

@end
