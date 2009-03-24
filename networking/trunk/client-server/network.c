/******************************************************************************
 *  Source File: network.c
 *
 *  Program:     Tux Bomber
 *
 *  Functions:   set_conn_type(int type)
 *               conn_setup(char *host, char *port)
 *               keepalive()
 *               send_map(unsigned char *map, size_t len)
 *               recv_map(unsigned char *map, size_t len)
 *               request_move(int x, int y)
 *               request_bomb(int type)
 *               explode_bomb(int x, int y)
 *               transfer(unsigned char *data, size_t len)
 *               add_coords_xy(int x, int y, unsigned char *data, size_t len)
 *               process_data(unsigned char *data, size_t len)
 *               
 *  Date:        March 2, 2009
 *
 *  Revisions:   March 20, 2009 - Steffen L. Norgren
 *                   Updated headers to cornform to Aman's standards
 *                   TODO: Change dynamic arrays to conform to ISO C99
 *                   TODO: Correct duplicate symbols errors
 * 
 *  Designer:    David Young
 *  Programmer:  David Young, Steffen L. Norgren
 * 
 *  Interface:   set_conn_type(int type)
 *		            int type: TCP or UDP connection type.
 * 
 *  Returns:     0 on success or (-1) on error.
 * 
 *  Description: Set the connection type to TCP or UDP.
 *
 *****************************************************************************/

#include "network.h"

/******************************************************************************
 *  Function:    set_conn_type
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   set_conn_type(int type)
 *		            int type: TCP or UDP connection type.
 * 
 *  Returns:     0 on success or (-1) on error.
 * 
 *  Description: Set the connection type to TCP or UDP.
 *
 *****************************************************************************/
int set_conn_type(int type) {
	if(type == TCP || type == UDP) {
		conn_type = type;
		return 0;
	} else {
		fprintf(stderr, "invalid connection type in %s:set_conn_type\n", __FILE__);
		return -1;
	}
}

/******************************************************************************
 *  Function:    conn_setup
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   conn_setup(char *host, char *port)
 *		            char *host: The IP-address or hostname to connect to
 *                  char *port: The port number to connect to
 * 
 *  Returns:     0 on success or (-1) on failure.
 * 
 *  Description: Connect to the specified host and port.
 *
 *****************************************************************************/
int conn_setup(char *host, char *port) {
	struct addrinfo hints, *results;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = conn_type;
	
	if(getaddrinfo(host, port, &hints, &results) == -1) {
		fprintf(stderr, "getaddrinfo failed in %s:connect_to_server\n", __FILE__);
		return -1;
	}
	
	sock = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	
	memcpy(&server, (struct sockaddr_in *)results->ai_addr, sizeof(server));
	
	if(conn_type == TCP) {
		connect(sock, (struct sockaddr *)&server, sizeof(server));
	}
	
	return 0;
}

/******************************************************************************
 *  Function:    keepalive
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   keepalive()
 * 
 *  Returns:     void
 * 
 *  Description: Send a KEEPALIVE packet to let the other side know
 *               we're still here.
 *
 *****************************************************************************/
void keepalive() {
	unsigned char data = KEEPALIVE;
	transfer(sock, &data, 1);
}

/******************************************************************************
 *  Function:    send_map
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   send_map(unsigned char *map, size_t len)
 *                   unsigned char *map: The map's character representation
 *                   size_t len: The length of the map string
 *
 *  Returns:     0 on success or (-1) if malloc fails.
 * 
 *  Description: Sends the map to the client.
 *
 *****************************************************************************/
int send_map(int *sockets, int num_sockets, unsigned char *map, size_t len) {
	unsigned char *data = (unsigned char *)malloc(len + 1);
	int a;
	
	if(data == NULL) {
		fprintf(stderr, "malloc failed in send_map\n");
		return -1;
	}
	
	data[iTYPE] = MAP;
	memcpy(data + 1, map, len);
	
	for(a = 0; a < num_sockets; a++) {
		if(transfer(sockets[a], data, len + 1) < len + 1) { /* not the whole len was transfered */
			return -1;
		}
	}
	
	return 0;
}

/******************************************************************************
 *  Function:    recv_map
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   recv_map(unsigned char *map, size_t len)
 *                   unsigned char *map: The map's character representation
 *                   size_t len: The length of the map string
 *
 *  Returns:     0 on success or (-1) if malloc fails.
 * 
 *  Description: Receives the map from a client.
 *
 *****************************************************************************/
int recv_map(unsigned char *map, size_t len) {
	/* this is where logic converts the uchar* into the format they want */
	return 0;
}

/******************************************************************************
 *  Function:    request_move
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   request_move(int x, int y)
 *                   int x: The x-coordinate to which the client wants to move
 *                   int y: The y-coordinate to which the client wants to move
 *
 *  Returns:     0 on success or (-1) if malloc fails.
 * 
 *  Description: Send a request to the server, asking to move to xy
 *
 *****************************************************************************/
