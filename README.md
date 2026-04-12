# UDP Client-Server
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-5.15-green.svg)](https://www.qt.io/)

Клиент-серверное приложение для передачи данных по протоколу UDP

## Требования

- CMake 3.15+
- Qt 5.15+ (компоненты: Core, Widgets, Network)
- Компилятор с поддержкой C++17

## Сборка

```bash
# Клонируем репозиторий
git clone https://github.com/Doggerr111/udp_task.git
cd udp_task

# Создаём папку для сборки
mkdir build && cd build

# Генерируем проект
cmake ..

# Собираем
cmake --build .
```
## Запуск сервера

```bash
./Server/Server      (Linux)
```
## Запуск клиента
```bash
./Client/Client      (Linux)
```
## Тесты
```bash
./Tests/test_pack_unpack      (Linux)
```
