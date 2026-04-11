# UDP Client-Server

Клиент-серверное приложение для передачи данных по протоколу UDP

## Требования

- CMake 3.15+
- Qt 5.15+ (компоненты: Core, Widgets, Network)
- Компилятор с поддержкой C++17

## Сборка

```bash
# Клонируем репозиторий
git clone https://github.com/ваш_аккаунт/test_task.git
cd test_task

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
