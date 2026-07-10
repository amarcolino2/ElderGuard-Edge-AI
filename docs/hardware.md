# Hardware - Seeed Studio XIAO ESP32S3 Sense

## Especificacoes

- SoC: ESP32-S3 (dual-core, ate 240MHz)
- Memoria: SRAM + PSRAM (conforme variante da placa)
- Camera: OV2640 integrada
- Conectividade: Wi-Fi 2.4GHz
- Alimentacao: USB-C 5V (recomendado para prototipo)

## Pinagem de Camera usada no firmware

A configuracao atual em `firmware/src/camera_wrapper.cpp` usa:

| Sinal | GPIO |
|-------|------|
| XCLK | 10 |
| SIOD | 40 |
| SIOC | 39 |
| D0 (Y2) | 15 |
| D1 (Y3) | 17 |
| D2 (Y4) | 18 |
| D3 (Y5) | 16 |
| D4 (Y6) | 14 |
| D5 (Y7) | 12 |
| D6 (Y8) | 11 |
| D7 (Y9) | 48 |
| VSYNC | 38 |
| HREF | 47 |
| PCLK | 13 |
| RESET | -1 |
| PWDN | -1 |

Observacao: se sua revisao da placa diferir, ajuste esses valores no wrapper.

## Parametros de camera adotados

- `pixel_format = PIXFORMAT_JPEG`
- `frame_size = FRAMESIZE_QVGA` (captura de inferencia)
- `fb_count = 2` com PSRAM, senao `1`
- `grab_mode = CAMERA_GRAB_WHEN_EMPTY`
- retry de captura (ate 3 tentativas)

## Alimentacao

### Opcao 1: USB-C 5V (recomendada)

Conecte em fonte estavel de 5V/2A.

### Opcao 2: Power bank

Use power bank que mantenha saida ativa com baixo consumo.

### Opcao 3: bateria (futuro)

Pode ser implementado com circuito de carga/protecao e monitoramento via ADC.

## Montagem fisica

- Posicione a camera para cobrir cama e area de risco de queda.
- Evite contraluz forte e reflexos diretos.
- Prenda o modulo para minimizar vibracao/trepidacao.

