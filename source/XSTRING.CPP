/****************************************************************************************

XString

(c) 1996 by Jens von Pilgrim
   1997 by Jens von Pilgrim

Details: See header file

History:

Note: in Strip: while-loops (correct for non-short-evaluation... (first<length)

*****************************************************************************************/


#include "XString.h"

#define DEBUG
#include "ool.h"
//#include <stdlib.h>
//#include <string.h>
#include <ctype.h>
#include <locale.h>


#if !defined(__IBMCPP__)
   #define iswspace(a)  ( a == ' ' ? TRUE : FALSE)
#else
   #include <wctype.h>
#endif

#define XSTRING_DEFBLOCKSIZE 1

/*@ XString::StrCmp(const XString &inThen)
@group Comperasion
@remarks Returns exactly the result from strcmp, usefull if other function needs this result.
The Result is

== 0: this string == inThen

 < 0: this string < inThen

 > 0: this string > inThen
*/
int XString::StrCmp(const XString &inThen) const
{ X_ASSERT(m_pszChar!=NULL);
  X_ASSERT( inThen.m_pszChar!=NULL);

  return strcmp(m_pszChar, inThen.m_pszChar);
}



/*@ XString::XString()
@group Constructor/Destructor
@remarks Constructs an empty string
*/
XString::XString()
{
  m_pszChar = (char *) malloc(1);
  X_ASSERT(m_pszChar != NULL);
  m_pszChar[0] = '\0';
  m_Length = 0;
  m_isNULL = true;
  m_nBlockSize = XSTRING_DEFBLOCKSIZE;
}

/*@ XString::XString(const XString &aString)
@group Constructor/Destructor
@remarks Constructs a string as a copy of 'aString'
*/
XString::XString(const XString &aString)
{
  X_ASSERT(&aString != NULL);
  X_ASSERT(&aString.m_pszChar != NULL);

  m_Length = aString.m_Length;
  m_isNULL = aString.m_isNULL;

  m_pszChar = (char *) malloc(m_Length+1);
  X_ASSERT(m_pszChar != NULL);

  memcpy(m_pszChar, aString.m_pszChar, m_Length+1);

  X_ASSERT(m_Length == strlen(m_pszChar));
  m_nBlockSize = XSTRING_DEFBLOCKSIZE;
}

/*@ XString::XString(char *pszChar)
@group Constructor/Destructor
@remarks Constructs a string as a copy of pszChar, usefull as caster
*/
XString::XString(char *pszChar)
{
    X_ASSERT(pszChar != NULL);
    m_Length = strlen(pszChar);
    m_pszChar = (char *) malloc(m_Length+1);
    X_ASSERT(m_pszChar != NULL);

   memcpy(m_pszChar, pszChar, m_Length+1);

    X_ASSERT(GetLength() == strlen(m_pszChar));
   m_isNULL = false;
   m_nBlockSize = XSTRING_DEFBLOCKSIZE;
}




XString::XString(const char *pszChar)
{
    X_ASSERT(pszChar != NULL);
    m_Length = strlen(pszChar);
    m_pszChar = (char *) malloc(m_Length+1);
    X_ASSERT(m_pszChar != NULL);

    memcpy(m_pszChar, pszChar, m_Length+1);

    X_ASSERT(GetLength() == strlen(m_pszChar));
   m_isNULL = false;
   m_nBlockSize = XSTRING_DEFBLOCKSIZE;
}

XString::XString(char *pszChar, int duplicate)
{
    if (pszChar != NULL) m_Length = strlen(pszChar);
    else m_Length = 0;
    m_pszChar = pszChar;
   m_isNULL = false;
   m_nBlockSize = XSTRING_DEFBLOCKSIZE;
}


/*@  XString::XString(char aChar)
@group Constructor/Destructor
@remarks Constructs a string with first char == aChar
*/
XString::XString(char aChar)
{
  m_Length = (aChar!=0) ? 1 : 0;

  m_pszChar = (char *) malloc(m_Length+1);
  X_ASSERT(m_pszChar != NULL);

  m_pszChar[0] = aChar;
  m_pszChar[m_Length] = '\0';

  X_ASSERT(m_Length == strlen(m_pszChar));
  m_isNULL = false;
  m_nBlockSize = XSTRING_DEFBLOCKSIZE;
}

/*@  XString::XString(int aNumber, int Radix)
@group Constructor/Destructor
@remarks Constructs a string and casts an int, usefull as caster
*/
XString::XString(int aNumber, int Radix)
{ m_pszChar = (char *) malloc(20);
  _itoa(aNumber, m_pszChar, Radix);
  m_Length = strlen(m_pszChar);
  ReleaseBuffer();
  m_isNULL = false;
  m_nBlockSize = XSTRING_DEFBLOCKSIZE;
}

/*@  XString::XString(long aNumber, int Radix)
@group Constructor/Destructor
@remarks Constructs a string and casts an int, usefull as caster
*/
XString::XString(long aNumber, int Radix)
{   m_pszChar = (char *) malloc(40);
   _ltoa(aNumber, m_pszChar, Radix);
   m_Length = strlen(m_pszChar);
   ReleaseBuffer();
   m_nBlockSize = XSTRING_DEFBLOCKSIZE;
   m_isNULL = false;
}

/*@  XString::~XString()
@group Constructor/Destructor
@remarks (Virtual) Destructor, removes string from memory
*/
XString::~XString()
{
  X_ASSERT(m_pszChar != NULL);
  free   (m_pszChar);
}



/*@ XString::operator =(const XString &aString)
@group Set
@returns Returns copy of aString
@remarks The Sourcestring aString is copied (duplicated) to this-string
*/
XString XString::operator =(const XString &aString)
{
  X_ASSERT(aString.m_pszChar != NULL);
  X_ASSERT(aString.m_Length == strlen(aString.m_pszChar));

  if (m_pszChar==aString.m_pszChar) return *this;   // July 9 97

  free(m_pszChar);
  m_Length = aString.m_Length;
  m_pszChar = (char *) malloc(m_Length+1);
  X_ASSERT(m_pszChar != NULL);

  memcpy(m_pszChar, aString.m_pszChar, m_Length+1);

  X_ASSERT(m_Length == strlen(m_pszChar));
  m_isNULL = aString.m_isNULL;

  return *this;
}



