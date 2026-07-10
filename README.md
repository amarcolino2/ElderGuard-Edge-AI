Sistema de monitoramento assistivo para idosos usando Edge AI e TinyML em ESP32-S3 Sense

⚠️ Aviso: Este projeto é um protótipo tecnológico/open source para estudo e experimentação. Não é um dispositivo médico certificado e não deve ser utilizado como único meio de monitoramento de saúde ou segurança.

📌 Índice
Visão Geral

Arquitetura

Hardware

Modelo de IA

Firmware

Configuração do Telegram

Monitoramento (Heartbeat)

Dataset e Treinamento

Instalação e Uso

Estrutura do Repositório

Contribuição

Licença

🧠 Visão Geral
ElderGuard Edge AI é um sistema de monitoramento baseado em visão computacional e TinyML que reconhece estados relacionados ao uso de uma cama e detecta eventos como quedas, tentativas de levantar e imobilidade prolongada.

O dispositivo executa um modelo de classificação de imagens localmente no microcontrolador Seeed Studio XIAO ESP32S3 Sense, enviando alertas via Telegram Bot e mantendo um heartbeat para monitoramento de disponibilidade.

Estados visuais detectados:
lying – pessoa deitada/dormindo

sitting – pessoa sentada

standing – pessoa em pé

on_floor – pessoa no chão

empty_bed – cama vazia

Eventos inferidos pela máquina de estados:
Dormindo normalmente

Acordou

Sentou na cama

Tentando levantar

Saiu da cama

Fora da cama por tempo incomum

Queda detectada

Permaneceu imóvel após queda

Tentativas repetidas de levantar sem sucesso

🏗️ Arquitetura

graph TD
    A[Seeed XIAO ESP32S3 Sense] --> B[Câmera integrada]
    B --> C[TinyML Model - TensorFlow Lite]
    C --> D[Firmware - Máquina de Estados]
    D --> E[Eventos]
    E --> F[Telegram Bot]
    E --> G[Better Stack / UptimeRobot]
    
    H[Wi-Fi] --> A
    I[USB-C 5V / Bateria] --> A

🔧 Hardware
Componente	Especificação
Microcontrolador	Seeed Studio XIAO ESP32S3 Sense
Câmera	OV2640 (integrada)
Conectividade	Wi-Fi 802.11 b/g/n
Alimentação	USB-C 5V ou bateria externa
Opções de energia:
Fonte USB-C 5V (recomendada para protótipo)

Power Bank portátil

Bateria 18650 com circuito de carga (futuro)

🧪 Modelo de IA
Classificação de imagens com 5 classes:

lying

sitting

standing

on_floor

empty_bed

Características:

Modelo: TensorFlow Lite Micro (quantizado INT8)

Tamanho: < 200KB

Precisão alvo: > 85% (em dados de validação)

Ferramenta de treinamento: Edge Impulse Studio

Lógica temporal (firmware):
A IA apenas classifica o frame atual. O firmware mantém um histórico e aplica uma máquina de estados finitos para inferir eventos complexos.

Exemplos:

lying → sitting → standing → tentativa de levantar

standing → on_floor → queda

on_floor por mais de X segundos → possível emergência

📦 Firmware
Desenvolvido em Arduino Framework (compatível com PlatformIO e Arduino IDE).

Bibliotecas utilizadas:
WiFi.h

esp_camera.h

UniversalTelegramBot.h

ArduinoJson.h

ESP32httpUpdate.h (OTA)

Funcionalidades:
Conexão Wi-Fi automática

Captura e redimensionamento da imagem

Inferência do modelo .tflite

Máquina de estados com temporizadores

Envio de alertas com imagem JPEG via Telegram

Heartbeat HTTP para Better Stack/UptimeRobot

OTA básico para atualizações

Salvamento de configuração em SPIFFS

🤖 Configuração do Telegram
Crie um bot no Telegram via @BotFather e obtenha o token.

Obtenha seu chat ID (pode usar o bot @userinfobot).

Configure as credenciais no arquivo config.h.

Exemplo de alerta:
text
🚨 ALERTA

Possível queda detectada
Quarto: 01
Confiança: 95%
A mensagem é acompanhada pela imagem JPEG capturada.

📡 Monitoramento (Heartbeat)
A cada 1 minuto, o dispositivo envia uma requisição HTTP GET para:

Better Stack (Uptime monitor) – via endpoint de heartbeat

UptimeRobot – via ping simples

