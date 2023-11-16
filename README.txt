##############################    COMPILACION    ##############################

* Para generar el ejecutable, se debe utilizar el comando 'make' desde la
consola dentro del directorio del proyecto, pudiendo optar por

                        1)       make parser
                        2)    make testgenerator
                        3)        make tests
                        4)           make

* 1) Compila main.c parser.c trie.c GLIST.c GQUEUES.c
     Crea el ejecutable parser

* 2) Compila testgenerator.c
     Crea el ejecutable testgenerator
     
* 3) Compila tests.c parser.c trie.c GLIST.c GQUEUES.c  
     Crea el ejecutable tests
   
* 4) Es equivalente a make all o a ejecutar los comandos (1), (2) y (3)
     por separado.

###########################     EJECUCION PARSER     ##########################

* Para ejecutar el programa principal, se debera utilizar desde la consola
el comando

              ./parser <diccionario.txt> <frases.txt> [<salida.txt>]
                           
* IMPORTANTE! Los archivos <diccionario.txt> y <frases.txt> deben
encontrarse en los directorios 'dicts' y 'phrases' respectivamente, de otra 
manera se producira un error al intentar abrirlos.

* SALIDA. El tercer argumento, < salida.txt > es opcional. De no indicarse
explicitamente, se generara una salida con el nombre 'parsing_output.txt'.
Tanto en este caso como si se brinda un nombre, los archivos de salida se
almacenan en el directorio 'outputs'.

################################  TESTGENERATOR  ##############################

* Dentro del directorio del proyecto se encuentra un archivo test_generator.c.
Tras ser compilado como indica el apartado de compilacion, crea el ejecutable
testgenerator. Este programa es utilizado desde la consola con el siguiente 
comando

            ./testgenerator <dict.txt> <num_lineas> <largo_lineas>  

donde <dict.txt> es el nombre de un diccionario en la carpeta dicts.
      <num_lineas> es la cantidad de lineas a generar
      <largo_lineas> es el largo promedio de las lineas a generar
      
* De esta manera, tras su ejecucion, testgenerator crea un nuevo archivo txt
en el directorio 'phrases' que consta de <num_lineas> lineas de caracteres de
longitud entre [ <largo_lineas> - <largo_lineas> / 4,
               [ <largo_lineas> + <largo_lineas> / 4 ]
generadas aleatoriamente a partir de todos los posibles prefijos de <dict.txt>

#################################    TESTS    #################################

* Se incluyen algunos de los tests realizados para el programa en el programa
tests.c.

* Se ejecuta por consola tras su compilacion con make tests simplemente con el
comando 

                                   ./tests

* test_duhalde testea casos borde del parseo con un diccionario y listado de 
frases modelo.

* Los tests restantes son simples chequeos de funciones relevantes para el 
programa.

#################################   INFORME   #################################

* Una explicacion mas detallada de la estructura de datos y algoritmo propuestos
puede encontrarse en el archivo informe.pdf dentro de este mismo directorio.

* Alli tambien se encuentra un detalle sobre los casos de prueba adjuntados y
el generador de tests.

################################# ########### #################################


