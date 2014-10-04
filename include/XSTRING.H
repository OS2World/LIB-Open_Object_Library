/*
   XString

   Version July 1997

   (C) 1996 Jens von Pilgrim
       1997 Jens von Pilgrim

   jpilgri@gwdg.de
*/


/*@
@class XString
@symbol XString
@type overview
@remarks XString implements a powerful String-Class!
@history
16.08.1995
- FastFind noch nicht ausgereift!

26.03.1996
- XString arbeitet jetzt schneller (Länge wird gespeichert),
  nicht mehr "direkt" kompatibel zu pszChar!
- assert-Abfrage wird nur bei definierten DEBUG durchgeführt

27.03.1996
- neuer () Operator: Liefert char-Zeiger zurück,
  optionaler Parameter: Count analog zu Left()
- Cast-Operatoren für
  XString -> int, const int, long, const long, double, const double
  (umbenannt nach MakeXXX, s.u.)
- Konstruktoren für Zahlen: XString(int|long [,radix])

01.04.1996:
- Copy-Konstruktor ohne Speicherreservierung für den Char-String
  => vorsicht! Schnell aber gefährlich! Daher PROTECTED!

29.05.1996:
- Strip-Funktion
- [] - Operator
- DelSubString

05.06.1996:
- Replace
- Bug in Find behoben ( z.B. ./ in .../ nicht gefunden)!

  06.06.1996:
- Umstellung: signed nach XSIZE_T bei
  Find und m_Length (und allgemein wo From oder To oder so auftauchen!)
  => Umsetllung von Find:
  statt: Positon = Find(Was, Von, Bis) nun:
  gefunden = Find(Pos, Was, Von, Bis)
  Ergebnis: BOOL (true: Found, false: Not found)

07.06.1996:
- operatoren <, <=, >, <= funktionieren jetzt RICHTIG!

06.08.1996:
- Anpassungen für Visual C++ 4.0 (operatoren als Friends...)
- MakeUpper, MakeLower

12.08.1996:
- InCase - Flag bei Find/FindRev/Replace
- neue Funktion UpperC

17.10.1006:
- diverse Anpassungen fuer Visual C++ 4.0 und XVT
- BOOL (und TRUE, FALSE) werden durch BOOL, true und false ersetzt

20.11.1996:
Anpasungen für VC4.0:
- Operatoren ==, <=, >=, <, > 3 mal vorhanden:
  XString-XString, lpszChar-XString, XString-lpszChar
- []-Operator 3-mal: [XSIZE_T], [ULONG], [int]

20.11.1996:
- []-Operator liefert Refernez auf Zeichen, so dass
  z.B.:  A[i]='x' möglich ist

01.02.1997:
- zusätzliche Find/FindRev-Funktion ohne
  Rückparameter der gefundenen Stelle

08.02.1997:
- Folgende Funktionen als const definiert:
  Find, FindRev, SFind, SFindRev, IsEmpty, StrCmp,
  Mid, Left, Right, GetLength
  caster: int, long, char etc...
- statt XBOOL jetzt: BOOL aus BOOL.h der STL
- neue Funktion: StripWhitespaces ( int where = XBOTH )

11.02.1997:
- stream-operatoren: << und >>

12.02.1997:
- neue Funktionalität:
  XString können "undefiniert" sein, also nicht initialisiert bzw. definiert NULL.
  Der Standardkonstruktor erzeugt einen "undefinierten" String, welcher sich
  aber voll kompatibel zur vorherigen Version zeigt.
- Folgende Funktionen sind daher neu:
  BOOL IsNULL() const;
  void MakeNULL();
- Zur leichterten Lesbarkeit wurde auch die
  "Empty-"Funktionalität überarbeitet:
   BOOL IsEmpty() const; // wie gehabt
   void MakeEmpty();

- sämtliche (potentielle) Schreibfunktionen haben als Nachbedingung IsNULL()==false
  (auch DelSubString, +=, MakeUpper, Strip,
  ReleaseBuffer, MakeEmpty)
  ABER: = kopiert Zustand!

13.03.1997:
- Umbenannt:
  sFind -> Search
  sFindRev -> SearchRev
- neue Funktionen:
  FindWhitespace
  SearchWhitespace

24.03,1997:
- NEUE TEMPLATE-CLASS: XSplitableString
- Split // by Separator
- Split // by Whitespaces
- GetTableDatas

02.04.1997:
- veränderte Syntax/Semantik von At()
  1) Syntax: char At()
  2) wenn laenge<=index => At()==0

09.07.1997:
- neue Suchfunktion: TurboSearch (vgl. ct' August 1997)
- Vorbereitet für eigene Malloc-Funktion, um Blockgröße anzugeben.
  Achtung: Funktionen sind noch nicht implementiert
  ( - GetBlockSize, SetBlockSize)
- folgende Fehler behoben:
  - Zuweisung A=A erzeugte bislang Fehler
  - Konstruktor XString(char) erzeugt Fehler bei XString('\0')
- XSplitableString nun in eigener Datei
- BOOL wird, wenn nicht definiert, als typedef gesetzt.
  Daher wird jetzt hier kein STL-header mehr benötigt, der typedef
  ist aber der gleiche - also eigentlich keine Probleme
- operator char* () geändert (keine Left-Funkionalität mehr, wie GetBuffer

****************************************************************************/


