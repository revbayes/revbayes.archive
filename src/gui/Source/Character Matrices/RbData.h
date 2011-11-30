#import <Cocoa/Cocoa.h>
@class RbDataCell;
@class RbTaxonData;



@interface RbData : NSObject <NSCoding> {

	NSMutableArray*      data;
    int                  dataType;
    BOOL                 isHomologyEstablished;
	NSString*            name;
	int                  numCharacters;
	int                  numTaxa;
	NSMutableArray*      taxonNames;
	NSMutableSet*        excludedTaxa;
	NSMutableSet*        excludedCharacters;
}

@property (readwrite) int dataType;
@property (readwrite) BOOL isHomologyEstablished;
@property (readwrite,retain) NSString* name;
@property (readwrite) int numCharacters;
@property (readwrite) int numTaxa;

- (void)addTaxonData:(RbTaxonData*)td;
- (void)addTaxonName:(NSString*)n;
- (RbDataCell*)cellWithRow:(int)r andColumn:(int)c;
- (int)dataSize;
- (void)excludeTaxonIndexed:(int)idx;
- (void)excludeCharacterIndexed:(int)idx;
- (void)includeAllCharacters;
- (void)includeAllTaxa;
- (int)indexOfTaxonNamed:(NSString*)nme;
- (id)initWithRbData:(RbData*)d;
- (BOOL)isCharacterExcluded:(int)idx;
- (BOOL)isTaxonExcluded:(int)idx;
- (BOOL)isCharacterMissAmbig:(int)idx;
- (int)numExcludedCharacters;
- (int)numExcludedTaxa;
- (int)numCharactersForTaxon:(int)idx;
- (void)print;
- (void)restoreTaxonIndexed:(int)idx;
- (void)restoreCharacterIndexed:(int)idx;
- (NSString*)taxonWithIndex:(int)i;

@end
