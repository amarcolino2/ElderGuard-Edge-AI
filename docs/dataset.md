# Preparacao do Dataset

## Objetivo

Montar um dataset para classificacao das classes:

- lying
- sitting
- standing
- on_floor
- empty_bed

## Datasets Publicos Recomendados

Nao redistribuimos datasets. Use as fontes oficiais:

1. UP-Fall Detection Dataset
- URL: http://sites.usp.br/lamu/up-fall-detection-dataset/

2. UR Fall Dataset
- URL: http://fenix.univ.rzeszow.pl/~mkepski/ds/uf.html

3. Le2i Fall Detection Dataset
- URL: http://le2i.cnrs.fr/Fall-detection-Dataset

4. COCO Dataset
- URL: https://cocodataset.org/

5. MPII Human Pose Dataset
- URL: http://human-pose.mpi-inf.mpg.de/

## Script de Preparacao

O script `tools/dataset_prepare.py` organiza imagens e redimensiona para o tamanho esperado.

Exemplo de uso:

```bash
pip install -r tools/requirements.txt
python tools/dataset_prepare.py --input /caminho/para/videos --output ./dataset --size 96
```

## Estrutura Sugerida

```text
dataset/
    train/
        lying/
        sitting/
        standing/
        on_floor/
        empty_bed/
    validation/
        lying/
        sitting/
        standing/
        on_floor/
        empty_bed/
    test/
        lying/
        sitting/
        standing/
        on_floor/
        empty_bed/
```

## Boas Praticas

- Balanceie o numero de imagens entre classes.
- Inclua variacoes de iluminacao e angulo.
- Evite overfitting com augmentacao.
- Mantenha conjunto de teste separado e sem vazamento.

## Proximo Passo

Depois da preparacao, siga o guia em `docs/training.md`.