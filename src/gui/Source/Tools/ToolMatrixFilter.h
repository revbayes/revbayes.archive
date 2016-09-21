#import <Cocoa/Cocoa.h>
#import "Outlet.h"
#import "ToolData.h"
@class WindowControllerMatrixFilter;



@interface ToolMatrixFilter : ToolData <NSCoding> {

    WindowControllerMatrixFilter*      controlWindow;
}

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (id)initWithScaleFactor:(float)sf;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end
