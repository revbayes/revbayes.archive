#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"



@implementation RbData

@synthesize dataType;
@synthesize isHomologyEstablished;
@synthesize name;
@synthesize alignmentMethod;
@synthesize stateLabels;
@synthesize numCharacters;
@synthesize numTaxa;

- (void)addTaxonData:(RbTaxonData*)td {

    [data addObject:td];
}

- (void)addTaxonName:(NSString*)n {

    [self cleanName:n];
	[taxonNames addObject:n];
}

- (RbDataCell*)cellWithRow:(size_t)r andColumn:(int)c {

    if (r >= [data count])
        return nil;
    RbTaxonData* td = [data objectAtIndex:r];
    if (c >= [td numCharacters])
        return nil;
    return [td dataCellIndexed:c];
}

- (char)stateWithRow:(size_t)r andColumn:(int)c {

    
    // get the data cell
    RbDataCell* dc = nil;
    if (r >= [data count])
        dc = nil;
    RbTaxonData* td = [data objectAtIndex:r];
    if (c >= [td numCharacters])
        dc = nil;
    dc = [td dataCellIndexed:c];
    if (dc == nil)
        return ' ';
    
    char ch = [dc getDiscreteStateWithLabels:stateLabels];
    return ch;
}

- (void)cleanName:(NSString*)nameStr {

    nameStr = [nameStr stringByReplacingOccurrencesOfString:@"\'" withString:@""];
    nameStr = [nameStr stringByReplacingOccurrencesOfString:@"\"" withString:@""];
    nameStr = [nameStr stringByReplacingOccurrencesOfString:@" " withString:@"_"];
}

- (void)clear {

    [data removeAllObjects];
    [taxonNames removeAllObjects];
    [excludedTaxa removeAllObjects];
    [excludedCharacters removeAllObjects];
    numTaxa = 0;
    numCharacters = 0;
}

- (RbData*)copiedFrom {

    return copiedFrom;
}

- (int)dataSize {

	return (int)[data count];
}

- (void)deleteLastTaxon {

    if ([data count] > 0)
        {
        [data removeLastObject];
        [taxonNames removeLastObject];
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
	[aCoder encodeObject:stateLabels         forKey:@"stateLabels"];
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
        NSNumber* n = [[NSNumber alloc] initWithInt:idx];
		[excludedCharacters addObject:n];
        }
}

- (RbTaxonData*)getDataForTaxonIndexed:(int)idx {

    return [data objectAtIndex:idx];
}

- (RbTaxonData*)getDataForTaxonWithName:(NSString*)ns {

    for (RbTaxonData* td in [data objectEnumerator])
        {
        NSString* str = [td taxonName];
        if ( [ns isEqualToString:str] == YES )
            return td;
        }
    return nil;
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
		data            = [[NSMutableArray alloc] init];
		name            = [[NSString alloc] init];
		taxonNames      = [[NSMutableArray alloc] init];
        alignmentMethod = [[NSString alloc] init];
		stateLabels     = [[NSString alloc] init];
		
		// allocate sets keeping track of excluded taxa and characters
		excludedTaxa        = [[NSMutableSet alloc] init];
		excludedCharacters  = [[NSMutableSet alloc] init];
        
        // initialize some variables
		numTaxa               = 0;
		numCharacters         = 0;
        isHomologyEstablished = NO;
        alignmentMethod       = @"Unknown";
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
		stateLabels           = [aDecoder decodeObjectForKey:@"stateLabels"];
		excludedTaxa          = [aDecoder decodeObjectForKey:@"excludedTaxa"];
		excludedCharacters    = [aDecoder decodeObjectForKey:@"excludedCharacters"];
        copiedFrom            = [aDecoder decodeObjectForKey:@"copiedFrom"];
		}
	return self;
}

