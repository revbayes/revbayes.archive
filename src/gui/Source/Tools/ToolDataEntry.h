#import <Foundation/Foundation.h>
#import "ToolData.h"
@class RbData;
@class WindowControllerDataEntry;




@interface ToolDataEntry : ToolData <NSCoding> {

    WindowControllerDataEntry*         controlWindow;
}

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end
