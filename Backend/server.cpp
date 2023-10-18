#include "server.h"

int main() {
  int sockfd, client_socket;
  struct sockaddr_in server_address, client_address;
  socklen_t client_address_len = sizeof(client_address);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8000);  // Порт сервера
  server_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) <
      0) {
    std::cerr << "Error binding" << std::endl;
    return 1;
  }

  if (listen(sockfd, SOMAXCONN) < 0) {
    std::cerr << "Error listening" << std::endl;
    return 1;
  }

  client_socket =
      accept(sockfd, (struct sockaddr*)&client_address, &client_address_len);

  if (client_socket < 0) {
    std::cerr << "Error accepting connection" << std::endl;
    return 1;
  }

  const int max_client_buffer_size = 1024;
  char buf[max_client_buffer_size];
  for (;;) {
    ssize_t result = recv(client_socket, buf, max_client_buffer_size, 0);

    std::stringstream response;
    std::stringstream response_body;

    if (result == -1) {
      std::cerr << "Error receiving data" << std::endl;
    } else if (result == 0) {
      std::cerr << "Connection closed by client" << std::endl;
    } else if (result > 0) {
      buf[result] = '\0';

      response_body << "<title>Test C++ HTTP Server</title>\n"
                    << "<h1>Test page</h1>\n"
                    << "<p>This is the body of the test page...</p>\n"
                    << "<h2>Request headers</h2>\n"
                    << "<pre>" << buf << "</pre>\n"
                    << "<em><small>Test C++ HTTP Server</small></em>\n";

      response << "HTTP/1.1 200 OK\r\n"
               << "Version: HTTP/1.1\r\n"
               << "Content-Type: text/html; charset=utf-8\r\n"
               << "Content-Length: " << response_body.str().length()
               << "\r\n\r\n"
               << response_body.str();

      ssize_t send_result = send(client_socket, response.str().c_str(),
                                 response.str().length(), 0);

      if (send_result == -1) {
        std::cerr << "Error sending data" << std::endl;
      }

      close(client_socket);
    }

    close(sockfd);
  }
  return 0;
}
