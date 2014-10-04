emximp -o java2emx.a jempo10.lib
ar r java2emx.a
ar s java2emx.a
emxomf -s -l java2emx.a
