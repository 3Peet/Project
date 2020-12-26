cipher1 = "3b41020b1908523506050601130f0a45"
cipher2 = "260907174f0401411a0c0a4d06080301"
plain1  = "54686973206973207468652074696d65"     # I love Thailand!
plain2  = "49206c6f766520546861696c616e6421"     # This is the time

c1=[]   # Cyphertext 1
c2=[]   # Cyphertext 2
p1=[]   # Plaintext 1
p2=[]   # Plaintext 2
key=[]  

def splitbit(a,b):
    for i in range(32):
        hex_str = ("0x"+b[i])
        hex_int = int(hex_str, 16) #String to Hex
        a.append(hex_int)

splitbit(c1,cipher1)
splitbit(c2,cipher2)
splitbit(p1,plain1)
splitbit(p2,plain2)

for j in range(32):     # เปรียบเทียบตัวอักษรทุกตัวในตำแหน่งเดียวกัน (32ตัว)
    for i in range(17): # KEY brute force ตั้งแต่ i = 0x0 ถึง 0xf
        if((c1[j]^i == p1[j]) and (c2[j]^i == p2[j])):
            print("KEY is ",hex(i),"   @Position : ",j)
            key.append(hex(i))
            
print(key)


