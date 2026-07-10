# Treinamento do Modelo (Edge Impulse)

## Objetivo

Gerar modelo de classificacao para as classes:

- lying
- sitting
- standing
- on_floor
- empty_bed

## Passo a passo

1. Crie projeto no Edge Impulse.
2. Faca upload do dataset (veja `docs/dataset.md`).
3. Defina split treino/validacao/teste.
4. Configure pipeline de imagem (ex.: 96x96 RGB).
5. Treine com Transfer Learning.
6. Avalie matriz de confusao e acuracia por classe.
7. Exporte em TensorFlow Lite Micro (INT8).

## Parametros sugeridos (ponto de partida)

- Epochs: 30
- Learning rate: 0.001
- Batch size: 32

Ajuste conforme overfitting/underfitting.

## Exportacao para firmware

1. Exporte em formato compativel com TFLite Micro.
2. Gere/coloque `model.tflite.h` em `firmware/src/`.
3. Ative modo real no classificador:

```cpp
// #define USE_FAKE_CLASSIFIER 1
#define USE_REAL_INFERENCE_PIPELINE 1
```

4. Implemente/valide preprocessamento e pos-processamento no pipeline real.

## Validacao no dispositivo

- Compare saida de classificacao com cenas reais.
- Ajuste thresholds e regras da maquina de estados.
- Teste em diferentes iluminacoes e horarios.
