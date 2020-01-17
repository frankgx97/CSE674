#coding:utf8
import random

length = 10 # 边长
threshold = 55 #范围1-100，越大生成墙的几率越高
start = 0
end = length*length - 1 #目标为右下角

matrix = []

def get_up(x,y):
    return True if x == 0 else matrix[x-1][y]['s']

def get_left(x,y):
    return True if y == 0 else matrix[x][y-1]['e']

def get_down(x,y):
    return True if x >= length-1 else rand()

def get_right(x,y):
    return True if y >= length-1 else rand()

def rand():
    r = random.randint(1,100)
    if r < threshold :
        return True
    else:
        return False

def generate():
    for i in range(length):
        matrix.append([{}]*length)
    for i in range(length):
        for j in range(length):
            sum = 0
            w = n = e = s = False
            if get_left(i,j):
                w = True
                sum+=8
            if get_up(i,j):
                n = True
                sum+=4
            if get_right(i,j):
                e = True
                sum+=2
            if get_down(i,j):
                s = True
                sum+=1
            matrix[i][j] = {
                'w':w,
                'n':n,
                'e':e,
                's':s,
                'sum':sum,
            }
    #print(matrix)

def pr():
    print(length*length)
    print(start)
    print(end)
    for i in matrix:
        for j in i:
            print(j['sum'], '', end='',flush=True)

generate()
pr()