/*@ XString::operator () (..)
*/
/*
const char *const XString::operator() (XSIZE_T inCount)
{
  return Left(inCount)();
}
*/

/*@ XString::operator() ()
@group Get
*/
/*
const char *const XString::operator() () const
{ return m_pszChar;
}
*/

/*@ XString::operator[] ()
@group Get
@remarks Returns char-reference at position index
         The first char has index 0!
@returns Returns the char at position inZeroIndex.
*/
char& XString::operator[](XSIZE_T inZeroIndex) const
{ X_ASSERT(inZeroIndex<m_Length);
  return m_pszChar[inZeroIndex];
}

/*@ XString::operator[] ()
@group Get
@remarks
@returns Returns the char at position inZeroIndex.
*/
char& XString::operator[] (int inZeroIndex) const
{ X_ASSERT(inZeroIndex<m_Length);
  return m_pszChar[inZeroIndex];
}

/*@ XString::At ()
@group Get
@remarks Returns copy(!) of char at position index.
         If index is greater than the length, 0 is returned.
       The first char has index 0!
@returns Returns the char at position inZeroIndex od 0.
*/
char XString::At(XSIZE_T inZeroIndex) const
{ if (inZeroIndex>=m_Length) return 0;
  return m_pszChar[inZeroIndex];
}

/*@ XString:: ()
@group Get
@remarks Returns pointer of charpointer of the string
@returns lpsz-Pointer
*/
/*
XString::operator const char *const() const
{ return m_pszChar; }
*/

/*@ char * (..)
@group   Caster
@scope   public
@brief
@return   <t ';' c=2>;
   XString::operator char;
         </t>
@remarks

@history   <t ';' c=2>;
   Jul 09 1997;   created by ;
         </t>
*/
/*
XString::operator char *() const { return m_pszChar; }
*/


/*@ XString::MakeInt (..)
@group   Caster
@scope   public
@brief   Wandelt einen XString in einen interger um
@return   <t ';' c=2>;
   int;
         </t>
@remarks
Wandelt einen XString in einen interger um
@history   <t ';' c=2>;
   Jul 09 1997;   created by Jens von Pilgrim;
         </t>
@author   Jens von Pilgrim
*/
int XString::MakeInt() const
{
   X_ASSERT(m_pszChar != NULL);
   return atoi(m_pszChar);
}


/*@ XString::MakeLong (..)
@group   Caster
@scope   public
@brief   Wandelt einen XString in einen long integer um
@return   <t ';' c=2>;
   long;
         </t>
@remarks
Wandelt einen XString in einen long integer um
@history   <t ';' c=2>;
   Jul 09 1997;   created by Jens von Pilgrim;
         </t>
@author   Jens von Pilgrim
*/
long XString::MakeLong() const
{ X_ASSERT(m_pszChar != NULL);
  return atol(m_pszChar);
}

/*@ XString::MakeDouble (..)
@group   Caster
@scope   public
@brief   Wandelt einen XString in einen double um
@return   <t ';' c=2>;
   double;
         </t>
@remarks
Wandelt einen XString in einen double um
@history   <t ';' c=2>;
   Jul 09 1997;   created by Jens von Pilgrim;
         </t>
@author   Jens von Pilgrim
*/
double XString::MakeDouble() const
{ X_ASSERT(m_pszChar != NULL);
  return atof(m_pszChar);
}


/*@ XString::operator +=(const XString &Str)
@group Set
@returns Returns thisString+Str
@remarks Concats string with another string Str and saves the result in string.
@example
XString a,b,c;

...

a=(b+=c);

Result: (a==b) = true
*/
XString XString::operator +=(const XString &Str)
{
  m_isNULL = false;

  X_ASSERT(Str.m_pszChar!=NULL);
  X_ASSERT(m_pszChar!=NULL);
  X_ASSERT(m_Length==strlen(m_pszChar));

  char *newChar = (char *) malloc(m_Length+Str.m_Length+1);

  X_ASSERT(newChar != NULL);

  memcpy(newChar, m_pszChar, m_Length);
  memcpy(&newChar[m_Length], Str.m_pszChar, Str.m_Length + 1);

  free(m_pszChar);
  m_pszChar = newChar;
  m_Length += Str.m_Length;

  X_ASSERT(m_Length == strlen(m_pszChar));

  return *this;
}


/*@ XString::operator +=(const char *Str)
@group Set
@remarks Adds a string
*/
XString XString::operator +=(const char *Str)
{
  m_isNULL = false;

  X_ASSERT(Str!=NULL);
  X_ASSERT(m_pszChar!=NULL);
  X_ASSERT(m_Length==strlen(m_pszChar));

  XSIZE_T length = strlen(Str);
  char *newChar = (char *) malloc(m_Length+length+1);

  X_ASSERT(newChar != NULL);

  memcpy(newChar, m_pszChar, m_Length);
  memcpy(&newChar[m_Length], Str, length + 1);

  free(m_pszChar);
  m_pszChar = newChar;
  m_Length += length;


  X_ASSERT(m_Length == strlen(m_pszChar));

  return *this;


}

/*@ XString::Insert()
@group Set
@remarks Inserts a substring into the sting at postion pos.
@example
XString A("ABCGHI");
A.Insert("DEF", 3);
cout << A();      // result: ABCDEFGHI
*/
const XString XString::Insert(const XString& inSubStr , XSIZE_T inPos)
{
   m_isNULL = false;

   X_ASSERT(m_pszChar!=NULL);
   X_ASSERT(m_Length==strlen(m_pszChar));

   if (inPos>GetLength())
   {
      return operator +=(inSubStr);
   }

   char *newChar = (char *) malloc(m_Length+inSubStr.m_Length+1);

   X_ASSERT(newChar != NULL);

   if (inPos>0)
      memcpy(newChar, m_pszChar, inPos);
   memcpy(newChar+inPos, inSubStr.m_pszChar, inSubStr.m_Length);
   memcpy(newChar+inPos+inSubStr.m_Length, m_pszChar+inPos, GetLength()-inPos+1);

   free(m_pszChar);
   m_pszChar = newChar;
   m_Length += inSubStr.m_Length;

   X_ASSERT(m_Length == strlen(m_pszChar));

   return *this;
}




