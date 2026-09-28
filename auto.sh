#!/bin/bash

# Диапазон и шаг
START=-10
END=30
STEP=0.05

# Файл с входными данными
INPUT_FILE="../test.json"

# Счетчик для прогресса
count=0
total=$(echo "($END - $START) / $STEP + 1" | bc)

# Цикл по дБ
for snr in $(seq $START $STEP $END); do
    # Передаем имя файла и SNR через stdin
    printf "%s\n%s\n" "$INPUT_FILE" "$snr" | ./pucchf2
    
    # Проверяем код возврата
    if [ $? -ne 0 ]; then
        echo "Ошибка при SNR = $snr дБ" >&2
        exit 1
    fi
    
    # Прогресс
    count=$((count + 1))
    echo "[$count/$total] SNR = $snr дБ"
done

echo "Готово!"