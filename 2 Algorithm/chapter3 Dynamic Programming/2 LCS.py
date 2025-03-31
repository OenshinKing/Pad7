from random import choice
from string import digits,ascii_letters
from time import perf_counter
import numpy as np

def LCS(s1,s2):
    start_time = perf_counter()
    m, n = len(s1), len(s2)

    table = [[0]*(n+1) for _ in range(m+1)]
    longest = [[""]*(n+1) for _ in range(m+1)]

    for i in range(1, m+1):
        for j in range(1, n+1):

            if s1[i-1] == s2[j-1]:
                table[i][j] = table[i-1][j-1] + 1
                longest[i][j] = longest[i-1][j-1] + s1[i-1]
            else:
                if table[i-1][j] > table[i][j-1]:
                    table[i][j] = table[i-1][j]
                    longest[i][j] = longest[i-1][j]
                else:
                    table[i][j] = table[i][j-1]
                    longest[i][j] = longest[i][j-1]
    end_time = perf_counter()
    elapsed_time = end_time - start_time
    return elapsed_time,table[m][n],longest[m][n]


chars = ascii_letters+digits
size = 32

file = open("D:\\1华工\\1课程\\大二下\\Pad7\\2 Algorithm\\chapter3 Dynamic Programming\\1IO\\2 time.csv",'w')
file.write("M,N,Time\n")
for i in range(1,size+1):
    for j in range(1,size+1):
        print(i,j)
        for k in range(1,20):
            time_list = list()

            a, b = "", ""
            for k in range(0,i*16+1):
                a += choice(chars)
            for l in range(0,j*16+1):
                b += choice(chars)

            time_list.append(LCS(a,b)[0])
        time_mean = np.array(time_list).mean()
        file.write(','.join([str(i*16),str(j*16),f"{time_mean}"]) + '\n')

file.close()