# Hardware - Seeed Studio XIAO ESP32S3 Sense

## Especificações

- **SoC**: ESP32-S3 (Xtensa LX7 dual-core, 240MHz)
- **Memória**: 512KB SRAM, 8MB PSRAM, 16MB Flash
- **Câmera**: OV2640 (2MP), interface DVP
- **Conectividade**: Wi-Fi 802.11 b/g/n, Bluetooth 5.0
- **GPIOs**: 11 pinos (incluindo I2C, SPI, UART, ADC)
- **Alimentação**: 5V via USB-C ou bateria Li-Po (conector JST)

## Pinagem da Câmera (já integrada)

A câmera OV2640 é conectada internamente aos seguintes pinos:

| Sinal | Pino ESP32 |
|-------|------------|
| D0..D7 | GPIO4, GPIO5, GPIO6, GPIO7, GPIO15, GPIO16, GPIO17, GPIO18 |
| VSYNC | GPIO14 |
| HREF  | GPIO27 |
| PCLK  | GPIO13 |
| XCLK  | GPIO10 |
| SIOD  | GPIO21 |
| SIOC  | GPIO22 |
| RESET | GPIO9 |
| PWDN  | -1 (não usado) |

## Esquema de Alimentação

### Opção 1: USB-C 5V (recomendada)
Conecte o cabo USB-C a uma fonte de 5V/2A. O ESP32 consome ~200mA em operação normal.

### Opção 2: Power Bank
Utilize um power bank com saída USB-A para USB-C. Verifique se suporta corrente contínua.

### Opção 3: Bateria 18650 (futuro)
- Use um módulo carregador TP4056 com proteção.
- Conecte a saída do módulo ao pino `5V` do XIAO (via diodo para evitar retroalimentação).
- O XIAO suporta entrada de 3.7V a 5.5V no pino `BAT` (se disponível) ou via `5V`.

### Medição de bateria (futuro)
Use um divisor resistivo no pino ADC (GPIO8) para medir a tensão da bateria e enviar no heartbeat.

## Montagem Física

- O XIAO Sense já possui a câmera soldada.
- Posicione o dispositivo em um ângulo que cubra a cama e o chão ao redor.
- Fixe com fita dupla-face ou suporte impresso em 3D (modelo disponível em breve).

## Cuidados

- Evite luz solar direta na lente.
- Mantenha o dispositivo em local seco e ventilado.