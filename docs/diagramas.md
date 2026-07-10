
stateDiagram-v2
    [*] --> Lying
    Lying --> Sitting : Acordou
    Sitting --> Standing : Tentando levantar
    Standing --> OnFloor : Queda
    OnFloor --> OnFloor : Imóvel (timer)
    OnFloor --> Standing : Levantou
    Standing --> EmptyBed : Saiu da cama
    EmptyBed --> Lying : Retornou
    Lying --> Lying : Dormindo

sequenceDiagram
    participant ESP32
    participant Telegram
    participant User
    ESP32->>ESP32: Detecção de queda
    ESP32->>ESP32: Captura imagem
    ESP32->>Telegram: Envia foto + mensagem
    Telegram->>User: Notificação push
    User->>User: Verifica situação