/*@ XString::IsEmpty ()
@group Comperasion
@returns true, if the String is empty

BOOL, else
@remarks If the string is not allocated (f.e. a Stringpointer before calling new) is NOT empyt! If
you call any String-Function before calling the Constructor, most functions fail and the program
will exit by a failed X_ASSERT!
*/
BOOL XString::IsEmpty() const
{ return GetLength()==0; }


void XString::MakeEmpty()
{
  m_pszChar = (char*) realloc(m_pszChar, 1);
  X_ASSERT(m_pszChar != NULL);
  m_pszChar[0] = '\0';
  m_Length = 0;
  m_isNULL = false;
}


BOOL XString::IsNULL() const
{
   return m_isNULL;
}

void XString::MakeNULL()
{
   MakeEmpty();
   m_isNULL = true;
}


/*@ XString::Mid(XSIZE_T From, XSIZE_T Count)
@group Get
@returns Returns substring with Count-length from position From in string.
@remarks If From>GetLength(), an empty string is returned; if From+Count>GetLength(),
a string with a length of (GetLength()-From) is returned.
*/
XString XString::Mid(XSIZE_T From, XSIZE_T Count) const
{
  X_ASSERT(m_pszChar != NULL);
  X_ASSERT(m_Length == strlen(m_pszChar));

  if ((Count == 0) || (From>=m_Length)) return XString();
  if (From+Count>m_Length) Count=m_Length-From;

  X_ASSERT (Count<=m_Length);

  XString Res(NULL, 1);
  Res.m_pszChar = (char *) malloc(Count+1);

  X_ASSERT(Res.m_pszChar != NULL);

  memcpy(Res.m_pszChar, m_pszChar+From, Count);
  Res.m_pszChar[Count]=0;
  Res.m_Length = Count;

  X_ASSERT(Res.m_Length == strlen(Res.m_pszChar));

  return Res;
}

/*@ XString::Left(XSIZE_T Count)
@group Get
@returns Returns the first Count chars as a XString from string. Thus, it's equal to Mid(0, nCount).
@remarks If Count>GetLength(), a copy of the string is returned, if Count is 0, an empty string is returned.
*/

XString XString::Left(XSIZE_T Count) const
{
  X_ASSERT(m_pszChar != NULL);

  if (Count==0) return XString();

  if (Count>m_Length) Count=m_Length;

  XString Res(NULL, 1);
  Res.m_Length = Count;
  Res.m_pszChar = (char *) malloc(Count+1);

  X_ASSERT(Res.m_pszChar != NULL);

  memcpy(Res.m_pszChar, m_pszChar, Count);
  Res.m_pszChar[Count]=0;

 X_ASSERT(Res.m_Length == strlen(Res.m_pszChar));

  return Res;
}


/*@ XString::Right(..)
@group Get
@remarks What do you think this methods is for? See Left() for details, rigth up!
*/
XString XString::Right(XSIZE_T Count) const
{
  X_ASSERT(m_pszChar != NULL);

  if (Count==0) return XString();
  if (Count>m_Length) Count=m_Length;

  XString Res(NULL, 1);
  Res.m_Length = Count;
  Res.m_pszChar = (char *) malloc(Count+1);

  X_ASSERT(Res.m_pszChar != NULL);

  memcpy(Res.m_pszChar, &m_pszChar[m_Length-Count], Count+1);
  // abschlieáendes 0 wird mitkopiert

  X_ASSERT(Res.m_Length == strlen(Res.m_pszChar));

  return Res;
}

/*@ XString::Find()
@group Enhanced
@returns true, if substring is in string, outPos is the (zero-indexed) position of the substring in string (the first). BOOL, if the substring was not found
@remarks Because some parameters have standard values, you can call this method also with Find(pos, "...").
*/
BOOL XString::Find(XSIZE_T &outPos, const XString &SubString, XSIZE_T From, XSIZE_T inTo, BOOL inInCase) const
{ X_ASSERT (SubString.m_pszChar != NULL);

  if ( (inTo==0) || (inTo>GetLength()) ) inTo = GetLength();
  if (inTo<From) return false;

  X_ASSERT (m_pszChar!=NULL);

  if ((SubString.m_Length==0) || (SubString.m_Length>inTo-From)) return false;

  XSIZE_T StrI, SubI=0,
         SubLength=SubString.m_Length-1,
         First;

  if (!inInCase)
  {
   for (StrI=From; StrI<inTo-SubLength; StrI++)
   {
      if (m_pszChar[StrI]==SubString.m_pszChar[SubI])
      {   First = StrI;
         do
         {
            if (SubI == SubLength) { outPos = First; return true; };
            SubI++;
            StrI++;
         }   while (m_pszChar[StrI]==SubString.m_pszChar[SubI]);
         SubI=0;
         StrI = First; // !!, eg */ in ***/ nicht gefunden!
      }
   }
  }
  else  // Find InCase
  {
   for (StrI=From; StrI<inTo-SubLength; StrI++)
   {
      if (UpperChar(m_pszChar[StrI])==UpperChar(SubString.m_pszChar[SubI]))
      {   First = StrI;
         do
         {
            if (SubI == SubLength) { outPos = First; return true; };
            SubI++;
            StrI++;
         }   while (UpperChar(m_pszChar[StrI])==UpperChar(SubString.m_pszChar[SubI]));
         SubI=0;
         StrI = First; // !!, eg */ in ***/ nicht gefunden!
      }
   }
  }



  return false;
}




