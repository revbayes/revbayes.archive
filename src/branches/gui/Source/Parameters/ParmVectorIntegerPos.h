#import <Foundation/Foundation.h>
#import "ParmRandomVariable.h"



@interface ParmVectorIntegerPos : ParmRandomVariable <NSCoding> {
    
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
