#import "RbDataCell.h"
#import "RbTaxonData.h"



@implementation RbTaxonData

@synthesize dataType;
@synthesize taxonName;

- (void)addObservation:(RbDataCell*)dc {

    [data addObject:dc];
}

- (RbDataCell*)dataCellIndexed:(int)idx {

	return [data objectAtIndex:idx];
}

- (void)dealloc {

    NSLog(@"delete %@", self);
	[data release];
	[taxonName release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodeInt:dataType      forKey:@"dataType"];
	[aCoder encodeObject:data       forKey:@"data"];
	[aCoder encodeObject:taxonName  forKey:@"taxonName"];
}

- (id)init {

    if ( (self = [super init]) ) 
        {
		// allocate an array holding the data
		data       = [[NSMutableArray alloc] init];
		taxonName  = [[NSString alloc] init];
		
        // initialize some variables
		dataType           = 0;
        }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        dataType            = [aDecoder decodeIntForKey:@"dataType"];
		data                = [aDecoder decodeObjectForKey:@"data"];
		taxonName           = [aDecoder decodeObjectForKey:@"taxonName"];
		[data retain];
		[taxonName retain];
		}
	return self;
}

- (id)initWithRbTaxonData:(RbTaxonData*)d {

    if ( (self = [super init]) ) 
		{
		// allocate an array holding the data
		data      = [[NSMutableArray alloc] init];
		taxonName = [[NSString alloc] init];
		
		// copy information
		if (d != nil)
			{
			dataType      = [d dataType];
			taxonName     = [NSString stringWithString:[d taxonName]];
			for (int i=0; i<[d numCharacters]; i++)
				{
				RbDataCell* c = [d dataCellIndexed:i]; 
				RbDataCell* newC = [[RbDataCell alloc] initWithCell:c];
				[data addObject:newC];
				}
			}
		}
    return self;
}

- (int)numCharacters {

    return (int)[data count];
}

@end
