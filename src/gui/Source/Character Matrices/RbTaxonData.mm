#import "RbDataCell.h"
#import "RbTaxonData.h"



@implementation RbTaxonData

@synthesize dataType;
@synthesize taxonName;
@synthesize numCharacters;

- (RbDataCell*)dataCellIndexed:(int)idx {

	return [data objectAtIndex:idx];
}

- (void)dealloc {

	[data release];
	[taxonName release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodeInt:dataType      forKey:@"dataType"];
	[aCoder encodeInt:numCharacters forKey:@"numCharacters"];
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
		numCharacters      = 0;
        }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        dataType            = [aDecoder decodeIntForKey:@"dataType"];
		numCharacters       = [aDecoder decodeIntForKey:@"numCharacters"];
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
		
        // initialize some variables
		numCharacters = 0;

		// copy information
		if (d != nil)
			{
			numCharacters = [d numCharacters];
			dataType      = [d dataType];
			taxonName     = [NSString stringWithString:[d taxonName]];
			for (int i=0; i<numCharacters; i++)
				{
				RbDataCell* c = [d dataCellIndexed:i]; 
				RbDataCell* newC = [[RbDataCell alloc] initWithCell:c];
				[data addObject:newC];
				}
			}
		}
    return self;
}

@end
