#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class RbData;
@class WindowControllerDataEntry;




@interface ToolDataEntry : ToolData <NSCoding> {

    WindowControllerDataEntry*     controlWindow;
}

- (void)closeControlPanel;
- (RbData*)dataMatrix;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (void)initializeDataMatrix:(RbData*)dm;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end

