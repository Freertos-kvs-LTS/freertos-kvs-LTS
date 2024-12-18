##################################################################################
#                                                                                #
#                           example_socket_tcp_trx                               #
#                                                                                #
##################################################################################

Table of Contents
~~~~~~~~~~~~~~~~~
 - Description
 - Setup Guide
 - Parameter Setting and Configuration
 - Result description
 - Supported List

 
Description
~~~~~~~~~~~
    Example of TCP bidirectional transmission with use two threads for TCP tx/rx on one socket.
    Example 1 uses non-blocking recv and semaphore for TCP send/recv mutex.
    Example 2 does not use any synchronization mechanism, but can only run correctly on lwip with TCPIP thread msg api patch.

Setup Guide
~~~~~~~~~~~
        1. Add socket_tcp_trx example to SDK
        
        /component/common/example/socket_tcp_trx
        .
        |-- example_socket_tcp_trx_1.c
        |-- example_socket_tcp_trx_2.c
        |-- example_socket_tcp_trx.h
        `-- readme.txt
        
	2.Select which case to compile in app_example.
		void app_example(void)
		{
			example_socket_tcp_trx_1();
			//example_socket_tcp_trx_2();
		}

        4. Add socket_tcp_trx example source files to project
        (a) For IAR project, add ota http example to group <example> 
            $PROJ_DIR$\..\..\..\component\example\socket_tcp_trx\example_socket_tcp_trx_1.c
            $PROJ_DIR$\..\..\..\component\example\socket_tcp_trx\example_socket_tcp_trx_2.c
        (b) For GCC project,  use CMD "make all EXAMPLE=socket_tcp_trx" to compile socket_tcp_trx example.

			
Parameter Setting and Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Modify SERVER_PORT in example_socket_tcp_trx_1(2).c for listen port.
    e.g. #define SERVER_PORT 5001
    Make automatical Wi-Fi connection when booting by using wlan fast connect example.

Result description
~~~~~~~~~~~~~~~~~~
    A socket TCP trx example thread will be started automatically when booting. 
    A TCP server will be started to wait for connection.
    Start a TCP client connecting to this server to start a TCP bidirectional transmission.
        iperf -c <tcp_server_IP_address> -d -i 1

Supported List
~~~~~~~~~~~~~~
[Supported List]
        Supported IC :
                RTL8730A, RTL872XE