#ifndef _XSTRING_H_
#define _XSTRING_H_

#include "xinclude.h"
//#ifndef _MSC_VER
// #include <BOOL.h> // the STL BOOL.h: BOOL, true, false (as int)
/*#else
//#include <yvals.h>
#endif
*/


#include <iostream.h>


#ifdef _MSC_VER
#pragma warning ( disable : 4237 )
#endif


//#ifndef BOOL
//   typedef int BOOL;
//#endif

#ifndef XBOOL
   #define XBOOL BOOL
   #define XTRUE 1
   #define XFALSE 0
#endif

#ifndef true
   #define true 1
#endif

#ifndef false
   #define false 0
#endif


#ifndef NULL
  #define NULL 0
#endif

#ifndef XSIZE_T
  #define XSIZE_T unsigned long int
#endif

#ifndef XLEFT
  #define XALL 0
  #define XLEFT 1
  #define XRIGHT 2
  #define XBOTH 3
#endif

// TBO
#define _XSTRING_DECL_

#define _LINK_TO_
// #define _BUILD_DLL_

#ifdef _BUILD_DLL_
#undef _XSTRING_DECL_
  #define _XSTRING_DECL_ _declspec (dllexport)
#else
  #ifndef _LINK_TO_
    #undef _XSTRING_DECL_
    #define _XSTRING_DECL_ _declspec (dllimport)
  #endif
#endif


