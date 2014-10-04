/*
 * create an empty dll to use as a resource dll
 */

extern "C" {

   int __dll_initialize( void )
   {
        return 1;
   }


   int __dll_terminate( void )
   {
        return 1;
   }


}