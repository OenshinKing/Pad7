from random import random

file1 = open("1IO/DEC1024_L.txt", 'w')
file2 = open("1IO/HEX1024_L.txt",'w')

file3 = open("1IO/DEC1024_R.txt", 'w')
file4 = open("1IO/HEX1024_R.txt",'w')

for j in range(0,50):
    num_L = ''
    num_R = ''

    for i in range(1,1025):
        if random() > 0.5:
            num_L = num_L + '1'
        else:
            num_L = num_L + '0'

        if random() > 0.5:
            num_R = num_R + '1'
        else:
            num_R = num_R + '0'

        if (i % 8) == 0:
            num_L = num_L + ','
            num_R = num_R + ','

    segments_L = num_L.split(',')[:-1]
    segments_R = num_R.split(',')[:-1]

    num_L = ''
    num_R = ''

    for i in segments_L:
        num_L = int(i,2)
        file1.write(format(num_L,'d') + ',')
        file2.write(format(num_L,'X') + ',')
    file1.write('\n')
    file2.write('\n')

    for i in segments_R:
        num_R = int(i,2)
        file3.write(format(num_R,'d') + ',')
        file4.write(format(num_R,'X') + ',')
    file3.write('\n')
    file4.write('\n')

file1.close()
file2.close()
file3.close()
file4.close()
