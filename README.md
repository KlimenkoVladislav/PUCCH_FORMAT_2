# PUCCH_FORMAT_2

## Сборка и запуск

``` bash
git clone https://github.com/KlimenkoVladislav/PUCCH_FORMAT_2.git
mkdir build && cd build
cmake ..
make
./pucchf2
```

## Многократный автоматический апуск для анализа BLER

``` bash
python3 -m venv venv
source venv/bin/activate
pip install matplotlib numpy

cd build
chmod +x ../auto.sh
./../auto.sh
python ../BLER/BLER.py
```