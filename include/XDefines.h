#ifndef __XDEFINES_H__
#define __XDEFINES_H__


///////////////#define DEBUG  // release: clear this


#ifndef NULL
  #define NULL 0
#endif

#define X_ERR -1
//#define X_OK  0

#ifdef DEBUG
  #include "assert.h"
  #define ASSERT(x) assert(x)
#else
  #define ASSERT(x)
#endif

#ifndef ABSTRACT
  #define ABSTRACT = 0
#endif

#endif
