emximp -o rexx2emx.a rexx.lib
ar r rexx2emx.a
ar s rexx2emx.a
emxomf -s -l rexx2emx.a