/*@ XString::FindRev(XSIZE_T &outPos, const XString &SubString, XSIZE_T From = 0, XSIZE_T inTo = 0)
@group Enhanced
@returns true, if substring is in string, outPos is the (zero-indexed) position of the substring in string (the first). BOOL, if the substring was not found
@remarks Because some parameters have standard values, you can call this method also with Find(pos, "...").
@history <t ';' c=2>;
   1998/20/01; First character and complete string is find now, too!;
        </t>
*/
BOOL XString::FindRev(XSIZE_T &outPos, const XString &SubString, XSIZE_T From, XSIZE_T inTo, BOOL inInCase) const
{ X_ASSERT (SubString.m_pszChar != NULL);

  if (inTo==0) inTo = GetLength()-1;

  if (From>inTo) return false;

  X_ASSERT (m_pszChar!=NULL);

  if (SubString.m_Length==0) return false;
  if (SubString.m_Length-1>inTo-From) return false;

  XSIZE_T StrI, SubI=SubString.m_Length-1,
         SubLength=SubString.m_Length-1,
         First;

  if (!inInCase)
  {
   for (StrI=inTo; StrI>=From+SubLength ; StrI--)
   {
      if (m_pszChar[StrI]==SubString.m_pszChar[SubI])
      {
         First = StrI;
         do
         {
            if (SubI == 0) { outPos = First; return true; };
            SubI--;
            StrI--;
         }
         while (m_pszChar[StrI]==SubString.m_pszChar[SubI]);
         SubI=SubLength;
         StrI = First; // !!, eg */ in ***/ nicht gefunden!
      }
      if (StrI==0) return false;
   }
  }
  else    // FindRev InCase
  {
   for (StrI=inTo; StrI>=From+SubLength; StrI--)
   {
      if (UpperChar(m_pszChar[StrI])==UpperChar(SubString.m_pszChar[SubI]))
      {
         First = StrI;
         do
         {
            if (SubI == 0) { outPos = First; return true; };
            SubI--;
            StrI--;
         }
         while (UpperChar(m_pszChar[StrI])==UpperChar(SubString.m_pszChar[SubI]));
         SubI=SubLength;
         StrI = First; // !!, eg */ in ***/ nicht gefunden!
      }
      if (StrI==0) return false;
   }
  }

  return false;
}



/*@ XString::Search ()
@group Enhanced
@remarks Like Find() without position-variable
*/
BOOL XString::Search(const XString &SubString, XSIZE_T From, XSIZE_T inTo, BOOL inInCase) const
{  XSIZE_T dummy;
   return Find(dummy, SubString, From, inTo, inInCase);
}

/* XString::SearchRev()
@group Enhanced
@remarks Like Search, from back to front
*/
BOOL XString::SearchRev(const XString &SubString, XSIZE_T From, XSIZE_T inTo, BOOL inInCase) const
{  XSIZE_T dummy;
   return FindRev(dummy, SubString, From, inTo, inInCase);
}

/* XString::FindWhitespace ()
@group Enhanced
@remarks Returns position of next whitespace. This function uses iswspace(), so see there about
problems with country-codes.
*/
BOOL XString::FindWhitespace ( XSIZE_T &outPos, XSIZE_T inFrom, XSIZE_T inTo) const
{
   if (inTo==0) inTo = GetLength();
   if (inFrom>inTo) return false;


   X_ASSERT (m_pszChar!=NULL);

   outPos = inFrom;

   while ( outPos < GetLength() )
   {
      if ( iswspace( At ( (unsigned char) outPos ) ) ) return true;
      outPos ++;
   }
   return false;
}

/* XString::FindAlpha ()
@group Enhanced
@remarks Returns position of next alpha-char (a-z, A-Z). This function uses isalpha(),
so see there about problems with country-codes.
*/
BOOL XString::FindAlpha ( XSIZE_T &outPos, XSIZE_T inFrom, XSIZE_T inTo) const
{
   if (inTo==0) inTo = GetLength();
   if (inFrom>inTo) return false;


   X_ASSERT (m_pszChar!=NULL);

   outPos = inFrom;

   while ( outPos < GetLength() )
   {
      if ( isalpha( At ( outPos ) ) ) return true;
      outPos ++;
   }
   return false;
}

/* XString::FindNonAlpha ()
@group Enhanced
@remarks Returns position of next non-alpha-char (!= a-z, !=A-Z). This function uses isalpha(),
so see there about problems with country-codes.
*/
BOOL XString::FindNonAlpha ( XSIZE_T &outPos, XSIZE_T inFrom , XSIZE_T inTo) const
{
   if (inTo==0) inTo = GetLength();
   if (inFrom>inTo) return false;


   X_ASSERT (m_pszChar!=NULL);

   outPos = inFrom;

   while ( outPos < GetLength() )
   {
      if ( !isalpha( At ( outPos ) ) ) return true;
      outPos ++;
   }
   return false;
}

/* XString::SearchAlpha ()
@group Enhanced
@remarks Like FindAlpha(), without position-variable
*/
BOOL XString::SearchAlpha( XSIZE_T inFrom, XSIZE_T inTo ) const
{
   if (inTo==0) inTo = GetLength();
   if (inFrom>inTo) return false;


   X_ASSERT (m_pszChar!=NULL);

   inFrom;

   while ( inFrom < GetLength() )
   {
      if ( isalpha( At ( inFrom ) ) ) return true;
      inFrom ++;
   }
   return false;
}

/* XString::SearchNonAlpha ()
@group Enhanced
@remarks Like FindNonAlpha(), without position-variable
*/
BOOL XString::SearchNonAlpha ( XSIZE_T inFrom , XSIZE_T inTo ) const
{
   if (inTo==0) inTo = GetLength();
   if (inFrom>inTo) return false;


   X_ASSERT (m_pszChar!=NULL);

   inFrom;

   while ( inFrom < GetLength() )
   {
      if ( !isalpha( At ( inFrom ) ) ) return true;
      inFrom ++;
   }
   return false;
}

/*@ XString::SearchWhitespace ()
@remarks Stripes whitespaces from string, see Strip()
*/
BOOL XString::SearchWhitespace ( XSIZE_T inFrom, XSIZE_T inTo) const
{
   if (inTo==0) inTo = GetLength();
   if (inFrom>inTo) return false;


   X_ASSERT (m_pszChar!=NULL);

   inFrom;

   while ( inFrom < GetLength() )
   {
      if ( iswspace( At ( inFrom ) ) ) return true;
      inFrom ++;
   }
   return false;
}


/*@ XString::GetLength()
@group Get
@remarks Returns the length of the string!
*/
XSIZE_T XString::GetLength() const
{ return m_Length; }


