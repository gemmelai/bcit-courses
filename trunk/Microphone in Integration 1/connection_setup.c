#include "win_main.h"

/*------------------------------------------------------------------------
--		FUNCTION:		client
--
--		DATE:			February 24, 2009
--
--		REVISIONS:		March 30 - Added a type int which will allow us
--								   to create either a TCP or UDP connection.
--						April 10, 2009 - Steffen L. Norgren, modified to
--							allow for both TCP and UDP connection types.
--
--		DESIGNER:		Jerrod Hudson
--		PROGRAMMER:		Jerrod Hudson, Steffen L. Norgren
--
--		INTERFACE:		void client(HWND hwnd, clientInfo ci)
--							HWND hwnd: Handle to the window to paint
--							clientInfo ci: User defined client options,
--							including IP, port, protocol, packetsize, etc.
--
--		RETURNS:		void
--
--		NOTES:
--		Creates a socket, binds it to a port, allow asyncronous
--		message passing.
------------------------------------------------------------------------*/
void setup_client(HWND hwnd, int type)
{
	WSADATA				wsaData;
	char				arg;

	if((WSAStartup(MAKEWORD(2, 2), &wsaData)) == -1)
	{
		MessageBox(NULL, "WSAStartup failed!", "ERROR", MB_OK);
		WSACleanup();
	}

	if (type == SOCK_STREAM) {
		/* Create TCP socket */
		if((ci.tcpSocket = socket(AF_INET, type, IPPROTO_TCP)) == INVALID_SOCKET)
			MessageBox(NULL, "Unable to create TCP socket!", "ERROR", MB_OK);

		/* Set SO_REUSEADDR for port reuseability */
		if(setsockopt(ci.tcpSocket, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
			MessageBox(NULL, "Unable to set SO_REUSEADDR", "ERROR", MB_OK);

		memset((char *)&remote, 0, sizeof(SOCKADDR_IN));
		remote.sin_family = AF_INET;
		remote.sin_addr.s_addr = inet_addr(ci.ip);
		remote.sin_port = htons(ci.tcp_port);

		WSAAsyncSelect(ci.tcpSocket, hwnd, WM_TCP_SOCKET, FD_CONNECT | FD_WRITE | FD_READ | FD_CLOSE);
	}
	else if (type == SOCK_DGRAM) {
		/* Create UDP socket */
		if((ci.udpSocket = socket(AF_INET, type, IPPROTO_UDP)) == INVALID_SOCKET)
			MessageBox(NULL, "Unable to create UDP socket!", "ERROR", MB_OK);

		memset((char *)&udp_remote, 0, sizeof(SOCKADDR_IN));
		udp_remote.sin_family = AF_INET;
		udp_remote.sin_addr.s_addr = inet_addr(ci.ip);
		udp_remote.sin_port = htons(ci.udp_port);
	}
}

/*------------------------------------------------------------------------
--		FUNCTION:		server
--
--		DATE:			February 24, 2009
--
--		REVISIONS:		March 30 - Added a type int which will allow us
--								   to create either a TCP or UDP connection.
--						April 10, 2009 - Steffen L. Norgren, modified to
--							allow for both TCP and UDP connection types.
--
--		DESIGNER:		Jerrod Hudson
--		PROGRAMMER:		Jerrod Hudson, Steffen L. Norgren
--
--		INTERFACE:		void server(HWND hwnd, serverInfo si)
--							HWND hwnd: Handle to the window to paint
--							serverInfo si: User defined port and protocol
--
--		RETURNS:		void
--
--		NOTES:
--		Creates a socket, binds an address to it, sets the total number
--		of clients, and listens for connections.
------------------------------------------------------------------------*/
void setup_server(HWND hwnd, int type)
{
	WSADATA				wsaData;
	char				arg;

	if((WSAStartup(MAKEWORD(2, 2), &wsaData)) == -1)
	{
		MessageBox(NULL, "WSAStartup failed!", "ERROR", MB_OK);
		WSACleanup();
	}

	if (type == SOCK_STREAM)
	{
		/* Create TCP socket */
		if((ci.tcpSocket = socket(AF_INET, type, IPPROTO_TCP)) == INVALID_SOCKET)
			MessageBox(NULL, "Unable to create TCP socket!", "ERROR", MB_OK);

		/* Set SO_REUSEADDR for port reuseability */
		if(setsockopt(ci.tcpSocket, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
			MessageBox(NULL, "Unable to set SO_REUSEADDR", "ERROR", MB_OK);

		memset((char *)&local, 0, sizeof(SOCKADDR_IN));
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = INADDR_ANY;
		local.sin_port = htons(ci.tcp_port);

		/* Bind address to socket */
		if(bind(ci.tcpSocket, (SOCKADDR *)&local, sizeof(SOCKADDR)) == SOCKET_ERROR)
			MessageBox(NULL, "Unable to bind address to socket!", "ERROR", MB_OK);

		/* Set max number of connections */
		if(listen(ci.tcpSocket, NUMCONNECTIONS))
			MessageBox(NULL, "Unable to set number of connections!", "ERROR", MB_OK);

		WSAAsyncSelect(ci.tcpSocket, hwnd, WM_TCP_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
	}
	else if (type == SOCK_DGRAM) {
		/* Create TCP socket */
		if((ci.udpSocket = socket(AF_INET, type, IPPROTO_UDP)) == INVALID_SOCKET)
			MessageBox(NULL, "Unable to create UDP socket!", "ERROR", MB_OK);

		memset((char *)&udp_local, 0, sizeof(SOCKADDR_IN));
		udp_local.sin_family = AF_INET;
		udp_local.sin_addr.s_addr = INADDR_ANY;
		udp_local.sin_port = htons(ci.udp_port);

		/* Bind address to socket */
		if(bind(ci.udpSocket, (SOCKADDR *)&udp_local, sizeof(SOCKADDR)) == SOCKET_ERROR)
			MessageBox(NULL, "Unable to bind address to socket!", "ERROR", MB_OK);
	}
}