int request_move(int x, int y) {
	int len = sizeof(unsigned char) * ((sizeof(int) * 2) + 2);
	unsigned char *data = (unsigned char *)malloc(len);
	
	if(data == NULL) {
		fprintf(stderr, "malloc failed in request_move\n");
		return -1;
	}
	
	data[iTYPE] = MOVE;
	add_coords_xy(x, y, data + 1, len - 1);
	transfer(sock, data, len);
	
	return 0;
}

/******************************************************************************
 *  Function:    request_bomb
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   request_bomb(int type)
 *                   int type: The type of bomb to drop
 *
 *  Returns:     0 on success or !0 on error
 * 
 *  Description: Send a request to the server, asking to drop a bomb at xy
 *
 *****************************************************************************/
int request_bomb(int type) {
	int len = sizeof(int) + 2;
	unsigned char data[len]; /* ISO C90 forbids variable-size array */
	
	data[iTYPE] = BOMB;
	memcpy(data + 1, &type, sizeof(int));
	data[len - 1] = '\0';
	transfer(sock, data, len);
	
	return 0;
}

/******************************************************************************
 *  Function:    explode_bomb
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   explode_bomb(int x, int y)
 *                   int x: The x-coordinate of the bomb to blow up
 *                   int y: The y-coordinate of the bomb to blow up
 *
 *  Returns:     0 on success or !0 on error
 * 
 *  Description: Send a message to all the clients, saying that the
 *               bomb at xy blew up.
 *
 *****************************************************************************/
int explode_bomb(int x, int y) {
	int len = sizeof(unsigned char) * ((sizeof(int) * 2) + 2);
	unsigned char *data = (unsigned char *)malloc(len);
	
	data[iTYPE] = EXPLOSION;
	add_coords_xy(x, y, data + 1, len - 1);
	transfer(sock, data, len);
	
	return 0;
}

/******************************************************************************
 *  Function:    transfer
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   transfer(unsigned char *data, size_t len)
 *                   unsigned char *data: The string of data to transfer
 *                   size_t len: The length of the data string
 *
 *  Returns:     0 on success or (-1) on error
 * 
 *  Description: Transfer the data, regardless of TCP/UDP
 *
 *****************************************************************************/
int transfer(int sockt, unsigned char *data, size_t len) {
	if(sockt == -1) { /* the socket is invalid */
		return -1;
	}
	
	if(conn_type == TCP) {
		return send(sockt, data, len, 0);
	} else if(conn_type == UDP) {
		return sendto(sockt, data, len, 0, (struct sockaddr *)&server, sizeof(server));
	} else { /* this should never happen */
		return -1;
	}
}

/******************************************************************************
 *  Function:    add_coords_xy
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   add_coords_xy(int x, int y, unsigned char *data, size_t len)
 *                   int x: The first coordinate to add
 *                   int y: The second coordinate to add
 *                   unsigned char *data: The data string to add the coords into
 *                   size_t len: The length of the data string
 *
 *  Returns:     0 on success or (-1) on error
 * 
 *  Description: Writes x and y into the string and appends a null-char
 *
 *****************************************************************************/
int add_coords_xy(int x, int y, unsigned char *data, size_t len) {
	if(len < (2 * sizeof(int)) + 1) {
		fprintf(stderr, "len is < required length in add_coords_xy\n");
		return -1;
	}
	
	memcpy(data, &x, sizeof(int));
	memcpy(data + sizeof(int), &y, sizeof(int));
	
	data[len - 1] = '\0';
	
	return 0;
}

/******************************************************************************
 *  Function:    process_data
 * 
 *  Date:        March 2, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   process_data(unsigned char *data, size_t len)
 *                   unsigned char *data: The string to get data from
 *                   size_t len: The length of the data string
 *
 *  Returns:     0 on success or (-value) on error
 *               he failure values are listed in network.h
 * 
 *  Description: Process the data that's received.
 *
 *****************************************************************************/
int process_data(unsigned char *data, size_t len) {
	switch(data[iTYPE]) {
		case KEEPALIVE: /* do nothing */
			break;
			
		case MAP: /* deal with the map */
			if(mode == CLIENT) {
				/* update the map */
			} else if(mode == SERVER) { /* why is the client sending the server a map? */
				return ERR_SERV_RECV_MAP;
			} else {
				return ERR_UNKNOWN_INPUT;
			}
			
			break; /* end case MAP */
			
		case MOVE: /* client requesting a move */
			if(mode == CLIENT) { /* client shouldn't get a MOVE message */
				return ERR_CLNT_RECV_MOVE;
			} else if(mode == SERVER) {
				/* the client wants to move to this position */
			} else {
				return ERR_UNKNOWN_INPUT;
			}
			
			break; /* end case MOVE */
			
		case BOMB: /* client requesting a bomb drop */
			if(mode == CLIENT) {
				return ERR_CLNT_RECV_BOMB;
			} else if(mode == SERVER) {
				/* the client wants to drop a bomb of the specified type */
			} else {
				return ERR_UNKNOWN_INPUT;
			}
			
			break; /* end case BOMB */
			
		case EXPLOSION: /* draw the explosion */
			if(mode == CLIENT) {
				/* draw the explosion */
			} else if(mode == SERVER) { /* only servers send EXPLOSION messages */
				return ERR_SERV_RECV_EXPLOSION;
			} else {
				return ERR_UNKNOWN_INPUT;
			}
			
			break; /* end case EXPLOSION */
			
		default: /* this should never happen */
			return ERR_UNKNOWN_INPUT;
	}
	
	return 0;
}

