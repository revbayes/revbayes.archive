#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class RbData;
@class WindowControllerDataEntry;




@interface ToolDataEntry : Tool <NSCoding> {

    RbData*                        dataMatrix;
    WindowControllerDataEntry*     controlWindow;
}

- (void)closeControlPanel;
- (RbData*)dataMatrix;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)initializeDataMatrix;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end