// TBO
#if !defined( __WATCOMC__ )
class _XSTRING_DECL_ XString: public XObject
#else
class _export_ XString: public XObject
#endif
{
public:
   XString   ();
    XString   (const XString &aString);

    XString (char aChar);
   XString (char *pszChar);
    XString (const char *pszChar);

    XString (int aNumber, int Radix = 10);
    XString (long aNumber, int Radix = 10);

    //virtual
   ~XString ();


public:

/////////////////////////
//  cast - operators

   int      MakeInt() const;
    long   MakeLong() const;
    double   MakeDouble() const;

/*
   operator const char *const() { return m_pszChar; }//const
    operator char * () const { return m_pszChar; }
operator const char * () { return m_pszChar; }
   const char *const operator() () const { return m_pszChar; }
*/
      operator char* () const { return((char*) m_pszChar); }
//TBO
#ifndef __WATCOMC__
      operator const char* () const { return((const char*) m_pszChar); }
#endif


   const char *const operator() (XSIZE_T inCount); // wie Left

    char& operator[] (XSIZE_T inZeroIndex) const; // neu 20.11
   char& operator[] (int inZeroIndex) const;     // neu 20.11
    char At(XSIZE_T inZeroIndex) const;

////////////////////////
//  comparison (- operators)
    XString operator =(const XString &aString);
    virtual int StrCmp(const XString &inThen) const;

////////////////////////
//   Attributs
   virtual  XSIZE_T GetLength() const;

////////////////////////
//  Buffer-Functions
    char* GetBuffer(int Size);
    char* GetBuffer(XSIZE_T Size=0);
    int ReleaseBuffer(XSIZE_T Length=0);

   BOOL SetBlockSize(int inBlockSize);
   int GetBlockSize() const;

////////////////////////
//   Operationen

    XString operator +=(const XString &Str);
    XString operator +=(const char *Str);

   BOOL IsEmpty() const;
   void MakeEmpty();

   BOOL IsNULL() const;
   void MakeNULL();

    void MakeUpper();
   void MakeLower();

    static char UpperChar(char inChar);


////////////////////////
//   SubString-Functions
    virtual XString Mid(XSIZE_T From, XSIZE_T Count) const;
    virtual XString Left(XSIZE_T Count) const;
    virtual XString Right(XSIZE_T Count) const;

    const XString Insert(const XString& , XSIZE_T);

   int DelSubString(XString inSubString);
    int DelSubString(XSIZE_T inFrom, XSIZE_T inCount=1);


////////////////////////////////
//  Search & Replace - Functions

    BOOL Find(XSIZE_T &outPos, const XString &SubString, XSIZE_T From = 0, XSIZE_T inTo = 0, BOOL inInCase=false) const;
    BOOL FindRev(XSIZE_T &outPos, const XString &SubString, XSIZE_T From = 0, XSIZE_T inTo = 0, BOOL inInCase=false) const;

//   BOOL TurboFind(XSIZE_T &outPos, const XString &SubString, XSIZE_T From = 0, XSIZE_T inTo = 0, BOOL inInCase=false, BOOL inInit = true) const;

   BOOL Search(const XString &SubString, XSIZE_T From = 0, XSIZE_T inTo = 0, BOOL inInCase=false) const;
    BOOL SearchRev(const XString &SubString, XSIZE_T From = 0, XSIZE_T inTo = 0, BOOL inInCase=false) const;

   BOOL FindWhitespace ( XSIZE_T &outPos, XSIZE_T inFrom = 0, XSIZE_T inTo = 0) const;
   BOOL SearchWhitespace ( XSIZE_T inFrom = 0, XSIZE_T inTo = 0) const;

   BOOL FindAlpha ( XSIZE_T &outPos, XSIZE_T inFrom = 0, XSIZE_T inTo = 0) const;
   BOOL FindNonAlpha ( XSIZE_T &outPos, XSIZE_T inFrom = 0, XSIZE_T inTo = 0) const;
   BOOL SearchAlpha( XSIZE_T inFrom = 0, XSIZE_T inTo = 0) const;
   BOOL SearchNonAlpha ( XSIZE_T inFrom = 0, XSIZE_T inTo = 0) const;

   int Replace(XString inSearch, XString inReplace, int inTimes=XALL, XSIZE_T inFrom=0, XSIZE_T inTo=0, BOOL inInCase=false);

/////////////////////////////////
//  Format-Functions

    int Strip(int inWhere = XBOTH, char inChar = ' ');
   int StripWhitespaces ( int inWhere = XBOTH );



/////////////////////////
// Friends

   /////////////////////////
   //  streams:
   _XSTRING_DECL_ friend ostream& operator<<(ostream&, const XString& );
   _XSTRING_DECL_ friend istream& operator>>(istream&, XString& );

   /////////////////////////
   // Compare (weitere Compare-Funktionen stützen sich
   // auf == und < ab, müssen also keine Friends sein!
   _XSTRING_DECL_ friend XString operator +(const XString Str1, const char *pszChar2);
   _XSTRING_DECL_ friend XString operator +(const char *pszChar1, const XString Str2);
    _XSTRING_DECL_ friend XString operator +(const XString& Str1, const XString& Str2);

    _XSTRING_DECL_ friend int operator ==(const XString &inS1, const XString &inS2) ;
   _XSTRING_DECL_ friend int operator ==(const XString &inS1, const char *pszChar2) ;
   _XSTRING_DECL_ friend int operator ==(const char *pszChar1, const XString &inS2) ;

   _XSTRING_DECL_ friend int operator <(const XString &inS1, const XString &inS2);
   _XSTRING_DECL_ friend int operator <(const XString &inS1, const char *pszChar2);
   _XSTRING_DECL_ friend int operator <(const char *pszChar1, const XString &inS2);

////////////////
protected:
   XString (char *pszChar, int duplicate);
   char* Malloc(XSIZE_T i_Size);

///////////
protected:
    char   *m_pszChar;
    XSIZE_T m_Length;
   BOOL   m_isNULL;
   int      m_nBlockSize;

};


