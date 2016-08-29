#import <Cocoa/Cocoa.h>
#import "Tool.h"


@interface ToolNumericalMcmcOutput : Tool <NSCoding> {
    
}

- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;

@end