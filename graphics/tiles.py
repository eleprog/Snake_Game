file_input = open('font 14x10.bmp', "rb")
file_output = open("../board computer/board computer/font 14x10.h", "w")

def Data_Read(adr, len):
    file_input.seek(adr)
    data = bytearray(file_input.read(len))
    per = 0
    for i in range(len):
        per |= data[i] << (8*i)
    return per

head = Data_Read(0x0A, 4)
tale = Data_Read(0x02, 4)
size = tale - head

# width должна быть кратной 4 в большую сторону
width = Data_Read(0x12, 2)
if width % 4:
    width += 4 - width % 4

height = size // width

file_input.seek(head)
mas = [[int(0)] * width for i in range(height)]

for i in range(height):
    for j in range(width):
       mas[height - i - 1][j] = file_input.read(1)

file_output.write('uint8_t font_14x10[10][2][10] = { \n')
for number in range(10):
    file_output.write('{')
    for line in range(2):
        file_output.write('{') 
        for i in range(10):
            data_byte = 0
            for j in range(8):
                if mas[j + 8 * line][i + 12*number] < bytes(0x8F):
                    data_byte |= 1<<j
            if i < 9:
                file_output.write(hex(data_byte) + ', ')
            elif line < 1:
                file_output.write(hex(data_byte) + '},\n')
            elif number < 9:
                file_output.write(hex(data_byte) + '}},\n\n')
            else:
                file_output.write(hex(data_byte) + '}}};')

file_input.close()
file_output.close()