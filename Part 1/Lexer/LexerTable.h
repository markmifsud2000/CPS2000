#ifndef CPS2000_ASSIGNMENT_LEXERTABLE_H
#define CPS2000_ASSIGNMENT_LEXERTABLE_H


#define E 32 //Error State


/*
 * Lookup table.
 * Represents any DFSAs used to accept/reject tokens in the language.
 */
const State table[][42] = {
        /*         cStart  cUnknown  cNewLine  cSpace  cDigit  cAsterisk  cFSlash  cPlus  cMinus  cLessThan  cGreaterThan  cEquals  cExclamation  cComma  cPoint  cUnderscore  cQuotation  cColon  cSemicolon  cLBracket  cRBracket  cLCurly  cRCurly  cLetter cPrintable */
/* State 00 */  {    E,       E,        1,        1,      2,      18,       19,      8,     9,       10,          12,        14,        16,        25,      E,        7,           5,       26,       27,         28,       29,        30,      31,       7,       E       }, /* State 00 */
/* State 01 */  {    E,       E,        1,        1,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 01 - Space */
/* State 02 */  {    E,       E,        E,        E,      2,       E,        E,      E,     E,        E,           E,         E,         E,         E,      3,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 02 - Integer Literal */
/* State 03 */  {    E,       E,        E,        E,      4,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 03 */
/* State 04 */  {    E,       E,        E,        E,      4,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 04 - Float Literal */
/* State 05 */  {    E,       E,        5,        5,      5,       5,        5,      5,     5,        5,           5,         5,         5,         5,      5,        5,           6,        5,        5,          5,        5,         5,       5,       5,       5       }, /* State 05 */
/* State 06 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 06 - String Literal */
/* State 07 */  {    E,       E,        E,        E,      7,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        7,           E,        E,        E,          E,        E,         E,       E,       7,       E       }, /* State 07 - Identifier */
/* State 08 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 08 - Plus */
/* State 09 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 09 - Minus */
        /*         cStart  cUnknown  cNewLine  cSpace  cDigit  cAsterisk  cFSlash  cPlus  cMinus  cLessThan  cGreaterThan  cEquals  cExclamation  cComma  cPoint  cUnderscore  cQuotation  cColon  cSemicolon  cLBracket  cRBracket  cLCurly  cRCurly  cLetter cPrintable */
/* State 10 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,        11,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 10 - Less Than */
/* State 11 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 11 - Less Than Equals */
/* State 12 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,        13,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 12 - Greater Than */
/* State 13 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 13 - Greater Than Equals */
/* State 14 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,        14,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 14 - Equals (Assign) */
/* State 15 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 15 - Equals (Relational) */
/* State 16 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,        17,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 16 */
/* State 17 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 17 - Not Equals */
/* State 18 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 18 - Asterisk */
/* State 19 */  {    E,       E,        E,        E,      E,      22,       20,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 19 - Forward Slash*/
        /*         cStart  cUnknown  cNewLine  cSpace  cDigit  cAsterisk  cFSlash  cPlus  cMinus  cLessThan  cGreaterThan  cEquals  cExclamation  cComma  cPoint  cUnderscore  cQuotation  cColon  cSemicolon  cLBracket  cRBracket  cLCurly  cRCurly  cLetter cPrintable */
/* State 20 */  {    E,      20,       21,       20,     20,      20,       20,     20,    20,       20,          20,        20,        20,        20,     20,       20,          20,       20,       20,         20,       20,        20,      20,      20,      20       }, /* State 20 - Comment */
/* State 21 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 21 - Comment */
/* State 22 */  {    E,      22,       22,       22,     22,      23,       22,     22,    22,       22,          22,        22,        22,        22,     22,       22,          22,       22,       22,         22,       22,        22,      22,      22,      22       }, /* State 22 */
/* State 23 */  {    E,      22,       22,       22,     22,      23,       24,     22,    22,       22,          22,        22,        22,        22,     22,       22,          22,       22,       22,         22,       22,        22,      22,      22,      22       }, /* State 23 */
/* State 24 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 24 - Comment */
/* State 25 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 25 - Comma */
/* State 26 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 26 - Colon */
/* State 27 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 27 - SemiColon */
/* State 28 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 28 - Left Bracket */
/* State 29 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 29 - Right Bracket */
        /*         cStart  cUnknown  cNewLine  cSpace  cDigit  cAsterisk  cFSlash  cPlus  cMinus  cLessThan  cGreaterThan  cEquals  cExclamation  cComma  cPoint  cUnderscore  cQuotation  cColon  cSemicolon  cLBracket  cRBracket  cLCurly  cRCurly  cLetter cPrintable */
/* State 30 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 30 - Left Curly Bracket */
/* State 31 */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State 31 - Right Curly Bracket */
/* State  E */  {    E,       E,        E,        E,      E,       E,        E,      E,     E,        E,           E,         E,         E,         E,      E,        E,           E,        E,        E,          E,        E,         E,       E,       E,       E       }, /* State  E */
};




/*
 * Lookup table used to determine whether a state is accepted or rejected.
 * Accepted states contain the token's type that was determined.
 */
        /* State:                 0         1           2             3            4            5             6             7         8      9        10            11             12              13               14              15            16         17     */
const TokenType accepting[] = {tREJECTED, tSPACE, tINTEGERLITERAL, tREJECTED, tFLOATLITERAL, tREJECTED, tSTRINGLITERAL, tIDENTIFIER, tPLUS, tMINUS, tLESSTHAN, tLESSTHANEQUAL, tGREATERTHAN, tGREATERTHANEQUAL, tEQUALASSIGN, tEQUALRELATIONAL, tREJECTED, tNOTEQUAL,
        /* State:                18         19       20        21        22         23         24       25      26        27         28          29       30       31         E     */
                               tASTERISK, tFSLASH, tCOMMENT, tCOMMENT, tREJECTED, tREJECTED, tCOMMENT, tCOMMA, tCOLON, tSEMICOLON, tLBRACKET, tRBRACKET, tLCURLY, tRCURLY, tREJECTED };




#endif //CPS2000_ASSIGNMENT_LEXERTABLE_H
