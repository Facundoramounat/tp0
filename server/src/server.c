#include "server.h"

int main(void) {
	logger = log_create("server.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	// Obtener el fd del servidor con un serverinfo valido
	int server_fd = iniciar_servidor();
	if(verficar_socket(&logger, server_fd) == EXIT_FAILURE) return EXIT_FAILURE;
	
	int cliente_fd = esperar_cliente(server_fd);
	if(verficar_socket(&logger, cliente_fd) == EXIT_FAILURE) return EXIT_FAILURE;
	
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}

/*Verifica el socket. Si hay error lo documenta en el logger y libera la memoria.4
Retorna 0 si el socket es correcto. (EXIT_SUCCESS)
Retorna 1 si hay error. (EXIT_FAILURE)
*/
int verficar_socket(t_log** logger, int socket){
	switch (socket){
		case ERR_ADDRINFO:
			log_error(*logger, "Error al crear la red");
			log_destroy(*logger);

		case ERR_SOCKET:
			log_error(*logger, "Error al crear el socket");
			log_destroy(*logger);

		case ERR_BIND:
			log_error(*logger, "No se pudo establecer conexion con el puerto, Esta ocupado?");
			log_destroy(*logger);

		case ERR_LISTEN:
			log_error(*logger, "No se pudo poner el servidor en modo de escucha");
			log_destroy(*logger);

		case ERR_ACCEPT:
			log_error(*logger, "No se pudo comunicar con el cliente");
			close(socket);
			log_destroy(*logger);

		default:
			log_info(*logger, "Servidor listo para recibir al cliente");
			return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
