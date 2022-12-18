file_input = open('tiles.bmp', "rb")
file_output = open("../Snake_LCD5110/Snake_LCD5110/tiles.h", "w")

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

file_output.write('// адреса тайлов\n')
file_output.write('#define TILE_BODY\t0\n')
file_output.write('#define TILE_NULL\t6\n')
file_output.write('#define TILE_FOOD\t7\n')
file_output.write('#define TILE_TURN\t8\n')
file_output.write('#define TILE_HEAD\t16\n')
file_output.write('#define TILE_TAIL\t24\n')
file_output.write('#define TILE_BONUS\t28\n\n')

file_output.write('const uint8_t tiles[40][2] PROGMEM = {\n')


for vertical in range (5):
    vertical *= 5
    for horizontal in range(8):
        horizontal *= 5

        dataByte = 0
        for bit in range(8):       
            if mas[vertical + bit % 4][horizontal + bit // 4] < bytes(0x0F):
                dataByte |= 1<<bit
        file_output.write('\t\t{ ' + hex(dataByte) + ', ')

        dataByte = 0
        for bit in range(8):
            
            if mas[vertical + bit % 4][horizontal + bit // 4 + 2] < bytes(0x0F):
                dataByte |= 1<<bit

        if (vertical / 5) * (horizontal / 5) < (5 - 1) * (8 - 1):
            file_output.write(hex(dataByte) + ' },\n')
        else:
            file_output.write(hex(dataByte) + ' }};')



file_input.close()
file_output.close()