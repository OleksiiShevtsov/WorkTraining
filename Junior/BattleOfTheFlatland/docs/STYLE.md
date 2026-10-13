# BOTF code style

## Naming

| Что | Стиль | Пример |
|-----|--------|--------|
| Члены класса | `m_` + snake_case | `m_position`, `m_clients` |
| Локальные / параметры | snake_case без префикса | `client_id`, `dt` |
| Константы | `k` + PascalCase | `kTickHz`, `kPort` |
| Свободные функции | snake_case | `build_world_state` |
| Типы / классы | PascalCase | `GameLogic`, `TcpServer` |
| Namespaces | `botf::…` | `botf::net`, `botf::logic`, `botf::app` |

Не использовать trailing underscore (`foo_`) для members.

## Форматирование

Корень проекта: `.clang-format` (Google-based, 2 spaces, column 100).  
При сборке target `botf_format` прогоняет `*.cpp` / `*.h` / `*.hpp` модулей проекта.

## Слои (ориентир)

- `net/` — транспорт (байты, framing, TCP), без геймплея  
- `logic/` — симуляция, без SFML Network / wire protobuf  
- `client/app`, `server` glue — сборка цикла; дальше выносится в App-классы (см. Foundation 23–26)
