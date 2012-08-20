#import "ValueReal.h"
#import "ValueVectorReal.h"



@implementation ValueVectorReal

- (Value*)clone {
    
    return [[ValueVectorReal alloc] initWithValue:self];
}

- (void)dealloc {
    
    // dealloc here
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
    [super encodeWithCoder:aCoder];
}

- (id)init {
    
    if ( (self = [super init]) ) 
		{
        for (int i=0; i<[vector count]; i++)
            {
            ValueReal* v = [[ValueReal alloc] initWithNumber:0.0];
            [vector addObject:v];
            }
		}
	return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		}
	return self;
}

- (id)initWithDimensions:(int)dim {
    
    if ( (self = [super init]) ) 
		{
        [vector removeAllObjects];
        for (int i=0; i<dim; i++)
            {
            ValueReal* v = [[ValueReal alloc] initWithNumber:0.0];
            [vector addObject:v];
            }
		}
	return self;
}

- (id)initWithValue:(Value*)v {
    
    if ( (self = [super init]) ) 
		{
        if ( [[v className] isEqualToString:@"ValueVectorReal"] == NO )
            return nil;
        [vector removeAllObjects];
        for (int i=0; i<[(ValueVector*)v dimensions]; i++)
            {
            Value* oldV = [(ValueVector*)v valueIndexed:i];
            Value* newV = [oldV clone];
            [vector addObject:newV];
            }
		}
	return self;
}

- (Value*)newValue {
    
    ValueReal* v = [[ValueReal alloc] initWithNumber:0.0];
    return v;
}

- (NSString*)stringRepresentation {
    
    NSString* strRep = @"(";
    for (int i=0; i<[self dimensions]; i++)
        {
        Value* v = [self valueIndexed:i];
        NSString* numStr = [v stringRepresentation];
        strRep = [strRep stringByAppendingString:numStr];
        if (i+1 < [self dimensions])
            strRep = [strRep stringByAppendingString:@","];
        }
    strRep = [strRep stringByAppendingString:@")"];
    return strRep;
}

@end
