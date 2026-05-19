# Обмен информацией с userspace через chardev   

## Сборка:  
make    
## Установка модуля:    
sudo insmod kernelspace.ko 
![](screenshots/build.jpeg) 

## Запуск userspace:  
./userspace  

![](screenshots/start.jpeg) 

## Выгрузка:
sudo rmmod kernelspace.ko 

## Очистка  
make clean