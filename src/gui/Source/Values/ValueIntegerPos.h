#import <Cocoa/Cocoa.h>
#import "Value.h"



@interface ValueIntegerPos : Value <NSCoding> {

    NSNumber*            value;
}

- (id)initWithNumber:(int)v;
- (void)setValue:(int)x;
- (int)value;

@end
