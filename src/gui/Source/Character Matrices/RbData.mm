#import "RbData.h"
#import "RbDataCell.h"



@implementation RbData

@synthesize dataType;
@synthesize name;
@synthesize numCharacters;
@synthesize numTaxa;

- (void)addCell:(RbDataCell*)cell {

	[data addObject:cell];
}

- (void)addTaxonName:(NSString*)n {

	[taxonNames addObject:n];
}

- (RbDataCell*)cellWithRow:(int)r andColumn:(int)c {

	return [data objectAtIndex:(r * numCharacters + c)];
}

- (RbDataCell*)dataCellIndexed:(int)idx {

	return [data objectAtIndex:idx];
}

- (int)dataSize {

	return (int)[data count];
}

- (void)dealloc {

	[data release];
	[name release];
	[taxonNames release];
	[excludedTaxa release];
	[excludedCharacters release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodeInt:dataType               forKey:@"dataType"];
	[aCoder encodeInt:numTaxa                forKey:@"numTaxa"];
	[aCoder encodeInt:numCharacters          forKey:@"numCharacters"];
	[aCoder encodeObject:data                forKey:@"data"];
	[aCoder encodeObject:name                forKey:@"name"];
	[aCoder encodeObject:taxonNames          forKey:@"taxonNames"];
	[aCoder encodeObject:excludedTaxa        forKey:@"excludedTaxa"];
	[aCoder encodeObject:excludedCharacters  forKey:@"excludedCharacters"];
}

- (void)excludeTaxonIndexed:(int)idx {

	if ( [self isTaxonExcluded:idx] == NO )
		{
		NSString* tn = [self taxonWithIndex:idx];
		[excludedTaxa addObject:tn];
		}
}


- (void)excludeCharacterIndexed:(int)idx {

	if ( [self isCharacterExcluded:idx] == NO )
		[excludedCharacters addObject:[NSNumber numberWithInt:idx]];
}

- (void)includeAllCharacters {

    [excludedCharacters removeAllObjects];
}

- (void)includeAllTaxa {

    [excludedTaxa removeAllObjects];
}

- (int)indexOfTaxonNamed:(NSString*)nme {

	id element;
	NSEnumerator* taxonEnumerator = [taxonNames objectEnumerator];
	int i = 0;
	while ( (element = [taxonEnumerator nextObject]) )
		{
		if ( [element isEqualToString:nme] == YES )
			return i;
		i++;
		}
	return -1;
}

- (id)init {

    if ( (self = [super init]) ) 
		{
		// allocate an array holding the data
		data       = [[NSMutableArray alloc] init];
		name       = [[NSString alloc] init];
		taxonNames = [[NSMutableArray alloc] init];
		
		// allocate sets keeping track of excluded taxa and characters
		excludedTaxa        = [[NSMutableSet alloc] init];
		excludedCharacters  = [[NSMutableSet alloc] init];
        
        // initialize some variables
		numTaxa            = 0;
		numCharacters      = 0;
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        dataType            = [aDecoder decodeIntForKey:@"dataType"];
		numTaxa             = [aDecoder decodeIntForKey:@"numTaxa"];
		numCharacters       = [aDecoder decodeIntForKey:@"numCharacters"];
		data                = [aDecoder decodeObjectForKey:@"data"];
		name                = [aDecoder decodeObjectForKey:@"name"];
		taxonNames          = [aDecoder decodeObjectForKey:@"taxonNames"];
		excludedTaxa        = [aDecoder decodeObjectForKey:@"excludedTaxa"];
		excludedCharacters  = [aDecoder decodeObjectForKey:@"excludedCharacters"];
		[data retain];
		[name retain];
		[taxonNames retain];
		[excludedTaxa retain];
		[excludedCharacters retain];
		}
	return self;
}