- (id)initWithRbData:(RbData*)d {

    if ( (self = [super init]) ) 
		{
		// allocate an array holding the data
		data            = [[NSMutableArray alloc] init];
		name            = @"";
		taxonNames      = [[NSMutableArray alloc] init];
        alignmentMethod = @"";
        stateLabels     = @"";
		
		// allocate sets keeping track of excluded taxa and characters
		excludedTaxa       = [[NSMutableSet alloc] init];
		excludedCharacters = [[NSMutableSet alloc] init];
        
        // initialize some variables
		numTaxa       = 0;
		numCharacters = 0;

		// copy information
		if (d != nil)
			{
			numTaxa               = [d numTaxa];
			numCharacters         = [d numCharacters];
			dataType              = [d dataType];
            isHomologyEstablished = [d isHomologyEstablished];
            [self setName:[d name]];
            [self setAlignmentMethod:[d alignmentMethod]];
            [self setStateLabels:[d stateLabels]];
            if ( [d copiedFrom] == nil )
                copiedFrom = d;
            else
                copiedFrom = [d copiedFrom];
			for (int i=0; i<[d numTaxa]; i++)
				{
				NSString* tn = [[NSString alloc] initWithString:[d taxonWithIndex:i]];
                [self cleanName:tn];
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

- (BOOL)isTaxonNamePresent:(NSString*)theName {

    for (NSString* str in [taxonNames objectEnumerator])
        {
        if ( [str isEqualToString:theName] == YES )
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

	for (int i=0; i<numTaxa; i++)
		{
        RbTaxonData* td = [self getDataForTaxonIndexed:i];
        NSLog(@"name = %@", [td taxonName]);
		for (int j=0; j<[td numCharacters]; j++)
			{
			RbDataCell* cell = [td dataCellIndexed:j];
			NSNumber* n = [cell val];
            char c = ' ';
            if ( [cell dataType] == DNA )
                c = [cell interpretAsDna:[n unsignedIntValue]];
            else if ( [cell dataType] == RNA )
                c = [cell interpretAsDna:[n unsignedIntValue]];
            else if ( [cell dataType] == AA )
                c = [cell interpretAsAminoAcid:[n unsignedIntValue]];
            else if ( [cell dataType] == STANDARD )
                c = [cell interpretAsStandard:[n unsignedIntValue]];
                
            printf("%c", c);
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

- (void)setStandardMatrixToHave:(int)rows andToHave:(int)columns {

    [self clear];
    
    numTaxa               = rows;
    numCharacters         = columns;
    isHomologyEstablished = YES;
    dataType              = STANDARD;
    copiedFrom            = nil;
    
    for (int i=0; i<numTaxa; i++)
        {
        RbTaxonData* td = [[RbTaxonData alloc] init];
        [td setDataType:STANDARD];
        [td setTaxonName:[NSString stringWithFormat:@"Taxon_%d", i+1]];
        [self addTaxonName:[NSString stringWithFormat:@"Taxon_%d", i+1]];
        for (int j=0; j<numCharacters; j++)
            {
            RbDataCell* dc = [[RbDataCell alloc] init];
            [dc setDataType:STANDARD];
            [dc setNumStates:10];
            [dc setIsAmbig:YES];
            [td addObservation:dc];
            }
        [data addObject:td];
        }
}

- (void)setNameOfTaxonWithIndex:(size_t)idx to:(NSString*)newName {

    if ( idx >= [taxonNames count] )
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
        if ( dataType == STANDARD || dataType == MIXED )
            {
            // write a morphological data file
            NSMutableString* outStr = [NSMutableString stringWithCapacity:100];
            [outStr appendString:@"#NEXUS\n\n"];
            [outStr appendString:@"begin data;\n"];
            [outStr appendFormat:@"   dimensions ntax=%d nchar=%d;\n", numTaxa, numCharacters];
            [outStr appendFormat:@"   format symbols=\"%@\" missing=? equate=\"N=?\" gap=-;\n", stateLabels];
            [outStr appendString:@"   matrix\n"];
            for (int i=0; i<numTaxa; i++)
                {
                RbTaxonData* td = [self getDataForTaxonIndexed:i];
                [outStr appendString:@"   "];
                //NSString* tn = [td taxonName];
                NSString* tn = [self taxonWithIndex:i];
                NSString* tn2 = [tn stringByReplacingOccurrencesOfString:@" " withString:@"_"];
                [self cleanName:tn2];
                [outStr appendString:tn2];
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
            // write a non-morphological data file
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
            [outStr appendString:@"   matrix\n"];
            for (int i=0; i<numTaxa; i++)
                {
                RbTaxonData* td = [self getDataForTaxonIndexed:i];
                [outStr appendString:@"   "];
                [outStr appendString:[self taxonWithIndex:i]];
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
        }
    else
        {
        // write out as FASTA file
        NSMutableString* outStr = [NSMutableString stringWithCapacity:100];
        for (int i=0; i<numTaxa; i++)
            {
            RbTaxonData* td = [self getDataForTaxonIndexed:i];
            [outStr appendString:@">"];

            [outStr appendString:[self taxonWithIndex:i]];
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
