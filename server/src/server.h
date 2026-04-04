#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils.h"


int verficar_socket(t_log** logger, int socket);
void iterator(char* value);

#endif /* SERVER_H_ */
