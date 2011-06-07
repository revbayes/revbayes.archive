#import <Foundation/Foundation.h>
#import "ParmRandomVariable.h"



@interface ParmVectorReal : ParmRandomVariable <NSCoding> {
    
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