- (id)initWithRbData:(RbData*)d {

    if ( (self = [super init]) ) 
		{
		// allocate an array holding the data
		data       = [[NSMutableArray alloc] init];
		name       = [[NSString alloc] init];
		taxonNames = [[NSMutableArray alloc] init];
		
		// allocate sets keeping track of excluded taxa and characters
		excludedTaxa        = [[NSMutableSet alloc] init];
		excludedCharacters  = [[NSMutableSet alloc] init];
        
        // initialize some variables
		numTaxa            = 0;
		numCharacters      = 0;

		// copy information
		if (d != nil)
			{
			numTaxa       = [d numTaxa];
			numCharacters = [d numCharacters];
			dataType      = [d dataType];
			name          = [NSString stringWithString:[d name]];
			for (int i=0; i<[d numTaxa]; i++)
				{
				NSString* tn = [NSString stringWithString:[d taxonWithIndex:i]];
				[taxonNames addObject:tn];
				if ( [d isTaxonExcluded:i] == YES )
					[excludedTaxa addObject:[NSNumber numberWithInt:i]];
				}
			for (int i=0; i<[d numCharacters]; i++)
				{
				if ( [d isCharacterExcluded:i] == YES )
					[excludedCharacters addObject:[NSNumber numberWithInt:i]];
				}
			for (int i=0; i<[d dataSize]; i++)
				{
				RbDataCell* c = [d dataCellIndexed:i]; 
				RbDataCell* newC = [[RbDataCell alloc] initWithCell:c];
				[data addObject:newC];
				}
			}
		}
    return self;
}

- (BOOL)isCharacterExcluded:(int)idx {

	id element;
	NSEnumerator* characterEnumerator = [excludedCharacters objectEnumerator];
	while ( (element = [characterEnumerator nextObject]) )
		{
		if ( [element intValue] == idx )
			return YES;
		}
	return NO;
}

- (BOOL)isCharacterMissAmbig:(int)idx {
	
	for (int i=0; i<numTaxa; i++)
		{
		RbDataCell* aCell = [self cellWithRow:i andColumn:idx];
		if ( [aCell isAmbig] == YES )
			return YES;
		}
	return NO;
}

- (BOOL)isTaxonExcluded:(int)idx {

	NSString* tn = [self taxonWithIndex:idx];
	id element;
	NSEnumerator* taxonEnumerator = [excludedTaxa objectEnumerator];
	while ( (element = [taxonEnumerator nextObject]) )
		{
		if ( [element isEqualToString:tn] == YES )
			return YES;
		}
	return NO;
}

- (int)numExcludedCharacters {

    return (int)[excludedCharacters count];
}

- (int)numExcludedTaxa {

    return (int)[excludedTaxa count];
}

- (void)print {

	if (numTaxa * numCharacters != [data count])
		{
		NSLog(@"problem printing data matrix");
		}
		
	for (int i=0, k=0; i<numTaxa; i++)
		{
		for (int j=0; j<numCharacters; j++)
			{
			RbDataCell* cell = [data objectAtIndex:k++];
			NSNumber* n = [cell val];
			if ([cell isDiscrete] == YES)
				printf("%u ", [n unsignedIntValue]);
			else 
				printf("%2.4lf ", [n doubleValue]);
			}
		printf("\n");
		}
}

- (void)restoreTaxonIndexed:(int)idx {

	NSString* tn = [self taxonWithIndex:idx];
	id element;
	NSEnumerator* taxonEnumerator = [excludedTaxa objectEnumerator];
	while ( (element = [taxonEnumerator nextObject]) )
		{
		if ( [element isEqualToString:tn] == YES )
			{
			[excludedTaxa removeObject:element];
			return;
			}
		}
}

- (void)restoreCharacterIndexed:(int)idx {

	id element;
	NSEnumerator* characterEnumerator = [excludedCharacters objectEnumerator];
	while ( (element = [characterEnumerator nextObject]) )
		{
		if ( [element intValue] == idx )
			{
			[excludedCharacters removeObject:element];
			return;
			}
		}
}

- (NSString*)taxonWithIndex:(int)i {

	return [taxonNames objectAtIndex:i];
}

@end
