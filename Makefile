all: servidor_tcp cliente_tcp servidor_udp cliente_udp

servidor_tcp: servidor_tcp.c
	gcc -o servidorTCP servidor_tcp.c

cliente_tcp: cliente_tcp.c
	gcc -o clienteTCP cliente_tcp.c

servidor_udp: servidor_udp.c
	gcc -o servidorUDP servidor_udp.c

cliente_udp: cliente_udp.c
	gcc -o clienteUDP cliente_udp.c