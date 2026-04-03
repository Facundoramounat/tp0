#include "server.h"

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	//Obtener el fd del servidor con un serverinfo valido
	int server_fd = iniciar_servidor();
	switch (server_fd){
		case ERR_ADDRINFO:
			log_error(logger, "Error al crear la red");
			log_destroy(logger);
			return EXIT_FAILURE;
		case ERR_SOCKET:
			log_error(logger, "Error al crear el socket");
			log_destroy(logger);
			return EXIT_FAILURE;
		case ERR_BIND:
			log_error(logger, "No se pudo establecer conexion con el puerto, Esta ocupado?");
			log_destroy(logger);
			return EXIT_FAILURE;
		default:
			log_info(logger, "Servidor listo para recibir al cliente");
			break;
	}
	return EXIT_SUCCESS;

	int cliente_fd = esperar_cliente(server_fd);

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
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
