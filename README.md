# Simple HTTP Web Server

A lightweight HTTP web server implemented in C that serves static files from a `www` directory. The server supports various file types including HTML, CSS, JavaScript, images, PDFs, and videos.

## Features

- Static file serving from `www` directory
- Support for multiple MIME types:
  - HTML (.html)
  - CSS (.css)
  - JavaScript (.js)
  - Images (.jpg, .jpeg, .png)
  - Video (.mp4)
  - PDF (.pdf)
- Basic error handling (400, 404, 500)
- Auto-serves index.html for root path requests
- Custom 404 error page

## Prerequisites

- Windows operating system
- GCC compiler (MinGW)
- WinSock2 library

## Project Structure
webserver/ ├── www/ # Web root directory │ ├── index.html # Default landing page │ ├── Error404.html # Custom 404 error page │ ├── style.css # Stylesheets │ └── script.js # JavaScript files ├── main.c # Main server implementation ├── server.h # Server header file ├── mime_types.c # MIME type handling ├── mime_types.h # MIME type definitions ├── mutils.c # Utility functions ├── mutils.h # Utility header file └── Makefile # Build configuration


## Building the Project

Use the included Makefile to build the project:<br><br>
make <br><br>
This will create the executable webserver.exe.<br><br>

## Running the Server<br>
Make sure you have a www directory with your web files<br>
Run the compiled executable:<br>

   gcc main.c -o server.exe -lws2_32<br>
  ./server.exe<br><br>
The server will start on port 3000<br>
Access the website at http://localhost:3000<br>

## Configuration<br>
   Default port: 3000 (defined in PORT macro)<br>
   Buffer size: 1024 bytes (defined in BUFFER_SIZE macro)<br>
   Web root directory: "www" (defined in WWW_DIR macro)<br>

## Error Handling<br>
The server handles several error cases:<br>

400 Bad Request: Invalid HTTP request<br>
404 Not Found: Requested file doesn't exist<br>
500 Internal Server Error: Server-side issues<br>

## Contributors<br>
Kithmini Mayodya
