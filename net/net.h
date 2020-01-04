#pragma once
#ifndef __NET_H
#define __NET_H

/* Return a fd corresponding to a bound TCP socket at portno, -1 on error. */
int get_tcp_listener_sock (char* portno);

#endif // __NET_H

