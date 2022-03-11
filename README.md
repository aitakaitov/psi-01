# Description

KIV/PSI task 1 - UDP multicast client (+ server)

# Installation

Client is in the <code>udp_multicast_client</code> directory and can be built with CMake.
Server is in the <code>udp_multicast_server</code> directory and can be built with CMake.

Both server and client are written in C and can be run only in Linux environment, not on Windows.

# Usage

Both client and server can be run without any parameters and will use SSDP group and port as defaults (<code>239.255.255.250:1900</code>). It is also possible to specify the group and port by running the binaries with the first parameter being group and the second being port. The server sends <code>This is a UDP multicast message!</code> which the client then prints into the console.
