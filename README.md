**Для работы модуля необходимо обновить до последней версии [Utils](https://github.com/Pisex/cs2-menus/releases).**

**[VIP] No Fall Damage** — добавляет возможность отключения урона от падения для VIP-игроков.

В `groups.ini` добавьте следующее:
```ini
  // Включение/выключение отключения урона от падения для VIP-игроков
  // Формат: "nofalldamage" "1" (1 для включения, 0 для выключения)
  "nofalldamage" "1"
```

В файл `vip.phrases.txt` добавьте следующее:
```
    "nofalldamage"
    {
        "en"    "No Fall Damage"
        "ru"    "Отключение урона от падения"
    }
```
Требования: [VIP CORE](https://csdevs.net/resources/vip-core.511/)

