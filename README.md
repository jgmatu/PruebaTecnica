## Nota Tecnica

	- Los destructores se llaman de forma implicita al terminar el ambito de ejecución si son llamados
	  en la pila de la función en este caso la función main.
	
	- En tal caso los join de los thread son llamados justo antes de terminar el ambito de la funcion 
	  main y por tanto antes de finalizar el proceso evitando problemas de runtime en la liberación
	  de recursos del proceso de la prueba técnica.

	- Optimización de memoria: Se usa la pila de la función main para guardar las referencias de los 
	  contenedores de las colas, los contenedores de la STL hacen el manejo de la memoria de heap
	  y allocator de los elementos de entrada y salida del generador de ruido y el filtro de los 
	  vectores de entrada.

	- Uso de smart pointers: Se hace uso de punteros inteligentes para la liberación de memoria
	  de los módulos y su llamada implicita al destructor que hace la espera activa de los hilos
	  de ejecución.

	- Se usa herencia para demostrar el trabajo con herencia en C++.
	
	- Se usa la clase genérica de colas para demostrar el uso de templates y además cómo forma 
	  óptima de usar ambas colas la de entrada de raw_data y la cola filter_data del
	  sensor simulado. 

	- Estructura del proyecto de forma ordenada: Las interfaces se hacen publicas en el directorio
	  de include, cada módulo tiene su propia carpeta con los ficheros fuentes necesarios para que
	  el módulo funcione de manera correcta.

	- Implementación de multiherencia para añadir la interfaz: Para terminar de forma correcta la
	  prueba técnica se ha hecho uso de multiherencia para añadir la interfaz IModule, ya que C++ no
	  permite el uso de interfaces se debe realizar la implmentación de la interfaz haciendo uso de 
	  multiherencia.

	- Para evitar el acceso a las clases internas de la interfaz IModule se hace uso de patrón de 
	  diseño de fabrica, cada fábrica llama de forma interna al módulo para pasar por argumento las
	  colas a las que puede acceder y evitar el uso de memoria global en el sistema.