Payload incluído (via query string ou corpo):

uptime (em segundos)

rssi (dBm)

version (firmware)

battery (se disponível)

Se o heartbeat falhar por mais de 5 minutos, a ferramenta de monitoramento gera um alerta de "Dispositivo offline".

📊 Dataset e Treinamento
Datasets recomendados (públicos e acadêmicos):
UP-Fall Detection Dataset

UR Fall Dataset

Le2i Fall Detection Dataset

COCO Dataset (para extrair pessoas)

MPII Human Pose Dataset

Script de preparação: tools/dataset_prepare.py
O script automatiza:

Extração de frames de vídeos

Redimensionamento para 96x96 (ou tamanho compatível)

Organização em pastas train/, validation/, test/

Estrutura de classes:

text
dataset/
  train/
    lying/
    sitting/
    standing/
    on_floor/
    empty_bed/
  validation/
    ...
  test/
    ...
Treinamento no Edge Impulse:
Crie uma conta no Edge Impulse

Crie um novo projeto

Faça upload das imagens (ou use o script para gerar um arquivo .zip)

Defina as 5 classes

Configure Image Classification como bloco de processamento

Use Transfer Learning (MobileNetV1 ou similar) e treine

Quantize para INT8 e exporte como Arduino library (TensorFlow Lite Micro)

📥 Instalação e Uso
Pré-requisitos:
Arduino IDE (>= 1.8.19) ou PlatformIO

Placa Seeed XIAO ESP32S3 Sense configurada

Cabo USB-C para upload

Passos:
Clone o repositório:

bash
git clone https://github.com/seu-usuario/elderguard-edge-ai.git
cd elderguard-edge-ai
Prepare o dataset e treine o modelo (conforme documentação em docs/training.md)

Instale as bibliotecas no Arduino IDE:

ESP32 (via Gerenciador de Placas)

UniversalTelegramBot (por Brian Lough)

ArduinoJson (por Benoit Blanchon)

Seeed_Arduino_Camera (ou esp32-camera)

Configure as credenciais no arquivo firmware/src/config.h:

cpp
#define WIFI_SSID     "sua-rede"
#define WIFI_PASSWORD "senha"
#define BOT_TOKEN     "seu-token-telegram"
#define CHAT_ID       "seu-chat-id"
#define HEARTBEAT_URL "https://betterstack.com/..." // opcional
Copie o modelo exportado (arquivo .tflite e .h) para a pasta firmware/src/.

Compile e faça upload para o ESP32.

Monitore via Serial Monitor (115200 bps).

📁 Estrutura do Repositório
text
ElderGuard-Edge-AI/
├── README.md                     # Este arquivo
├── LICENSE
├── .gitignore
├── docs/
│   ├── architecture.md           # Detalhamento da arquitetura
│   ├── dataset.md                # Guia de datasets e preparação
│   ├── training.md               # Passo a passo no Edge Impulse
│   └── hardware.md               # Esquema elétrico e montagem
├── firmware/
│   ├── platformio.ini            # (se usar PlatformIO)
│   └── src/
│       ├── ElderGuard.ino        # Código principal
│       ├── config.h              # Credenciais e configurações
│       ├── wifi_manager.h
│       ├── camera_wrapper.h
│       ├── classifier.h          # Interface com o modelo TFLite
│       ├── state_machine.h       # Máquina de estados
│       ├── telegram_notifier.h
│       ├── heartbeat.h
│       ├── ota_update.h
│       └── model.tflite.h        # Modelo quantizado (gerado pelo Edge Impulse)
├── tools/
│   ├── dataset_prepare.py        # Script para preparar o dataset
│   └── requirements.txt          # Dependências Python
└── examples/
    └── telegram_message.json     # Exemplo de mensagem de alerta
🤝 Contribuição
Contribuições são bem-vindas! Sinta-se à vontade para abrir issues e pull requests.

Antes de contribuir:

Leia as diretrizes de código.

Certifique-se de que seu modelo e firmware funcionam em hardware real.

Documente quaisquer alterações.

📄 Licença
Este projeto é licenciado sob a MIT License – veja o arquivo LICENSE para detalhes.

🏁 Próximos Passos
Suporte a bateria 18650 com monitoramento de carga

Modo de baixo consumo para operação por bateria

Reconhecimento de múltiplas pessoas (futuro)

Integração com Home Assistant (via webhook)


Desenvolvido com ❤️ para fins educacionais e de pesquisa.    