/*@ XString::GetBuffer(XSIZE_T Size)
@group Caster
@remarks This is the most dangerous function, because it allows you access to the heart of XString, the
char-buffer! Don't use this function for dircet manipulations of the buffer! This function has
another job to do!

The job of this method is to work as a caster, when some awefull and dirty C-functions needs a
char-pointer, like sprintf. Size is the size you initialize the string, take care that the size
is great enough!

Don't forget to call the ReleaseBuffer()-Function after GetBuffer()! ReleaseBuffer correct the length
of the string.

The Size is exactly the size of chars in the buffer, the byte for the zero-byte is automatically added!

See the example!
@example
XString a;

double pi=3.14;

sprintf(a.GetBuffer(100), "Pi = %2.3f", pi); a.ReleaseBuffer();
*/
char* XString::GetBuffer(XSIZE_T Size)
{
  if (Size==0)
  {
     return m_pszChar;
  }


  char *temp;

  temp = (char *) calloc(sizeof(char), (size_t) Size+1);
  X_ASSERT (temp != NULL);

  XSIZE_T MinLength;
  if (Size>m_Length) MinLength = m_Length; else MinLength = Size;

  memcpy(temp, m_pszChar, MinLength);

  free(m_pszChar);
  m_pszChar = temp;
  m_Length = Size;

  return m_pszChar;
}

/*@ XString::GetBuffer(int Size=0)
@remarks Sometimes an int is available for the length..
*/
char* XString::GetBuffer(int Size)
{ X_ASSERT (Size>=0);
  return GetBuffer((XSIZE_T) Size);
}


/*@ XString::ReleaseBuffer(XSIZE_T Length)
@group Caster
@remarks After getting the buffer with GetBuffer, and after setting the size of the string to Size, this function
correct the size of the buffer, so that the size of the buffer is equal to the length of the string.
Don't use any other function after GetBuffer, before not calling ReleaseBuffer!!!!!!!!!!
*/
int XString::ReleaseBuffer(XSIZE_T Length)
{
  m_isNULL = false;

  if (Length==0) Length = strlen(m_pszChar);

  X_ASSERT (Length<=m_Length);

  char *temp = (char *) malloc(Length+1);

  memcpy(temp, m_pszChar, Length);

  temp[Length] = 0;

  free(m_pszChar);
  m_pszChar = temp;
  m_Length = Length;

  X_ASSERT(m_Length == strlen(m_pszChar));

  return Length;
}


BOOL XString::SetBlockSize(int inBlockSize)
{
   if (inBlockSize > 0)
      m_nBlockSize = inBlockSize;
   else return false;
   // todo: alte Block evtl neu allokieren!
   return false;
}

int XString::GetBlockSize() const
{
   return m_nBlockSize;
}


char* XString::Malloc(XSIZE_T i_Size)
{
   return (char*) malloc (i_Size);
}

/*@ XString::Strip(int inWhere, char inChar)
@group Enhanced
@returns Number of removed chars
@remarks Strip removes all inChars at the beginning (inWhere=XLEFT), at the end (inWhere=XRIGHT), at both ends
(inWhere=XBOTH) or removes all inChars (inWhere=XALL) from the string!
If you have to remove substring, use Replace() instead!
*/
int XString::Strip(int inWhere, char inChar)
{
  m_isNULL = false;

  X_ASSERT(m_pszChar != NULL);
  if (m_Length == 0) return 0;

  XSIZE_T First=0, Last=m_Length;  // Last 1index (>0)...

  if (inWhere != XRIGHT) // then strip left
  {
    while ((First<m_Length) && (m_pszChar[First] == inChar)) First++;

    if (First==m_Length) // Stripp all
    { free(m_pszChar);
      m_pszChar = (char *) malloc(1);
      X_ASSERT(m_pszChar != NULL);
      m_pszChar[0] = '\0';
      m_Length = 0;

      X_ASSERT(m_Length == strlen(m_pszChar));

      return Last;
    }
  }

  if (inWhere != XLEFT) // then strip right
  {

    while ((Last>0) && (m_pszChar[Last-1] == inChar))
    {  Last--; }
    if (Last==0) // Stripp all
    { free(m_pszChar);
      m_pszChar = (char *) malloc(1);
      X_ASSERT(m_pszChar != NULL);
      m_pszChar[0] = '\0';
      Last = m_Length;
      m_Length = 0;

      X_ASSERT(m_Length == strlen(m_pszChar));

      return Last;
    }
  }

  char *stripped;

  if (inWhere != XALL) // then  build result string
  {
    if ((First==0) && (Last==m_Length)) // then nothing to do
      return 0;

    stripped = (char *) malloc(Last-First+1);
    X_ASSERT (stripped != NULL);
    memcpy(stripped, m_pszChar+First, Last-First);

    stripped[Last-First] = 0;
    free(m_pszChar);
    m_pszChar = stripped;
    XSIZE_T ret=m_Length;
    m_Length = Last-First;

    X_ASSERT(m_Length == strlen(m_pszChar));

    return ret-m_Length;
  }

  // inWhere == ALL
  stripped = (char *) malloc(Last-First+1);
  X_ASSERT (stripped != NULL);

  XSIZE_T index=First, strindex=0;    // 0-index
  while (index<Last)
  { if (m_pszChar[index] != inChar) // else ignore this char
    { stripped[strindex] = m_pszChar[index];
      strindex++;
    }
    index++;
  }

  free(m_pszChar);
  XSIZE_T oldL=m_Length;
  m_pszChar = (char *) malloc(strindex+1);
  X_ASSERT (m_pszChar != NULL);
  memcpy(m_pszChar, stripped, strindex);
  m_pszChar[strindex] = 0;

  free(stripped);
  m_Length = strindex;

  X_ASSERT(m_Length == strlen(m_pszChar));
  return oldL-m_Length;
}

