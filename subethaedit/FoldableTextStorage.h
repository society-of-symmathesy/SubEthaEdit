//
//  FoldableTextStorage.h
//  TextEdit
//
//  Created by Dominik Wagner on 04.01.09.
//  Copyright 2009 TheCodingMonkeys. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <OgreKit/OgreKit.h>
#import "NSMutableAttributedStringSEEAdditions.h"

#import "FoldedTextAttachment.h"
#import "AbstractFoldingTextStorage.h"
#import "TextStorage.h"

@class FullTextStorage;

extern NSString * const TextStorageLineEndingDidChange;
extern NSString * const TextStorageHasMixedLineEndingsDidChange;


extern NSString * const BlockeditAttributeName ;
extern NSString * const BlockeditAttributeValue;

@interface FoldableTextStorage : AbstractFoldingTextStorage {
	FullTextStorage *I_fullTextStorage;
	NSMutableArray *I_sortedFoldedTextAttachments;
	int I_editingCount;

    struct {
        BOOL hasBlockeditRanges;
        BOOL  isBlockediting;
        BOOL didBlockedit;
        NSRange didBlockeditRange;
        NSRange didBlockeditLineRange;
    } I_blockedit;

    NSMutableAttributedString *I_internalAttributedString;
    unsigned I_numberOfWords;

    TextStorage *I_containerTextStorage;
    struct {
        int length;
        int characterOffset;
        int startLine;
        int endLine;
    } I_scriptingProperties;
    
}

- (NSRange)foldedRangeForFullRange:(NSRange)inRange;
- (NSRange)fullRangeForFoldedRange:(NSRange)inRange;
- (FullTextStorage *)fullTextStorage;

- (void)fullTextDidReplaceCharactersInRange:(NSRange)inRange withString:(NSString *)inString;
- (void)fullTextDidSetAttributes:(NSDictionary *)inAttributes range:(NSRange)inRange;

#pragma mark folding methods
- (int)numberOfTopLevelFoldings;
- (void)foldRange:(NSRange)inRange;
- (void)unfoldAttachment:(FoldedTextAttachment *)inAttachment atCharacterIndex:(unsigned)inIndex;
- (void)unfoldAll;
- (void)foldAllWithFoldingLevel:(int)aFoldingLevel;

#pragma mark line numbers
- (int)lineNumberForLocation:(unsigned)location;
- (NSString *)positionStringForRange:(NSRange)aRange;
- (NSRange)findLine:(int)aLineNumber;
- (void)setHasMixedLineEndings:(BOOL)aFlag;

#pragma mark Blockedit
- (BOOL)hasBlockeditRanges;
- (void)setHasBlockeditRanges:(BOOL)aFlag;
- (BOOL)isBlockediting;
- (void)setIsBlockediting:(BOOL)aFlag;
- (BOOL)didBlockedit;
- (void)setDidBlockedit:(BOOL)aFlag;
- (NSRange)didBlockeditRange;
- (void)setDidBlockeditRange:(NSRange)aRange;
- (NSRange)didBlockeditLineRange;
- (void)setDidBlockeditLineRange:(NSRange)aRange;

- (void)stopBlockedit;


#pragma mark debug output
- (NSMutableAttributedString *)attributedStringOfFolding:(FoldedTextAttachment *)inAttachment;
- (NSString *)foldedStringRepresentationOfRange:(NSRange)inRange foldings:(NSArray *)inFoldings level:(int)inLevel;
- (NSString *)foldedStringRepresentation;

- (LineEnding)lineEnding;
- (void)setLineEnding:(LineEnding)newLineEnding;
- (void)setShouldWatchLineEndings:(BOOL)aFlag;
- (BOOL)hasMixedLineEndings;
- (void)setHasMixedLineEndings:(BOOL)aFlag;
- (unsigned int)encoding;
- (void)setEncoding:(unsigned int)anEncoding;
- (NSArray *)selectionOperationsForRangesUnconvertableToEncoding:(NSStringEncoding)encoding;


@end

@interface NSObject (FoldableTextStorageDelegateAdditions)

- (void)textStorageDidChangeNumberOfTopLevelFoldings:(FoldableTextStorage *)aTextStorage;

@end

@interface NSObject (TextStorageBlockeditDelegateAdditions)

- (NSDictionary *)blockeditAttributesForTextStorage:(TextStorage *)aTextStorage;
- (void)textStorageDidStopBlockedit:(TextStorage *)aTextStorage;
- (void)textStorageDidStartBlockedit:(TextStorage *)aTextStorage;

@end

@interface FoldableTextStorage (FoldableTextStorageDelegateAdditions)
- (void)textStorage:(FullTextStorage *)aTextStorage willReplaceCharactersInRange:(NSRange)aRange withString:(NSString *)aString;
- (void)textStorage:(FullTextStorage *)aTextStorage didReplaceCharactersInRange:(NSRange)aRange withString:(NSString *)aString;

@end

#pragma mark -

@interface FoldableTextStorage (TextStorageScriptingAdditions)

- (id)insertionPoints;

- (NSRange)rangeRepresentation;
- (NSNumber *)scriptedLength;
- (NSNumber *)scriptedStartCharacterIndex;
- (NSNumber *)scriptedNextCharacterIndex;
- (NSNumber *)scriptedStartLine;
- (NSNumber *)scriptedEndLine;
- (NSString *)scriptedContents;
- (void)setScriptedContents:(id)string;
- (NSArray *)scriptedCharacters;
- (NSArray *)scriptedLines;

@end

