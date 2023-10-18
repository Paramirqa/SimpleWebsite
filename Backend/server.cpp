#include "server.h"
#define DEBUG_MODE 1  // 1 для включения, 0 для отключения

// Объявление функции для обработки клиентского запроса
int main() {
  int sockfd, client_socket = 0;
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

  while (true) {
    client_socket =
        accept(sockfd, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket < 0) {
      std::cerr << "Error accepting connection" << std::endl;
      return 1;
    }

    // Вызываем функцию для обработки клиентского запроса с проверкой макроса
#if DEBUG_MODE
    std::cout << "Debug: Handling client request..." << std::endl;
#endif

    // Вызываем функцию для обработки клиентского запроса
    ClientRequest(client_socket);
  }
  close(sockfd);
  return 0;
}

// Определение функции для обработки клиентского запроса
void ClientRequest(int client_socket) {
  const int max_client_buffer_size = 1024;
  char buf[max_client_buffer_size];

  ssize_t bytesRead;
  std::string request_body;  // Создаем переменную для хранения тела запроса

  // Парсинг Боди-запроса
  do {
    bytesRead = recv(client_socket, buf, max_client_buffer_size, 0);
    if (bytesRead > 0) {
      request_body.append(
          buf, bytesRead);  // Добавляем данные к переменной request_body
    }
  } while (bytesRead > 0);

  // Выводим тело запроса на стандартный вывод (для демонстрации)
  std::cout << "HTTP Request Body:\n" << request_body << std::endl;

  // Открываем файл для чтения
  std::ifstream file(
      "Test_files/aaaa.html");  // Замените "example.txt" на имя вашего файла

  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return;
  }

  std::stringstream response;
  std::stringstream response_body;

  while (true) {
    // Читаем данные из файла в буфер
    file.read(buf, max_client_buffer_size);
    ssize_t bytesRead = file.gcount();

    if (bytesRead == 0) {
      break;  // Достигнут конец файла
    }

    response_body.write(buf, bytesRead);
  }

  file.close();  // Закрываем файл

  // Остальной код для отправки данных клиенту остается без изменений
  response << "HTTP/1.1 200 OK\r\n"
           << "Version: HTTP/1.1\r\n"
           << "Content-Type: text/html; charset=utf-8\r\n"
           << "Content-Length: " << response_body.str().length() << "\r\n\r\n"
           << response_body.str();

  ssize_t send_result =
      send(client_socket, response.str().c_str(), response.str().length(), 0);

  if (send_result == -1) {
    std::cerr << "Error sending data" << std::endl;
  }
#if DEBUG_MODE
  std::cout << "Debug: Received request body:\n" << request_body << std::endl;
#endif

  close(client_socket);
}