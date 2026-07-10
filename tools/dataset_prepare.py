#!/usr/bin/env python3
import os
import cv2
import shutil
import argparse
import random
from sklearn.model_selection import train_test_split
from tqdm import tqdm

def extract_frames(video_path, output_dir, class_name, interval=30, size=(96,96)):
    cap = cv2.VideoCapture(video_path)
    count = 0
    extracted = 0
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        if count % interval == 0:
            # Redimensionar
            resized = cv2.resize(frame, size, interpolation=cv2.INTER_AREA)
            out_filename = f"{class_name}_{extracted:06d}.jpg"
            out_path = os.path.join(output_dir, class_name, out_filename)
            cv2.imwrite(out_path, resized)
            extracted += 1
        count += 1
    cap.release()
    return extracted

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Pasta raiz com subpastas: ur_fall, le2i_fall, coco, mpii')
    parser.add_argument('--output', default='./dataset_final', help='Pasta de saída')
    parser.add_argument('--size', type=int, default=96, help='Tamanho da imagem (quadrado)')
    parser.add_argument('--interval', type=int, default=30, help='Intervalo entre frames')
    args = parser.parse_args()

    # Criar estrutura de saída
    for split in ['train', 'validation', 'test']:
        for cls in ['lying', 'sitting', 'standing', 'on_floor', 'empty_bed']:
            os.makedirs(os.path.join(args.output, split, cls), exist_ok=True)

    # 1. Processar vídeos da pasta UR Fall
    ur_path = os.path.join(args.input, 'ur_fall')
    if os.path.exists(ur_path):
        print("Processando UR Fall...")
        for video_file in os.listdir(ur_path):
            if video_file.endswith('.avi'):
                # Definir classe com base no nome do arquivo (ex: "fall-01.avi" -> on_floor)
                # Ajuste conforme sua organização
                if 'fall' in video_file.lower():
                    cls = 'on_floor'
                else:
                    cls = 'standing'  # exemplo, adapte
                video_full = os.path.join(ur_path, video_file)
                extract_frames(video_full, args.output, cls, args.interval, (args.size, args.size))

    # 2. Processar vídeos Le2i (similar)
    le2i_path = os.path.join(args.input, 'le2i_fall')
    if os.path.exists(le2i_path):
        print("Processando Le2i...")
        # Lógica similar

    # 3. Processar imagens COCO (usando anotações para filtrar)
    # Para simplificar, você pode copiar manualmente imagens de pessoas sentadas, etc.
    # Ou usar um script que leia as anotações e selecione as imagens com base nas poses.
    # Como isso é complexo, sugiro que, para este protótipo, você use apenas vídeos
    # para as classes principais, e complemente com algumas imagens do COCO que você
    # baixar manualmente.

    # 4. Dividir em treino/validação/teste (80/10/10)
    # O script de divisão deve percorrer as pastas de cada classe e mover aleatoriamente
    # uma porcentagem para validation e test.

    print("Dataset preparado em:", args.output)

if __name__ == '__main__':
    main()