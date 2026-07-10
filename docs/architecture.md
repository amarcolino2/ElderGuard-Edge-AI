# Arquitetura do ElderGuard Edge AI

## Visão Geral de Alto Nível

O sistema é composto por:

1. **Dispositivo de borda** (ESP32-S3 Sense) com câmera integrada
2. **Modelo TinyML** (TensorFlow Lite Micro) rodando localmente
3. **Máquina de estados** no firmware para interpretação temporal
4. **Telegram Bot** para notificações
5. **Serviço de monitoramento** (Better Stack/UptimeRobot) para heartbeat

## Fluxo de Dados

1. A câmera captura uma imagem a cada `X` segundos (configurável).
2. A imagem é redimensionada para o formato esperado pelo modelo (ex: 96x96 RGB).
3. O modelo classifica a imagem em uma das 5 classes.
4. A classe é passada para a máquina de estados que mantém um histórico e aplica regras temporais.
5. Quando um evento crítico é detectado (queda, imobilidade, etc.), o firmware:
   - Captura uma imagem de alta resolução
   - Envia uma mensagem com a imagem via Telegram
6. Simultaneamente, a cada 1 minuto, o firmware envia um heartbeat para o serviço de monitoramento.

## Máquina de Estados (Simplificada)

Estado atual: [classe da imagem]
Histórico: últimas N classificações
Temporizadores: tempo em cada estado

Regras:

lying + sitting + standing → tentativa de levantar

standing + on_floor (dentro de 2s) → queda

on_floor por > 30s → imobilidade após queda

empty_bed por > 5min → saiu da cama

tentativas repetidas de levantar (3x em 1min) → alerta de dificuldade


## Comunicação

- **Wi-Fi** para acesso à internet
- **HTTPS** para Telegram API (envio de mensagens e fotos)
- **HTTP GET** para heartbeat (Better Stack)

## Segurança

- As credenciais (Wi-Fi, Token, Chat ID) são armazenadas em SPIFFS ou no código (para protótipo).
- Recomenda-se criptografar as credenciais em produção.

## Escalabilidade

O projeto é monodispositivo, mas pode ser replicado para vários cômodos, cada um com seu próprio bot Telegram e heartbeat.