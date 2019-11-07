import sys

s1 = bytearray(10)   

s1[0] = 65 #A
s1[1] = 66 #B
s1[2] = 67 #C
s1[3] = 68 #D
s1[4] = 69 #E
s1[5] = 70 #F
s1[6] = 71 #G
s1[7] = 72 #H
s1[8] = 73 #I

t = buffer(s1)       

sys.stdout.write(t)
value = [0,0,0,0,0,0,0,0]
for i in range(8):
    value[i] = ord(sys.stdin.read(1))
    print ("value i -> " + str(value[i]))

sys.stdout.write('q')