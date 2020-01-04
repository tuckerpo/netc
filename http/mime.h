#pragma once
#ifndef __MIME_H
#define __MIME_H

/* Extract MIME type from a filename. Default to application/octet-stream if not other type could be deduced. */
char * get_mime_type (char *buf);
    

#endif // __MIME_H
