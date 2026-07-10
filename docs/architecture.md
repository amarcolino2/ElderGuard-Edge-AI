# Arquitetura do ElderGuard Edge AI

## Visao Geral

O sistema e composto por:

1. Dispositivo de borda (Seeed XIAO ESP32S3 Sense) com camera OV2640.
2. Classificador em dois modos: fake (padrao) ou pipeline real (embutido).
3. Maquina de estados para inferencia temporal de eventos.
4. Notificacao por Telegram.
5. Heartbeat HTTP para monitoramento externo.
6. Configuracao persistente em SPIFFS (`/config.json`).

## Fluxo de Dados

1. O loop principal captura um frame em QVGA para classificacao.
2. O classificador retorna um `VisualState` (`lying`, `sitting`, `standing`, `on_floor`, `empty_bed` ou `unknown`).
3. A maquina de estados aplica regras temporais e define evento critico quando necessario.
4. Em evento critico, o firmware opcionalmente captura foto em maior resolucao para enviar via Telegram.
5. Em paralelo:
   - envia heartbeat periodico com `uptime`, `rssi` e `version`
   - executa checagem OTA periodica.

## Modos de Classificacao

- **Modo fake (padrao)**: simula mudancas de estado para testes de fluxo.
- **Modo real**: ponto de entrada preparado para usar `model.tflite.h` com pipeline TinyML real.

Chaveamento via `firmware/src/config.h`:

- `USE_FAKE_CLASSIFIER`
- `USE_REAL_INFERENCE_PIPELINE`

## Maquina de Estados (Resumo)

Regras principais implementadas:

- `standing -> on_floor` em intervalo curto: alerta de possivel queda.
- `on_floor` por mais de 30s: alerta de imobilidade.
- sequencia `lying -> sitting -> standing`: tentativa de levantar.
- `empty_bed` por periodo longo: pessoa fora da cama.

## Configuracao e Persistencia

`config_store` carrega/salva em SPIFFS:

- `wifi_ssid`
- `wifi_password`
- `bot_token`
- `chat_id`
- `heartbeat_url`
- `ota_url`

Se `config.json` nao existir ou estiver invalido, defaults de `config.h` sao gravados automaticamente.

## Comunicacao

- Wi-Fi para conectividade.
- HTTPS para Telegram API.
- HTTP GET para heartbeat.
- HTTP para OTA (`HTTPUpdate.h`).

## Escalabilidade

Projeto atual e monodispositivo. Pode ser expandido para multiplos comodos e sensores externos via `onExternalEvent`.