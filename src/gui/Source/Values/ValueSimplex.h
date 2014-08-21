#import <Cocoa/Cocoa.h>
#import "Value.h"



@interface ValueSimplex : Value <NSCoding> {
    
    NSArray*   value;
}

@property (nonatomic,strong) NSArray* value;

- (id)initWithVector:(NSArray*)v;

@end
