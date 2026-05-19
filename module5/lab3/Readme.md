# kbleds — мигание светодиодами клавиатуры

## Сборка:
make
## Установка модуля:
sudo insmod kbleds.ko
![](screenshots/build.jpeg)

## Изменяем значение в sysfs:
![](screenshots/sysfs.jpeg)
echo 1 >  /sys/kernel/systest/test
## Выгрузка:
sudo rmmod kbleds.ko