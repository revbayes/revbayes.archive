#import <Cocoa/Cocoa.h>
#import "ParmRandomVariable.h"
@class ValueInteger;



@interface ParmInteger : ParmRandomVariable <NSCoding> {

}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
