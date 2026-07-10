# 🤖 Como o Telegram recebe os eventos (Explicação técnica)
O firmware do ESP32 não fica "escutando" mensagens do Telegram. Ele só envia requisições HTTP para a API do Telegram.

Fluxo de recebimento:

O ESP32 detecta um evento (ex: queda).

Ele monta uma mensagem com o texto e a foto.

Ele faz uma requisição POST para: https://api.telegram.org/bot<SEU_TOKEN>/sendPhoto

O servidor do Telegram recebe essa requisição e entrega a mensagem diretamente no seu aplicativo (no celular/PC), porque você configurou o CHAT_ID (que é o identificador único da sua conta ou do seu grupo).

📝 Passo a passo (Recapitulando e detalhando)
1. Criar o Bot e pegar o Token
Abra o Telegram e procure por @BotFather.

Envie o comando: /newbot

Escolha um nome (ex: ElderGuard_Alert_Bot) e um username (ex: elderguard_alert_bot).

O BotFather vai te devolver uma mensagem com o Token. É algo como:
1234567890:ABCdefGHIjklMNOpqrsTUVwxyz

Guarde esse token – ele vai no config.h.

2. Pegar o seu Chat ID
Abra o Telegram e procure por @userinfobot.

Inicie uma conversa com ele (clique em "Start" ou envie qualquer mensagem).

Ele vai te responder com seu ID. É um número puro, tipo: 987654321.

Guarde esse número – ele vai no config.h.

3. Configurar no código (config.h)

cpp
// config.h
#define BOT_TOKEN     "1234567890:ABCdefGHIjklMNOpqrsTUVwxyz"
#define CHAT_ID       "987654321"
4. Como a mensagem chega até você (Recebimento)
Quando o ESP32 executar a função que te mostrei no código:

cpp
// Função que envia o alerta
void send_telegram_alert(const String& message, bool withPhoto) {
  if (withPhoto) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
      bot.sendPhoto(CHAT_ID, fb->buf, fb->len, message); // <--- Envia para SEU CHAT_ID
      esp_camera_fb_return(fb);
    }
  } else {
    bot.sendMessage(CHAT_ID, message);
  }
}
Resultado prático: Assim que o ESP32 capturar a imagem e chamar essa função, instantaneamente você verá a notificação pop-up no seu celular, exatamente como se fosse uma mensagem de um contato normal.

⚠️ Pontos de atenção para você não errar:
O Bot precisa ser iniciado: Você precisa mandar uma mensagem qualquer (ex: "/start") para o seu próprio bot (o que você criou no @BotFather) antes de tentar receber mensagens. Se o bot nunca foi iniciado por você, o Telegram bloqueia o envio.

Dica: Abra o chat com @elderguard_alert_bot, clique em "Start", e pronto.

Certificado SSL: O código usa WiFiClientSecure. No config.h, coloquei o certificado raiz do Telegram. Se der erro de SSL na hora de enviar, você pode desabilitar a verificação (apenas para testes) usando:

cpp
client.setInsecure(); // Coloque isso antes de usar o bot
Mensagem com imagem: Para o envio de foto funcionar, o buffer da câmera (fb->buf) deve estar em formato JPEG. O código de inicialização da câmera que te passei já está configurado com pixel_format = PIXFORMAT_JPEG, então está tudo certo.

📱 Exemplo de como chega no seu celular
Quando o evento de queda for detectado, você receberá isso no Telegram:

🚨 ALERTA

Possível queda detectada
Quarto: 01
Confiança: 95%

(e logo abaixo virá a foto que a câmera tirou na hora)