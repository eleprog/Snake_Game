file = open("C:/Users/User/Desktop/graphics.bmp", "rb")

#Считывание из файла данных
def Data_Read(adr, l):
    file.seek(adr)
    data = bytearray(file.read(l))
    per = 0
    for i in range(l):
        per |= data[i] << (8*i) 
    return per
    
head = Data_Read(0x0A, 4)
tale = Data_Read(0x02, 4)
width = Data_Read(0x12, 2)
height = int((tale - head) / width)
size = tale - head - 1

file.seek(head)
mas = bytearray(file.read(tale-head))

print('Начало массива: ' + str(head))
print('Конец массива: ' + str(tale))
print('Ширина массива: ' + str(width))
print('Высота массива: ' + str(height))

for i in range(height):
    for j in range(width):
        if mas[size - width -(-j + i*width)] > 0x80:
            print(' ', end='')
        else:
            print('#', end='')
    print()
file.close()