/*@ XString::StripWhitespaces ( int inWhere = XBOTH )
@group Enhanced
@returns Number of removed chars
@remarks Strip whitespaces ( Space, Tab, Linefeed, Carriage return )
        like function "Strip"
*/
int XString::StripWhitespaces ( int inWhere )
{

  setlocale( LC_ALL, "C" );
  X_ASSERT ( !iswspace((wint_t)'§') ); // sonst totaler Mist!

  m_isNULL = false;

  X_ASSERT(m_pszChar != NULL);
  if (m_Length == 0) return 0;

  XSIZE_T First=0, Last=m_Length;  // Last 1index (>0)...



  if (inWhere != XRIGHT) // then strip left
  {
    while ((First<m_Length) && (iswspace(m_pszChar[First])))
   {

      First++;
   }

    if (First==m_Length) // Stripp all
    { free(m_pszChar);
      m_pszChar = (char *) malloc(1);
      X_ASSERT(m_pszChar != NULL);
      m_pszChar[0] = '\0';
      m_Length = 0;

      X_ASSERT(m_Length == strlen(m_pszChar));

      return Last;
    }
  }

  if (inWhere != XLEFT) // then strip right
  {

    while ((Last>0) && (iswspace(m_pszChar[Last-1])))
    {  Last--; }
    if (Last==0) // Stripp all
    { free(m_pszChar);
      m_pszChar = (char *) malloc(1);
      X_ASSERT(m_pszChar != NULL);
      m_pszChar[0] = '\0';
      Last = m_Length;
      m_Length = 0;

      X_ASSERT(m_Length == strlen(m_pszChar));

      return Last;
    }
  }

  char *stripped;

  if (inWhere != XALL) // then  build result string
  {
    if ((First==0) && (Last==m_Length)) // then nothing to do
      return 0;

    stripped = (char *) malloc(Last-First+1);
    X_ASSERT (stripped != NULL);
    memcpy(stripped, m_pszChar+First, Last-First);

    stripped[Last-First] = 0;
    free(m_pszChar);
    m_pszChar = stripped;
    XSIZE_T ret=m_Length;
    m_Length = Last-First;

    X_ASSERT(m_Length == strlen(m_pszChar));

    return ret-m_Length;
  }

  // inWhere == ALL
  stripped = (char *) malloc(Last-First+1);
  X_ASSERT (stripped != NULL);

  XSIZE_T index=First, strindex=0;    // 0-index
  while (index<Last)
  { if (!iswspace(m_pszChar[index])) // else ignore this char
   { stripped[strindex] = m_pszChar[index];
      strindex++;
    }
    index++;
  }

  free(m_pszChar);
  XSIZE_T oldL=m_Length;
  m_pszChar = (char *) malloc(strindex+1);
  X_ASSERT (m_pszChar != NULL);
  memcpy(m_pszChar, stripped, strindex);
  m_pszChar[strindex] = 0;

  free(stripped);
  m_Length = strindex;

  X_ASSERT(m_Length == strlen(m_pszChar));
  return oldL-m_Length;
}





/*@ XString::DelSubString(XString inSubString)
@group Set
@remarks Deletes the first place occurrence of inSubString
*/
int XString::DelSubString(XString inSubString)
{
  m_isNULL = false;
  X_ASSERT(inSubString.m_pszChar!=NULL);
  X_ASSERT(inSubString.m_Length>0);

  XSIZE_T pos;
  if (Find(pos, inSubString)) return DelSubString(pos, inSubString.m_Length);
  return -1;
}

/*@ XString::DelSubString
@group Set
@remarks Deletes inCount-chars from inFrom (Zero-Index!).
*/
int XString::DelSubString(XSIZE_T inFrom, XSIZE_T inCount)
{
  m_isNULL = false;
  X_ASSERT(inFrom<m_Length);
  if (inCount==0) return -1;

  if (inFrom+inCount>m_Length) inCount=m_Length-inFrom;

  char *temp = (char *) malloc(m_Length-inCount+1);
  X_ASSERT(temp!=NULL);

  memcpy(temp, m_pszChar, inFrom);
  memcpy(temp+inFrom, m_pszChar+inFrom+inCount, m_Length-(inCount+inFrom));
  temp[m_Length-inCount] = '\0';
  m_Length -= inCount;
  free(m_pszChar);
  m_pszChar = temp;

  X_ASSERT(m_Length == strlen(m_pszChar));

  return 0;
}

/*@ XString::Replace(XString inSearch, XString inReplace, int inTimes, XSIZE_T inFrom, XSIZE_T inTo)
@group Enhanced
@returns Number of replacements
@parameters inSearch is the substring to search, inReplace is the string wich replaces the searchstring, the substring
is max. inTimes replaced and inFrom and inTo mark the scope.
@remarks This is one of the most powerful methods of XString. It works as the Search-Replace-Function of you editor.
Of course, the search- and the replace-substring must NOT have the same length! The methods is working very fast,
because it first searches the substring, allocs new memory by calculing the new size, and then a new string is build.
When the new string is build, there is no more search necessary (only if the replacement-string is greater then the
searchstring and the length of the searchstring is smaller then the size of a XSIZE_T-type!).

In this moment I'm working on another Replace-method: Replace(BadEnglish, CorrectEnglish, everywhere in this docu...) ;-)
*/
int XString::Replace(XString inSearch, XString inReplace, int inTimes, XSIZE_T inFrom, XSIZE_T inTo, BOOL inInCase)
{
  m_isNULL = false;

  XSIZE_T pos=inFrom;
  int count=0;
  char *temp;


  if (inTo==0) inTo=GetLength();
  if (inTo<inFrom)
  { count = inTo; inTo = inFrom; inFrom = count; count =0; }
  if (inFrom>inTo) return 0;


  X_ASSERT(m_Length == strlen(m_pszChar));

  if (inSearch.GetLength() == inReplace.GetLength())
  {
    while (Find(pos,inSearch, pos, inTo, inInCase))
    { count++;
      memcpy(&m_pszChar[pos], inReplace.m_pszChar, inReplace.GetLength());
      if (count==inTimes) break;
      pos += inSearch.GetLength();
    }
  }
  else
  if (inSearch.GetLength() > inReplace.GetLength())
  {
    while (Find(pos,inSearch, pos, inTo, inInCase))
    { count++;
      memcpy(&m_pszChar[pos], inReplace.m_pszChar, inReplace.GetLength());
      memmove(&m_pszChar[pos+inReplace.GetLength()],
              &m_pszChar[pos+inSearch.GetLength()], GetLength()+1-(pos+inSearch.GetLength()));
      if (count==inTimes) break;
      pos += inReplace.GetLength();
    }
    if (count>0)
    {
    temp = (char *) malloc(GetLength()+1-count*(inSearch.GetLength()-inReplace.GetLength()));
    memcpy(temp, m_pszChar,GetLength()+1-count*(inSearch.GetLength()-inReplace.GetLength()));
    free(m_pszChar);
    m_pszChar = temp;
    m_Length = GetLength()-count*(inSearch.GetLength()-inReplace.GetLength());
    }
  }
  else // inSearch.GetLength() < inReplace.GetLength()
  {
    XSIZE_T *posptr, firstpos, lastpos;
    unsigned char nextNull=0, offsets=0;

    if (inSearch.GetLength()>=sizeof(XSIZE_T))    // ptrlist in string
    {

      while (Find(pos,inSearch, pos, inTo, inInCase))
      { count++;
        if (count==1) firstpos = pos;
        else *posptr = pos;

        posptr = (XSIZE_T*) (&m_pszChar[pos]);
        if (count==inTimes) break;
        pos += inSearch.GetLength();
      }

    }
    else
    { offsets = 1;
      while (Find(pos, inSearch, pos, inTo, inInCase))
      {  count++;
         if (count==1) firstpos=pos;
       if (count==inTimes) break; // 97-03-14: Fehler behoben: inTimes wurde hier ignoriert
         pos += inSearch.GetLength();
      }
    }
    if (count>0)
    { XSIZE_T  nextpos = 0;
     int i=0;
      lastpos = 0;
      pos = firstpos;

      temp = (char *) malloc(GetLength()+1+count*(inReplace.GetLength()-inSearch.GetLength()));

      while (i<count)
      {
       if (offsets)   Find(nextpos, inSearch, pos+inSearch.GetLength(), inTo, inInCase);
       else           nextpos = *( (XSIZE_T*) (m_pszChar+pos) ) ;

        if (i>0)
        {  memcpy(&temp[inSearch.GetLength()+lastpos+(inReplace.GetLength()-inSearch.GetLength())*i ],
               &m_pszChar[lastpos+inSearch.GetLength()],
               pos-(lastpos+inSearch.GetLength()));                    // ...RRRxxxx...
           memcpy(&temp[inSearch.GetLength()+pos+(inReplace.GetLength()-inSearch.GetLength())*i
                         -inSearch.GetLength()],
                inReplace.m_pszChar, inReplace.GetLength()+1);           //...RRRxxxxRRR
        }
        else
        { memcpy(temp, m_pszChar, pos);
          memcpy(&temp[pos],inReplace.m_pszChar, inReplace.GetLength()+1);
        }


        lastpos = pos;
        pos = nextpos;
        i++;
      }

      memcpy(&temp[inSearch.GetLength()+lastpos+(inReplace.GetLength()-inSearch.GetLength())*i],    /// RRRxxxxx
             &m_pszChar[lastpos+inSearch.GetLength()],
             GetLength()-(lastpos+inSearch.GetLength())+1);

      XSIZE_T A1, A2;
      A1 =inSearch.GetLength()+lastpos+(inReplace.GetLength()-inSearch.GetLength())*i+GetLength()-(lastpos+inSearch.GetLength());
      A2 = GetLength()+count*(inReplace.GetLength()-inSearch.GetLength());

X_ASSERT(A1==A2);


      free(m_pszChar);
      m_pszChar = temp;
      m_Length = GetLength()+count*(inReplace.GetLength()-inSearch.GetLength());

    }
  }
  return count;
}


