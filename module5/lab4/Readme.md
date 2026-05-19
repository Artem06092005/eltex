# Обмен информацией с userspace через chardev   

## Сборка:  
make    
## Установка модуля:    
sudo insmod chardev.ko  
![](screenshots/build.jpeg) 

## Ввод вывод в chardev(под root):  
echo "text" > /dev/chardev  
cat /dev/chardev    
![](screenshots/io.jpeg) 

## Выгрузка:
sudo rmmod chardev.ko