#import <Foundation/Foundation.h>
#import "ParmRandomVariable.h"



@interface ParmVectorInteger : ParmRandomVariable <NSCoding> {
    
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