/*@ XString::MakeUpper()
@group Other conversions
@remarks a-z -> A-Z (with _strupr), plus: ä,ö,ü,ß -> Ä, Ö, Ü, SS
*/
void XString::MakeUpper()
{
   m_isNULL = false;

  X_ASSERT(m_pszChar != NULL);
  X_ASSERT(m_Length == strlen(m_pszChar));

  strupr(m_pszChar);

  XSIZE_T pos;
  for (pos=0; pos<m_Length; pos++) if (m_pszChar[pos] == 'ä') m_pszChar[pos] = 'Ä';
  for (pos=0; pos<m_Length; pos++) if (m_pszChar[pos] == 'ö') m_pszChar[pos] = 'Ö';
  for (pos=0; pos<m_Length; pos++) if (m_pszChar[pos] == 'ü') m_pszChar[pos] = 'Ü';

  Replace("ß", "SS");

}

/*@ XString::MakeUpper()
@group Other conversions
@remarks A-Z -> a-z (with _strupr), plus: Ä, Ö, Ü -> ä, ö, ü
*/
void XString::MakeLower()
{
   m_isNULL = false;

   X_ASSERT(m_pszChar != NULL);
  X_ASSERT(m_Length == strlen(m_pszChar));

  strlwr(m_pszChar);

  XSIZE_T pos;
  for (pos=0; pos<m_Length; pos++) if (m_pszChar[pos] == 'Ä') m_pszChar[pos] = 'ä';
  for (pos=0; pos<m_Length; pos++) if (m_pszChar[pos] == 'Ö') m_pszChar[pos] = 'ö';
  for (pos=0; pos<m_Length; pos++) if (m_pszChar[pos] == 'Ü') m_pszChar[pos] = 'ü';
}


const char __Xlow[ ] = "éâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥Pƒáíóúñ";
const char __Xupp[ ] = "ÉÂÄÀÅÇÊEÈIÎÌÄÅÉÆÆÔÖÒÛÙYÖÜ¢£¥PƒÁÍÓÚÑ";

char XString::UpperChar(char inChar)
{
   if (inChar<'a') return inChar;         // Großbuchstaben und Steuerzeichen
   if (inChar>=165) return inChar;         // Grafikzeichen
   if (inChar<='z') return inChar - 32;   // a..z -> A--Z
    if (inChar<'ü') return inChar;          // {...Ç
   return __Xupp[inChar-'é'];
}



/*****************************************************
FRIEND HELPERS:
********************************************************/

// FriendOperatoren:

int operator ==(const XString &inS1, const XString &inS2)
{ X_ASSERT(inS1.m_pszChar!=NULL);
  X_ASSERT(inS2.m_pszChar!=NULL);

  return strcmp(inS1.m_pszChar, inS2.m_pszChar)==0;
}

int operator ==(const XString &inS1, const char *pszChar2)
{
   X_ASSERT(inS1.m_pszChar!=NULL);
    X_ASSERT(pszChar2!=NULL);

   return strcmp(inS1.m_pszChar, pszChar2)==0;
}

int operator ==(const char *pszChar1, const XString &inS2)
{
   X_ASSERT(pszChar1!=NULL);
   X_ASSERT(inS2.m_pszChar!=NULL);

   return strcmp(pszChar1, inS2.m_pszChar)==0;
}

int operator <(const XString &inS1, const XString &inS2)
{ X_ASSERT(inS1.m_pszChar!=NULL);
  X_ASSERT(inS2.m_pszChar!=NULL);

  return strcmp(inS1.m_pszChar, inS2.m_pszChar)<0;
}

