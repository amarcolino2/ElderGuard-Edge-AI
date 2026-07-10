# Preparação do Dataset

## Datasets Públicos Recomendados

Para treinar o modelo, utilizamos datasets de detecção de quedas e poses humanas. **Não redistribuímos os datasets** – fornecemos scripts para baixar e processar.

### 1. UP-Fall Detection Dataset
- **URL**: http://sites.usp.br/lamu/up-fall-detection-dataset/
- **Descrição**: Contém vídeos de atividades diárias e quedas simuladas.
- **Uso**: Extrair frames das cenas com cama e pessoa.

### 2. UR Fall Dataset
- **URL**: http://fenix.univ.rzeszow.pl/~mkepski/ds/uf.html
- **Descrição**: Vídeos de quedas em diferentes cenários.

### 3. Le2i Fall Detection Dataset
- **URL**: http://le2i.cnrs.fr/Fall-detection-Dataset
- **Descrição**: Diversas situações de queda.

### 4. COCO Dataset
- **URL**: https://cocodataset.org/
- **Uso**: Extrair imagens com pessoas em diferentes poses, filtradas por categorias (sentado, em pé, deitado).

### 5. MPII Human Pose Dataset
- **URL**: http://human-pose.mpi-inf.mpg.de/
- **Uso**: Imagens com anotações de pose, úteis para identificar posições.

## Script de Preparação

O script `tools/dataset_prepare.py` automatiza a criação das pastas e o redimensionamento.

### Uso:

```bash
pip install -r tools/requirements.txt
python tools/dataset_prepare.py --input /caminho/para/videos --output ./dataset --size 96

Estrutura de Pastas Gerada:

dataset/
├── train/
│   ├── lying/
│   ├── sitting/
│   ├── standing/
│   ├── on_floor/
│   └── empty_bed/
├── validation/
│   └── (mesmas classes)
└── test/
    └── (mesmas classes)

Exemplo de Código do Script (resumo)

import os, cv2, shutil, argparse
from sklearn.model_selection import train_test_split

def extract_frames(video_path, output_dir, class_name, frame_interval=30):
    # extrai frames e salva na pasta correta
    pass

def resize_images(input_dir, output_dir, size=(96,96)):
    # redimensiona todas as imagens
    pass

# ... (lógica de divisão treino/validação/teste)

Importação no Edge Impulse
Compacte a pasta dataset em um arquivo .zip.

No Edge Impulse, vá em Data acquisition e faça upload do .zip.

O sistema automaticamente reconhecerá as pastas como labels.

Divida em treino/teste (80/20) dentro da plataforma.


---

## 3. Treinamento (`docs/training.md`)

```markdown
# Treinamento do Modelo no Edge Impulse

## 1. Criar Conta e Projeto

- Acesse [Edge Impulse](https://edgeimpulse.com/) e crie uma conta gratuita.
- Crie um novo projeto com nome `ElderGuard`.

## 2. Upload dos Dados

- Na aba **Data acquisition**, clique em **Upload data**.
- Selecione o arquivo `.zip` gerado pelo script de preparação.
- Após o upload, verifique se as classes estão corretas.

## 3. Dividir Dados

- Na aba **Data acquisition**, use a opção **Train/Test split** para separar 80% para treino e 20% para teste.

## 4. Pré-processamento

- No bloco **Impulse design**, adicione **Image** como bloco de processamento.
- Escolha o tamanho da imagem (ex: 96x96) e o modo de cor (RGB).
- Adicione **Transfer Learning** como bloco de aprendizado.
- Escolha o modelo base (MobileNetV1 0.25) – otimizado para microcontroladores.

## 5. Treinamento

- Vá para a aba **Transfer Learning**.
- Defina os parâmetros:
  - Número de épocas: 30
  - Taxa de aprendizado: 0.001
  - Tamanho do lote: 32
- Clique em **Start training**.

## 6. Avaliação e Teste

- Após o treino, verifique a acurácia no conjunto de teste.
- Faça ajustes (aumentar dados, mudar arquitetura) se necessário.

## 7. Quantização e Exportação

- Vá para a aba **Deployment**.
- Selecione **TensorFlow Lite Micro**.
- Marque a opção **Quantized (INT8)**.
- Clique em **Build**.
- Baixe o arquivo `.zip` contendo a biblioteca Arduino.

## 8. Integração com o Firmware

- Extraia o `.zip` e copie o arquivo `model.tflite` (ou `model.cpp/.h`) para a pasta `firmware/src/`.
- O firmware incluirá esse arquivo e usará a classe `ElasticLink` ou similar para executar a inferência.

## Dicas

- Para melhorar a acurácia, use **Data augmentation** (rotação, brilho, zoom) diretamente no Edge Impulse.
- Monitore o overfitting com a curva de validação.