# udp_server_among_us
Lets try testing a possible SUPER EASY among us project: idgaf about graphic, just technology.

Characteristics:

    1) UDP server to handle datagrams, i dont need to check always to receive the whole command by the user;
    2) asyncronous msg: [SERVER pov] for each connected client i associate to it a callback to handle it in the background while the server focuses on other clients connections or requests;
    3) 