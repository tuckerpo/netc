#include <stdio.h>
#include "mime.h"


int main (int argc, char** argv) {
   char testbuf[] = "something.HTML";
   char *mime_type = get_mime_type(testbuf);
   fprintf(stdout, "get_mime_type(%s) returning %s\n", testbuf, mime_type);
    return 0;
}
