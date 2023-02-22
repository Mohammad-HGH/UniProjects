# ! Float can also be scientific numbers with an "e" to indicate the power of 10.
# ? Note: You cannot convert complex numbers into another number type.


i = 1
while i < 6:
    print(i)  # 1 2 3
    if i == 3:
        break
    i += 1


i = 0
while i < 6:
    i += 1
    if i == 3:
        continue
    print(i)  # 1 2 4 5 6

for x in range(2, 30, 3):
    print(x)  # loop 2 to 30 with 3 steps

# change global value
x = 300


def myfunc():
    global x
    x = 200


myfunc()

print(x)
