#import <Cocoa/Cocoa.h>
#import "Tool.h"



@interface ToolSimulate : Tool <NSCoding> {

}

- (void)encodeWithCoder:(NSCoder *)aCoder;
- (void)execute;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (NSMutableAttributedString*)sendTip;

@end
