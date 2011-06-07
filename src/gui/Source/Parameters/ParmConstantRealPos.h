#import <Foundation/Foundation.h>
#import "ParmConstant.h"



@interface ParmConstantRealPos : ParmConstant <NSCoding> {
    
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
