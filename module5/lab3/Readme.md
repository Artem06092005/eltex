# kbleds — мигание светодиодами клавиатуры

## Сборка:
make
## Установка модуля:
sudo insmod kbleds.ko
![](screenshots/build.jpeg)

## Изменяем значение в sysfs:
echo 1 >  /sys/kernel/systest/test
![](sysfs.jpeg)

## Выгрузка:
sudo rmmod kbleds.ko