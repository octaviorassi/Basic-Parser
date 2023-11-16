# Compilacion

Para generar el ejecutable, se debe utilizar el comando 'make' desde la
consola dentro del directorio del proyecto, pudiendo optar por

`make parser`

+ Compila main.c parser.c trie.c GLIST.c GQUEUES.c
+ Crea el ejecutable parser
  
`make testgenerator`
+ Compila testgenerator.c
+ Crea el ejecutable testgenerator
  
`make tests`
+ Compila tests.c parser.c trie.c GLIST.c GQUEUES.c
+ Crea el ejecutable tests

`make`
+ Es equivalente a make all o a ejecutar los comandos todos por separado.

# Ejecucion del Parser

Para ejecutar el programa principal, se debera utilizar desde la consola
el comando

`./parser <diccionario.txt> <frases.txt> [<salida.txt>]`
                           
**IMPORTANTE!** Los archivos <diccionario.txt> y <frases.txt> deben
encontrarse en los directorios 'dicts' y 'phrases' respectivamente, de otra 
manera se producira un error al intentar abrirlos.

* SALIDA. El tercer argumento, < salida.txt > es opcional. De no indicarse
explicitamente, se generara una salida con el nombre 'parsing_output.txt'.
Tanto en este caso como si se brinda un nombre, los archivos de salida se
almacenan en el directorio 'outputs'.

# testgenerator

Dentro del directorio del proyecto se encuentra un archivo test_generator.c. Tras ser compilado como indica el apartado de compilacion, crea el ejecutable
testgenerator. Este programa es utilizado desde la consola con el comando

`./testgenerator <dict.txt> <num_lineas> <largo_lineas>`

donde 

+ `<dict.txt>` es el nombre de un diccionario en la carpeta dicts.
      
+ `<num_lineas>` es la cantidad de lineas a generar
    
+ `<largo_lineas>` es el largo promedio de las lineas a generar

De esta manera, tras su ejecucion, testgenerator crea un nuevo archivo txt en el directorio 'phrases' que consta de `<num_lineas>` lineas de caracteres de
longitud entre `<largo_lineas>` ± 25% generadas aleatoriamente a partir de todos los posibles prefijos de <dict.txt>

# Tests

Se incluyen algunos de los tests realizados para el programa en el programa tests.c.

Se ejecuta por consola tras su compilacion con make tests simplemente con el comando 

`./tests`

+ test_duhalde testea casos borde del parseo con un diccionario y listado de 
frases modelo.
+ Los tests restantes son simples chequeos de funciones relevantes para el 
programa.

# Informe

Una explicacion mas detallada de la estructura de datos y algoritmo propuestos puede encontrarse en el archivo informe.pdf dentro de este mismo directorio.
Alli tambien se encuentra un detalle sobre los casos de prueba adjuntados y el generador de tests.