////////////////////////////////////////////////
// Friends - Prototypes

_XSTRING_DECL_ ostream& operator<<(ostream&, const XString& );
_XSTRING_DECL_ istream& operator>>(istream&, XString& );

_XSTRING_DECL_ XString operator +(const XString Str1, const char *pszChar2);
_XSTRING_DECL_ XString operator +(const char *pszChar1, const XString Str2);
_XSTRING_DECL_ XString operator +(const XString& Str1, const XString& Str2);

_XSTRING_DECL_ int operator ==(const XString &inS1, const XString &inS2) ;
_XSTRING_DECL_ int operator !=(const XString &inS1, const XString &inS2) ;
_XSTRING_DECL_ int operator <=(const XString &inS1, const XString &inS2);
_XSTRING_DECL_ int operator >=(const XString &inS1, const XString &inS2);
_XSTRING_DECL_ int operator >(const XString &inS1, const XString &inS2);
_XSTRING_DECL_ int operator <(const XString &inS1, const XString &inS2);

_XSTRING_DECL_ int operator ==(const XString &inS1, const char *pszChar2) ;
_XSTRING_DECL_ int operator !=(const XString &inS1, const char *pszChar2) ;
_XSTRING_DECL_ int operator <=(const XString &inS1, const char *pszChar2);
_XSTRING_DECL_ int operator >=(const XString &inS1, const char *pszChar2);
_XSTRING_DECL_ int operator >(const XString &inS1, const char *pszChar2);
_XSTRING_DECL_ int operator <(const XString &inS1, const char *pszChar2);

_XSTRING_DECL_ int operator ==(const char *pszChar1, const XString &inS2) ;
_XSTRING_DECL_ int operator !=(const char *pszChar1, const XString &inS2) ;
_XSTRING_DECL_ int operator <=(const char *pszChar1, const XString &inS2);
_XSTRING_DECL_ int operator >=(const char *pszChar1, const XString &inS2);
_XSTRING_DECL_ int operator >(const char *pszChar1, const XString &inS2);
_XSTRING_DECL_ int operator <(const char *pszChar1, const XString &inS2);


_XSTRING_DECL_ int operator ==(const XString &inS1, char *pszChar2) ;
_XSTRING_DECL_ int operator !=(const XString &inS1, char *pszChar2) ;
_XSTRING_DECL_ int operator <=(const XString &inS1, char *pszChar2);
_XSTRING_DECL_ int operator >=(const XString &inS1, char *pszChar2);
_XSTRING_DECL_ int operator >(const XString &inS1, char *pszChar2);
_XSTRING_DECL_ int operator <(const XString &inS1, char *pszChar2);

_XSTRING_DECL_ int operator ==(char *pszChar1, const XString &inS2) ;
_XSTRING_DECL_ int operator !=(char *pszChar1, const XString &inS2) ;
_XSTRING_DECL_ int operator <=(char *pszChar1, const XString &inS2);
_XSTRING_DECL_ int operator >=(char *pszChar1, const XString &inS2);
_XSTRING_DECL_ int operator >(char *pszChar1, const XString &inS2);
_XSTRING_DECL_ int operator <(char *pszChar1, const XString &inS2);



#endif



