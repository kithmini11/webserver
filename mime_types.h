#ifndef MIME_TYPES_H
#define MIME_TYPES_H


#define MIME_HTML "text/html"
#define MIME_CSS "text/css"
#define MIME_JS "application/javascript"
#define MIME_PDF "application/pdf"

#include <string.h>

const char* get_mime_type(const char *file_path);

#endif