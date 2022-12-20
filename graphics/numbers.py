file_input = open('numbers.bmp', "rb")
file_output = open("../Snake_LCD5110/Snake_LCD5110/graphics/numbers.h", "w")

# функция считывания данных из файла
def Data_Read(adr, len):
    file_input.seek(adr)
    data = bytearray(file_input.read(len))
    per = 0
    for i in range(len):
        per |= data[i] << (8*i)
    return per

# считывание данных о размере картинки
head = Data_Read(0x0A, 4)
tale = Data_Read(0x02, 4)
size = tale - head

# считывание ширины
# (должна быть кратной 4 в большую сторону)
width = Data_Read(0x12, 2)
if width % 4:
    width += 4 - width % 4

# вычисление высоты
height = size // width

# перемещение указателя на начало данных картинки
file_input.seek(head)

# объявление массива размером с картинку
mas = [[int(0)] * width for i in range(height)]

# запись пикселей картинки в массив
for i in range(height):
    for j in range(width):
       mas[height - i - 1][j] = file_input.read(1)

# генерация файла с массивом данных
file_output.write('#include <avr/pgmspace.h>\n\n')

file_output.write('const uint8_t numbers[10][3] PROGMEM = {\n')

for number in range (10):
    for byte in range(3):
        dataByte = 0
        for bit in range(8):
            if mas[bit][number * 4 + byte] < bytes(0x0F):
                dataByte |= 1<<bit
        if byte == 0:
            file_output.write('\t\t{ ' + hex(dataByte) + ', ')
        elif byte == 1:
            file_output.write(hex(dataByte) + ', ')
        elif number < 9:
            file_output.write(hex(dataByte) + '},\n')
        else:
            file_output.write(hex(dataByte) + '}};')

file_input.close()
file_output.close()