# Лабораторная работа 3

**Название:** "Разработка драйверов сетевых устройств"

**Цель работы:** Получить знания и навыки разработки драйверов сетевых интерфейсов для операционной системы Linux.

## Описание функциональности драйвера

Разработанный драйвер создает виртуальный сетевой интерфейс, который перехватывает UDP-пакеты и выводит следующую информацию о перехваченном пакете:

* Адрес отправителя (saddr)
* Адрес получателя (daddr)
* Длину сообщения (data length)
* Перехваченное сообщение (data)

## Инструкция по сборке

1. Собрать проект, используя Makefile: `make`
2. Загрузить модуль в ядро: `insmod main.ko`
3. Выгрузить модуль из ядра: `rmmod main.ko`

## Инструкция пользователя

Для отправки UDP-пакетов используется команда `echo -n <INPUT> > /dev/udp/<HOST>/<PORT>`.
Перехваченный драйвером пакет можно увидеть в кольцевом буфере ядра при помощи команды `dmesg -T`.

## Примеры использования

### Установка модуля и проверка его успешного запуска

```shell
artix20210101:[root]:~/Documents/lab3# make
artix20210101:[root]:~/Documents/lab3# insmod main.ko
artix20210101:[root]:~/Documents/lab3# dmesg
[ 2128.408791] Module main unloaded
[ 2135.147613] Module main loaded
[ 2135.147614] main: create link vni0
[ 2135.147615] main: registered rx handler for lo
[ 2135.174594] vni0: device opened
```

### Проверка перехвата UDP-пакетов с наличием и без наличия указанной подстроки

```shell
artix20210101:[root]:~/Documents/lab3# sudo insmod main.ko rqr_str="my_str52"
artix20210101:[root]:~/Documents/lab3# dmesg -C
artix20210101:[root]:~/Documents/lab3# echo -n "my_str" > /dev/udp/127.0.2.1/12345
artix20210101:[root]:~/Documents/lab3# echo -n "my_str52125125" > /dev/udp/127.0.2.1/12345
artix20210101:[root]:~/Documents/lab3# dmesg
[ 2152.570417] Captured UDP datagram, saddr: 127.0.0.1
[ 2152.570419] daddr: 127.0.2.1
[ 2152.570420] Data length: 14. Data: my_str52125125
```
