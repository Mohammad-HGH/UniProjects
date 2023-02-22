# tuple items
thistuple = ("apple", "banana", "cherry")
print(thistuple)


# length of tuple
thistuple = ("apple", "banana", "cherry")
print(len(thistuple))


# ! caution! create tuple with one item.
thistuple = ("apple",)
print(type(thistuple))

# NOT a tuple
thistuple = ("apple")
print(type(thistuple))


# access to tuple item
thistuple = ("apple", "banana", "cherry")
print(thistuple[1])


''' range of tuple '''
thistuple = ("apple", "banana", "cherry", "orange", "kiwi", "melon", "mango")
print(thistuple[2:5])

# from first to orange (orange current exist)
thistuple = ("apple", "banana", "cherry", "orange", "kiwi", "melon", "mango")
print(thistuple[:4])


# from cherry to end
thistuple = ("apple", "banana", "cherry", "orange", "kiwi", "melon", "mango")
print(thistuple[2:])


# check if exist in tuple
thistuple = ("apple", "banana", "cherry")
if "apple" in thistuple:
    print("Yes, 'apple' is in the fruits tuple")

# update tuple
x = list(("apple", "banana", "cherry"))
y = x
y[1] = "kiwi"
x = tuple(y)
print(x)

# add item
thistuple = ("apple", "banana", "cherry")
y = list(thistuple)
y.append("orange")
thistuple = tuple(y)


# add tuple to tuple
thistuple = ("apple", "banana", "cherry")
y = ("orange",)
thistuple += y
print(thistuple)


# remove item from tuple
thistuple = ("apple", "banana", "cherry")
y = list(thistuple)
y.remove("apple")
thistuple = tuple(y)


# delete completly
thistuple = ("apple", "banana", "cherry")
del thistuple  # this will raise an error because the tuple no longer exists


# unpack tuple
fruits = ("apple", "banana", "cherry")

(green, yellow, red) = fruits

print(green)
print(yellow)
print(red)


# unpack Asterisk
fruits = ("apple", "banana", "cherry", "strawberry", "raspberry")

(green, yellow, *red) = fruits

print(green)
print(yellow)
print(red, type(red))  # ['cherry', 'strawberry', 'raspberry']


# Using Asterisk* everywhere
fruits = ("apple", "mango", "papaya", "pineapple", "cherry")
(green, *tropic, red) = fruits
print(green)
print(tropic)
print(red)


# loop tuple
thistuple = ("apple", "banana", "cherry")
for x in thistuple:
    print(x)

thistuple = ("apple", "banana", "cherry")
for i in range(len(thistuple)):
    print(thistuple[i])


# join tuples
tuple1 = ("a", "b", "c")
tuple2 = (1, 2, 3)

tuple3 = tuple1 + tuple2
print(tuple3)
''' ---------------------- '''
fruits = ("apple", "banana", "cherry")
mytuple = fruits * 2
print(mytuple)


# count()
thistuple = (1, 3, 7, 8, 7, 5, 4, 6, 8, 5)
x = thistuple.count(5) # count num of repeated item
print(x)


# return index of value
thistuple = (1, 3, 7, 8, 7, 5, 4, 6, 8, 5)
x = thistuple.index(8)
print(x)