#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"



@implementation RbData

@synthesize dataType;
@synthesize isHomologyEstablished;
@synthesize name;
@synthesize alignmentMethod;
@synthesize numCharacters;
@synthesize numTaxa;

- (void)addTaxonData:(RbTaxonData*)td {

    [data addObject:td];
}

- (void)addTaxonName:(NSString*)n {

	[taxonNames addObject:n];
}

- (RbDataCell*)cellWithRow:(int)r andColumn:(int)c {

    if (r >= [data count])
        return nil;
    RbTaxonData* td = [data objectAtIndex:r];
    if (c >= [td numCharacters])
        return nil;
    return [td dataCellIndexed:c];
}

- (RbData*)copiedFrom {

    return copiedFrom;
}

- (int)dataSize {

	return (int)[data count];
}

- (void)dealloc {

    NSLog(@"delete %@", self);
	[data release];
	[name release];
    [alignmentMethod release];
	[taxonNames release];
	[excludedTaxa release];
	[excludedCharacters release];
	[super dealloc];
}

- (void)deleteLastTaxon {

    if ([data count] > 0)
        {
        NSLog(@"before = %d", [data count]);
        [data removeLastObject];
        [taxonNames removeLastObject];
        NSLog(@"after = %d", [data count]);
        numTaxa--;
        }
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodeInt:dataType               forKey:@"dataType"];
    [aCoder encodeBool:isHomologyEstablished forKey:@"isHomologyEstablished"];
    [aCoder encodeObject:alignmentMethod     forKey:@"alignmentMethod"];
	[aCoder encodeInt:numTaxa                forKey:@"numTaxa"];
	[aCoder encodeInt:numCharacters          forKey:@"numCharacters"];
	[aCoder encodeObject:data                forKey:@"data"];
	[aCoder encodeObject:name                forKey:@"name"];
	[aCoder encodeObject:taxonNames          forKey:@"taxonNames"];
	[aCoder encodeObject:excludedTaxa        forKey:@"excludedTaxa"];
	[aCoder encodeObject:excludedCharacters  forKey:@"excludedCharacters"];
    [aCoder encodeObject:copiedFrom          forKey:@"copiedFrom"];
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
        {
        NSLog(@"excluding character with index = %d for matrix %@", idx, self);
        NSNumber* n = [[NSNumber alloc] initWithInt:idx];
		[excludedCharacters addObject:n];
        [n release];
        }
}

- (RbTaxonData*)getDataForTaxonIndexed:(int)idx {

    return [data objectAtIndex:idx];
}

