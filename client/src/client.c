#include "client.h"
#include <commons/config.h>
#include <string.h>


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger,"Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if (config == NULL){
		log_error(logger, "error al leer .config");
		abort();
	}
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	clave = config_get_string_value(config, "CLAVE");

	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);
	log_info(logger, "CLAVE: %s", clave);
	

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	if(verificar_conexion(&logger, conexion) == -1) return EXIT_FAILURE;
	
	log_info(logger, "Cliente conectado con exito");

	// Enviamos al servidor el valor de CLAVE como mensaje
	
	enviar_mensaje(clave, conexion);
	return EXIT_SUCCESS;
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

// Verifica si la conexion fue exitosa. En caso de no serlo, lo documenta en el logger y libera la memoria, retornando -1.
int verificar_conexion(t_log** logger, int conexion){
	switch (conexion){
		case ERR_ADDRINFO:
			log_error(*logger, "Error en el addrinfo");
			log_destroy(*logger);
			return -1;
		case ERR_SOCKET:
			log_error(*logger, "Error al crear el socket");
			log_destroy(*logger);
			return -1;
		case ERR_CONNECT:
			log_error(*logger, "Error al conectar");
			log_destroy(*logger);
			return -1;
		default:
			return 0;
	}
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("cliente.log","cliente",true,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config"); 

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	while(1){
		leido = readline("> ");

		if(leido == NULL || (*leido) == '\0')
			break;
		
		
		log_info(logger, leido);
		free(leido);
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	
	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");

	while(leido != NULL && strcmp(leido, "") != 0) {
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        
        free(leido);
		leido = readline("> ");
    }
	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	
}

