#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <string.h>


int	main()
{
	// Initializing Socket
	// AF_INET is for IPv4
	// SOCK STREAM TCP - Connection (tunnel)
	// SOCK DGRAM - UDP - Connectionless
	// 0 means use underlying protocol, in this care we already specified in SOCK_STREAM that its a TCP.
	// use man socket for more information.
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0)
		printf("Socket creation failed.\n");
	else
		printf("Socket creation successful.\n");

	// Once we have socket created. we need to create a struct sockaddr_in - which is actually same as struct sockadrr. sockaddr_in is created to make it easier to add details in sockaddr.
	/* The structure looks like this
	 * struct sockaddr_in {
	 * 	short	sin_family;
	 * 	u_short	sin_port;
	 * 	struct in_addr	sin_addr;
	 * 	char	sin_zero[8];
	 * }
	 */
	struct sockaddr_in srv;
	srv.sin_family = AF_INET; //Define the communication doman; in our case ipv4.
	// Before going into ports, please learn about Little Endian and Big Endian. Network uses Big Endian and Intel chips for example uses Little Endian, therefore any hexa bits need to be converted into Big Endian.
	// h = host, t = to, n = network, s = short.
	// Whats little Endian and big Endian? https://www.youtube.com/watch?v=T1C9Kj_78ek 	
	srv.sin_port = htons(27015);

	// We are specifying the address (ip). INADDR_ANY gives the current computer's ip. If you are running the server on a local machine its usually always INADDR_ANY;
	// more sin_addr is a struct. struct in_addr
	// Here's what in_addr struct looks like:
	/*
	 * struct in_addr {
    		uint32_t s_addr; // that's a 32-bit int (4 bytes)
	 *	};
	 */

	// uint32_t - <stdint.h> - Guarantees the variable takes exactly 32 bits(4 bytes) of memory.
	// stands for Unsigned Integer 32 Bits
	// Why dont we used unsignde int? not all computers have 4 bytes for int. uint_32 provides portability and consistency.

	// if you dont want ur current computers address use:
	// srv.sin_addr.s_addr = inet_addr("xxx.xxx.xxx.xxx");
	srv.sin_addr.s_addr = INADDR_ANY;
	
	// As we discussed before, sockaddr_in is easier to fill up instead of sockadrr therefore we create sockaddr_in and typecast it as sockaddr..
	// to ensure the size is equal we add 0s for additional space to ensure both structs have same size.
	memset(&(srv.sin_zero), 0, 8);

	// Why did we create sockaddr_in when we already have a socket??
	// Once we create the socket. It's up but it doesnt know where to send the data and from where to recieve. It doesnt know the port and address, thats why we specifies these thing in sockaddr_in so that it now has an address and port, the information it needs to send and recieve data.
	
	int s_res = bind(socket_fd, (struct sockaddr *)&srv, sizeof(srv));
	if (s_res < 0)
		printf("Failed to bind the sockaddr to the socket.\n");
	else
		printf("succesfully binded the sockaddr to the socket.\n");
	
	// Once Successfully created and binded. We are ready to listen to the requests.
	// Secondly argument specifices how many requests you want to actively deal with at a specific time, we stated 2.
	// INET6_ADDRSTRLEN for ipv6.
	char ip_str[INET_ADDRSTRLEN];
	
	// Runs with arpa include, converts ip to string from binary (network to presentation) and adds it to ip_str.
	inet_ntop(AF_INET, &srv.sin_addr, ip_str, INET_ADDRSTRLEN);
	s_res = listen(socket_fd, 2);
	if (s_res < 0)
		printf("Failed to listen to the socket.");
	else
		printf("Server is now listening to at %s:%i", ip_str, ntohs(srv.sin_port));

}