- (void)includeAllCharacters {

    NSLog(@"including all characters");

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
		data            = [[NSMutableArray alloc] init];
		name            = [[NSString alloc] init];
		taxonNames      = [[NSMutableArray alloc] init];
        alignmentMethod = [[NSString alloc] init];
		
		// allocate sets keeping track of excluded taxa and characters
		excludedTaxa        = [[NSMutableSet alloc] init];
		excludedCharacters  = [[NSMutableSet alloc] init];
        
        // initialize some variables
		numTaxa               = 0;
		numCharacters         = 0;
        isHomologyEstablished = NO;
        alignmentMethod       = [NSString stringWithString:@"Unknown"];
        copiedFrom            = nil;
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        dataType              = [aDecoder decodeIntForKey:@"dataType"];
        isHomologyEstablished = [aDecoder decodeBoolForKey:@"isHomologyEstablished"];
        alignmentMethod       = [aDecoder decodeObjectForKey:@"alignmentMethod"];
		numTaxa               = [aDecoder decodeIntForKey:@"numTaxa"];
		numCharacters         = [aDecoder decodeIntForKey:@"numCharacters"];
		data                  = [aDecoder decodeObjectForKey:@"data"];
		name                  = [aDecoder decodeObjectForKey:@"name"];
		taxonNames            = [aDecoder decodeObjectForKey:@"taxonNames"];
		excludedTaxa          = [aDecoder decodeObjectForKey:@"excludedTaxa"];
		excludedCharacters    = [aDecoder decodeObjectForKey:@"excludedCharacters"];
        copiedFrom            = [aDecoder decodeObjectForKey:@"copiedFrom"];
		[data retain];
		[name retain];
        [alignmentMethod retain];
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
		data            = [[NSMutableArray alloc] init];
		name            = [[NSString alloc] initWithString:@""];
		taxonNames      = [[NSMutableArray alloc] init];
        alignmentMethod = [[NSString alloc] initWithString:@""];
		
		// allocate sets keeping track of excluded taxa and characters
		excludedTaxa        = [[NSMutableSet alloc] init];
		excludedCharacters  = [[NSMutableSet alloc] init];
        
        // initialize some variables
		numTaxa            = 0;
		numCharacters      = 0;

		// copy information
		if (d != nil)
			{
			numTaxa               = [d numTaxa];
			numCharacters         = [d numCharacters];
			dataType              = [d dataType];
            isHomologyEstablished = [d isHomologyEstablished];
            [self setName:[d name]];
            [self setAlignmentMethod:[d alignmentMethod]];
            if ( [d copiedFrom] == nil )
                copiedFrom = d;
            else
                copiedFrom = [d copiedFrom];
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
            for (int i=0; i<[d numTaxa]; i++)
                {
                RbTaxonData* td = [d getDataForTaxonIndexed:i];
                RbTaxonData* newTd = [[RbTaxonData alloc] initWithRbTaxonData:td];
                [data addObject:newTd];
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

- (int)maxNumCharacters {

    int n = 0;
	id element;
	NSEnumerator* tdEnumerator = [data objectEnumerator];
	while ( (element = [tdEnumerator nextObject]) )
		{
        int x = [element numCharacters];
        if (x > n)
            n = x;
		}
    return n;
}

- (int)numCharactersForTaxon:(int)idx {

    RbTaxonData* td = [data objectAtIndex:idx];
    return [td numCharacters];
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

- (void)setCopiedFrom:(RbData*)d {

    copiedFrom = d;
}

- (void)setNameOfTaxonWithIndex:(int)idx to:(NSString*)newName {

    if (idx < 0 || idx >= [taxonNames count])
        NSLog(@"Error: Taxon index out of range");
    [taxonNames replaceObjectAtIndex:idx withObject:newName];
}

- (NSString*)taxonWithIndex:(int)i {

	return [taxonNames objectAtIndex:i];
}

- (void)writeToFile:(NSString*)fn {

    if ( [self isHomologyEstablished] == YES )
        {
        // write out as NEXUS file
        NSMutableString* outStr = [NSMutableString stringWithCapacity:100];
        [outStr appendString:@"#NEXUS\n\n"];
        [outStr appendString:@"begin data;\n"];
        [outStr appendFormat:@"   dimensions ntax=%d nchar=%d;\n", numTaxa, numCharacters];
        [outStr appendString:@"   format gap=- datatype="];
        if ( dataType == DNA )
            [outStr appendString:@"DNA;\n"];
        else if ( dataType == RNA )
            [outStr appendString:@"RNA;\n"];
        else if ( dataType == AA )
            [outStr appendString:@"protein;\n"];
        else if ( dataType == STANDARD )
            [outStr appendString:@"standard;\n"];
        [outStr appendString:@"   matrix\n"];
        for (int i=0; i<numTaxa; i++)
            {
            RbTaxonData* td = [self getDataForTaxonIndexed:i];
            [outStr appendString:@"   "];
            [outStr appendString:[td taxonName]];
            [outStr appendString:@"   "];
            for (int j=0; j<[td numCharacters]; j++)
                {
                RbDataCell* dc = [td dataCellIndexed:j];
                char c = [dc getDiscreteState];
                [outStr appendString:[NSString stringWithFormat:@"%c", c]];
                }
            [outStr appendString:@"\n"];
            }
        [outStr appendString:@"   ;\n"];
        [outStr appendString:@"end;\n\n"];

        NSError* myError;
        [outStr writeToFile:fn atomically:YES encoding:NSUTF8StringEncoding error:&myError];
        }
    else
        {
        // write out as FASTA file
        NSMutableString* outStr = [NSMutableString stringWithCapacity:100];
        for (int i=0; i<numTaxa; i++)
            {
            RbTaxonData* td = [self getDataForTaxonIndexed:i];
            [outStr appendString:@">"];
            [outStr appendString:[td taxonName]];
            [outStr appendString:@"\n"];
            for (int j=0; j<[td numCharacters]; j++)
                {
                RbDataCell* dc = [td dataCellIndexed:j];
                char c = [dc getDiscreteState];
                [outStr appendString:[NSString stringWithFormat:@"%c", c]];
                }
            [outStr appendString:@"\n\n"];
            }
        
        NSError* myError;
        [outStr writeToFile:fn atomically:YES encoding:NSUTF8StringEncoding error:&myError];
        }
}

@end