int operator <(const XString &inS1,const char *pszChar2)
{
   X_ASSERT(inS1.m_pszChar!=NULL);
    X_ASSERT(pszChar2!=NULL);

   return strcmp(inS1.m_pszChar, pszChar2)<0;
}

int operator <( const char *pszChar1, const XString &inS2)
{
   X_ASSERT(pszChar1!=NULL);
   X_ASSERT(inS2.m_pszChar!=NULL);

   return strcmp(pszChar1, inS2.m_pszChar)<0;
}


int operator !=(const XString &inS1, const XString &inS2)
{   return !(inS1==inS2);      }

int operator !=(const XString &inS1,  const char *pszChar2)
{   return !(inS1 == pszChar2); }

int operator !=(const char *pszChar1, const XString &inS2)
{   return !(pszChar1 == inS2);   }

int operator >(const XString &inS1, const XString &inS2)
{   return (inS2 < inS1);   }

int operator >(const XString &inS1, const char *pszChar2)
{   return (pszChar2 < inS1);   }

int operator >( const char *pszChar1, const XString &inS2)
{   return (inS2 < pszChar1 );   }

int operator <=(const XString &inS1, const XString &inS2)
{   return !(inS2<inS1);   }

int operator <=(const XString &inS1,  const char *pszChar2)
{   return !(pszChar2 < inS1);   }

int operator <=(const char *pszChar1, const XString &inS2)
{   return !(inS2 < pszChar1);   }

int operator >=(const XString &inS1, const XString &inS2)
{   return !(inS1<inS2);      }

int operator >=(const XString &inS1, const char *pszChar2)
{   return !(inS1 < pszChar2);   }

int operator >=(const char *pszChar1, const XString &inS2)
{   return !(pszChar1 < inS2);   }




///////////////////////// July 1997
int operator ==(const XString &inS1, char *pszChar2)
{ return inS1 == (const char*) pszChar2; }
int operator !=(const XString &inS1, char *pszChar2)
{ return inS1 != (const char*) pszChar2; }
int operator <=(const XString &inS1, char *pszChar2)
{ return inS1 <= (const char*) pszChar2; }
int operator >=(const XString &inS1, char *pszChar2)
{ return inS1 >= (const char*) pszChar2; }
int operator >(const XString &inS1, char *pszChar2)
{ return inS1 > (const char*) pszChar2; }
int operator <(const XString &inS1, char *pszChar2)
{ return inS1 < (const char*) pszChar2; }

int operator ==(char *pszChar1, const XString &inS2)
{ return (const char*) pszChar1 == inS2; }
int operator !=(char *pszChar1, const XString &inS2)
{ return (const char*) pszChar1 != inS2; }
int operator <=(char *pszChar1, const XString &inS2)
{ return (const char*) pszChar1 <= inS2; }
int operator >=(char *pszChar1, const XString &inS2)
{ return (const char*) pszChar1 >= inS2; }
int operator >(char *pszChar1, const XString &inS2)
{ return (const char*) pszChar1 > inS2; }
int operator <(char *pszChar1, const XString &inS2)
{ return (const char*) pszChar1 < inS2; }
/////////////////////////











// Friend-Functionen:

XString operator +(const XString Str1, const char *pszChar2)
{
  X_ASSERT(Str1.m_pszChar!=NULL);
  X_ASSERT(pszChar2!=NULL);

  X_ASSERT(Str1.m_Length == strlen(Str1.m_pszChar));

  XString Res;
  XSIZE_T C2Length=strlen(pszChar2);

  Res.m_Length = Str1.m_Length+C2Length;
  free(Res.m_pszChar);
  Res.m_pszChar = (char *) malloc(Res.m_Length+1);

  X_ASSERT(Res.m_pszChar != NULL);

  memcpy(Res.m_pszChar, Str1.m_pszChar, Str1.m_Length);
  memcpy(&Res.m_pszChar[Str1.m_Length], pszChar2, C2Length+1);

  X_ASSERT(Res.GetLength() == strlen(Res.m_pszChar));

  return Res;
}


XString operator +(const char *pszChar1, const XString Str2)
{
  X_ASSERT(Str2.m_pszChar!=NULL);
  X_ASSERT(pszChar1!=NULL);

  X_ASSERT(Str2.m_Length == strlen(Str2.m_pszChar));

  XString Res;
  XSIZE_T C1Length = strlen(pszChar1);

  Res.m_Length = Str2.m_Length+C1Length;
  free(Res.m_pszChar);
  Res.m_pszChar = (char *) malloc(Res.m_Length+1);

  X_ASSERT(Res.m_pszChar != NULL);

  memcpy(Res.m_pszChar, pszChar1, C1Length);
  memcpy(&Res.m_pszChar[C1Length], Str2.m_pszChar, Str2.m_Length+1);

  X_ASSERT(Res.m_Length == strlen(Res.m_pszChar));

  return Res;
}


XString operator +(const XString& Str1, const XString& Str2)
{
  X_ASSERT(Str2.m_pszChar!=NULL);
  X_ASSERT(Str1.m_pszChar!=NULL);

  X_ASSERT(Str1.m_Length == strlen(Str1.m_pszChar));
  X_ASSERT(Str2.m_Length == strlen(Str2.m_pszChar));

  XString Res;

  Res.m_Length = Str2.m_Length+Str1.m_Length;
  free(Res.m_pszChar);
  Res.m_pszChar = (char *) malloc(Res.m_Length+1);

  X_ASSERT(Res.m_pszChar != NULL);

  memcpy(Res.m_pszChar, Str1.m_pszChar, Str1.m_Length);
  memcpy(&Res.m_pszChar[Str1.m_Length], Str2.m_pszChar, Str2.m_Length+1);

  X_ASSERT(Res.GetLength() == strlen(Res.m_pszChar));

  return Res;
}

/* Vereinfachung:
   x==y und x<y definiert, ansonsten:
   x!=y : !(x==y)
   x>y  : y<x
   x<=y : !(y<x)
   x>=< : !(x<y)
*/

/////////////////////////////////////////////////
// streams:


ostream& operator<<(ostream& outStream, const XString& outString)
{
   return outStream << outString.m_pszChar;
}

istream& operator>>(istream& inStream, XString& inString)
{
   char buf[100];
   char c;
   while (inStream.get(c) && (c!= '\n'))
   {
      inString += c;
      inStream.get (buf, 100, '\n');
      inString += buf;

   }
   return inStream;
}