from random import random

file_L = open("HEX1024_L.txt",'w')
file_R = open("HEX1024_R.txt",'w')

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
        file_L.write(format(num_L,'X').zfill(2) + ',')
    file_L.write('\n')

    for i in segments_R:
        num_R = int(i,2)
        file_R.write(format(num_R,'X').zfill(2) + ',')
    file_R.write('\n')

file_L.close()
file_R.close()
