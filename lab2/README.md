# Лабораторная работа 2

**Название:** "Разработка драйверов блочных устройств"

**Цель работы:** Получить знания и навыки разработки драйверов блочных устройств для операционной системы Linux.

## Описание функциональности драйвера

Драйвер создаёт виртуальный жёсткий диск в оперативной памяти размером 50 Мбайт и разбивает его на 3 первичных раздела объёмами 10, 25 и 15 Мбайт. Каждый записываемый байт возводится в куб.

## Инструкция по сборке

1. Собрать проект, используя Makefile: `make`
2. Загрузить модуль в ядро: `insmod main.ko`
3. Выгрузить модуль из ядра: `rmmod main.ko`

## Инструкция пользователя

После загрузки модуля появятся файлы `/dev/vramdisk` и `/dev/vramdisk<1-3>`.
Записать информацию на диск можно с помощью команды `dd`. Считать информацию можно с помощью команды `hexdump`.

## Примеры использования

### Вывод разделов созданного виртуального диска

```shell
artix20210101:[root]:~/Documents/lab2# fdisk -l /dev/vramdisk
Disk /dev/vramdisk: 50 MiB, 52428800 bytes, 102400 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x36e5756d

Device         Boot Start    End Sectors Size Id Type
/dev/vramdisk1          1  20479   20479  10M 83 Linux
/dev/vramdisk2      20480  71679   51200  25M 83 Linux
/dev/vramdisk3      71680 102399   30720  15M 83 Linux
```

### Запись и чтение данных в созданный виртуальный диск

```shell
artix20210101:[root]:~/Documents/lab2# hexdump /dev/vramdisk1
0000000 0000 0000 0000 0000 0000 0000 0000 0000
*
09ffe00

artix20210101:[root]:~/Documents/lab2# echo "123456789" | dd of=/dev/vramdisk1
0+1 records in
0+1 records out
10 bytes copied, 0.00118081 s, 8.5 kB/s
artix20210101:[root]:~/Documents/lab2# hexdump /dev/vramdisk1
0000000 4891 402b 188d 00e7 e869 0000 0000 0000
0000010 0000 0000 0000 0000 0000 0000 0000 0000
*
09ffe00
```

### Измерение скорости передачи данных при копировании файлов между разделами созданного виртуального диска

```shell
artix20210101:[root]:~/Documents/lab2# dd if=/dev/vramdisk1 of=/dev/vramdisk2
20479+0 records in
20479+0 records out
10485248 bytes (10 MB, 10 MiB) copied, 0.153903 s, 68.1 MB/s
```

### Измерение скорости передачи данных при копировании файлов между разделами виртуального и реального жестких дисков

```shell
artix20210101:[root]:~/Documents/lab2# dd of=/dev/vramdisk1 if=/dev/sda2
dd: writing to '/dev/vramdisk1': No space left on device
20480+0 records in
20479+0 records out
10485248 bytes (10 MB, 10 MiB) copied, 0.121399 s, 86.4 MB/s

artix20210101:[root]:~/Documents/lab2# dd if=/dev/vramdisk1 of=/dev/sda3
20479+0 records in
20479+0 records out
10485248 bytes (10 MB, 10 MiB) copied, 0.0375259 s, 279 MB/s
```