/******************************************************************************
 *  Function:    parse_info_byte
 * 
 *  Date:        March 23, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   parse_info_byte(unsigned char *data)
 *		            unsigned char *data: array of bytes that makes up the data.
 * 
 *  Returns:     The value of the byte that was parsed.
 * 
 *  Description: Gets the action, player and extra information from a byte.
 *		The information is stored like this:
 *			action:	xxx- ---- (first 3 bits)
 *			player:	---x xx-- (second 3 bits)
 *			extra:	---- --xx (last 2 bits)
 *
 *****************************************************************************/
int parse_info_byte(unsigned char *data) {
	int type, player, extra;
	
	type = get_action_type(data[0]); /* gets bits xxx- ---- for action type */
	player = get_player_id(data[0]); /* gets bits ---x xx-- for player id */
	extra = get_extra_info(data[0]); /* gets bits ---- --xx for extra info */
	
	return data[0];
}

/******************************************************************************
 *  Function:    get_action_type
 * 
 *  Date:        March 23, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   get_action_type(int info)
 *		            int info: the byte containing the information we want.
 * 
 *  Returns:     The value of the first 3 bits of the lowest-byte of info.
 * 
 *  Description: Bit shifts and gets the value of the bits marked x: "xxx- ----"
 *		The value should be between 0 and 7 (3 bits).
 *
 *****************************************************************************/
int get_action_type(int info) {
	switch(info >> 5) { /* bits xxx- ---- for action type */
		case 0: /* movement */
			break;
			
		case 1: /* player quit */
			break;
			
		case 2: /* bomb planted */
			break;
			
		case 3: /* bomb explodes */
			break;
			
		case 4: /* player dies */
			break;
			
		case 5: /* powerup */
			break;
			
		case 6: /* currently not defined */
		case 7: /* currently not defined */
		default: /* this should never happen */
			break;
	}
	
	return (info >> 5);
}

/******************************************************************************
 *  Function:    get_player_id
 * 
 *  Date:        March 23, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   get_player_id(int info)
 *		            int info: the byte containing the information we want.
 * 
 *  Returns:     The value of the second 3 bits of the lowest-byte of info.
 * 
 *  Description: Bit shifts and gets the value of the bits marked x: "---x xx--"
 *		The value should be between 0 and 7 (3 bits).
 *
 *****************************************************************************/
int get_player_id(int info) {
	switch((info >> 2) % 8) { /* bits ---x xx-- for player id */
		case 0: /* player 1 */
			break;
			
		case 1: /* player 2 */
			break;
			
		case 2: /* player 3 */
			break;
			
		case 3: /* player 4 */
			break;
			
		case 4: /* player 5 */
			break;
			
		case 5: /* player 6 */
			break;
			
		case 6: /* player 7 */
			break;
			
		case 7: /* player 8 */
			break;
			
		default: /* this should never happen */
			break;
	}
	
	return 0;
}

/******************************************************************************
 *  Function:    get_extra_info
 * 
 *  Date:        March 23, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   get_extra_info(int info)
 *		            int info: the byte containing the information we want.
 * 
 *  Returns:     The value of the last 2 bits of the lowest-byte of info.
 * 
 *  Description: Gets the value of the bits marked x: "---- --xx"
 *		The value should be between 0 and 3 (2 bits).
 *
 *****************************************************************************/
int get_extra_info(int info) {
	switch(info % 4) { /* bits ---- --xx for extra info */
		case 0: /* up for movement. num_bombs++ for powerup */
			break;
			
		case 1: /* down for movement. blast_radius++ for powerup */
			break;
			
		case 2: /* left for movement. spikey bombs for powerup */
			break;
			
		case 3: /* right for movement. unused for powerup */
			break;
			
		default: /* this should never happen */
			break;
	}
	
	return 0;
}

/******************************************************************************
 *  Function:    create_info_byte
 * 
 *  Date:        March 23, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   create_info_byte(int info)
 *		            int action: the action to be performed.
 *					int player: the player who's doing it.
 *					int extra: extra information about the action.
 * 
 *  Returns:     The compiled info byte, or 224 on error (a value that shouldn't happen).
 * 
 *  Description: Combines the 3 fields into the info byte format that the
 *		logic group wanted.
 *
 *****************************************************************************/
unsigned char create_info_byte(int action, int player, int extra) {
	unsigned char info = 0;
	
	/* action only goes up to 5 cause that's all i was given. 6 and 7 are free,
	 so i use 7's shifted form (224) as an error return value */
	if(action >= 0 && action <= 5 && player >= 0 && player <= 7 && extra >= 0 && extra <= 3) {
		info += action << 5;
		info += player << 2;
		info += extra;
	} else {
		fprintf(stderr, "Invalid input in create_info_byte\n");
		fprintf(stderr, "\taction: %d, player: %d, extra: %d.\n", action, player, extra);
		return 224;
	}
	
	return info;
}
