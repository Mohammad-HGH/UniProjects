# The list is changeable, meaning that we can change, add, and remove items in a list after it has been created.
# we can have duplicate data item in list


thislist = ["apple", "banana", "cherry"]
print(len(thislist))  # get length of list


# NOTE: The search will start at index 2 (included) and end at index 5 (not included).
thislist = ["apple", "banana", "cherry", "orange", "kiwi", "melon", "mango"]
print(thislist[2:5])  # ['cherry', 'orange', 'kiwi']

# Condition true
if "apple" in thislist:
    print("Yes, 'apple' is in the fruits list")


#             0          1         2         3        4       5
thislist = ["apple", "banana", "cherry", "orange", "kiwi", "mango"]
thislist[1:3] = ["blackcurrant", "watermelon"]  # replace 1,2 with new values.
print(thislist)


# insert
'''
    The insert() method inserts an item at the specified index
'''
thislist = ["apple", "banana", "cherry"]
# first input is current station you want to insert second string
thislist.insert(3, "watermelon")
print(thislist)


# append
'''
add item to end of list
'''
thislist = ["apple", "banana", "cherry"]
thislist.append("orange")
print(thislist)


# extend two array to gether
'''
The extend() method does not have to append lists, you can add any iterable object (tuples, sets, dictionaries etc.).
'''
thislist = ["apple", "banana", "cherry"]
tropical = ["mango", "pineapple", "papaya"]
# ['apple', 'banana', 'cherry', 'mango', 'pineapple', 'papaya']
thislist.extend(tropical)
print(thislist)

''' ------------------------- '''

thislist = ["apple", "banana", "cherry"]
thistuple = ("kiwi", "orange")
thislist.extend(thistuple)
print(thislist)



# join lists
list1 = ["a", "b", "c"]
list2 = [1, 2, 3]
list3 = list1 + list2
print(list3)


# remove
thislist = ["apple", "banana", "cherry"]
thislist.remove("banana")
print(thislist)


# pop
thislist = ["apple", "banana", "cherry"]
thislist.pop(0)  # item index u want to pop
print(thislist)


thislist = ["apple", "banana", "cherry"]
thislist.pop()  # pop last item
print(thislist)


# del
''' The del keyword also removes the specified index. '''
thislist = ["apple", "banana", "cherry"]
del thislist[0]
print(thislist)

''' The del keyword can also delete the list completely. '''
thislist = ["apple", "banana", "cherry"]
del thislist


# clear
''' 
The clear() method empties the list.
The list still remains, but it has no content.
'''
thislist = ["apple", "banana", "cherry"]
thislist.clear()
print(thislist)


# loop the list
thislist = ["apple", "banana", "cherry"]

# ! instead above two lines u can wrtie shorthand ('List Comprehension')
# ? [print(x) for x in thislist]
for x in thislist:
    print(x)

thislist = ["apple", "banana", "cherry"]
for i in range(len(thislist)):
    print(thislist[i])


# List Comprehension
fruits = ["apple", "banana", "cherry", "kiwi", "mango"]
# store all items with a character to newlist
newlist = [x for x in fruits if "a" in x]
print(newlist)


# sort the list
thislist = ["orange", "mango", "kiwi", "pineapple", "banana"]
thislist.sort()
print(thislist)

thislist.sort(reverse=True)
print(thislist)


# nearby
''' Sort the list based on how close the number is to 50 '''


def myfunc(n):
    return abs(n - 50)


thislist = [100, 50, 65, 82, 23]
thislist.sort(key=myfunc)
print(thislist)


# case insensitive
thislist = ["banana", "Orange", "Kiwi", "cherry"]
thislist.sort(key=str.lower)
print(thislist)


# ! copy list
'''
! You cannot copy a list simply by typing list2 = list1, because: list2 will only be a reference to list1, and changes made in list1 will automatically also be made in list2.
? Another way to make a copy is to use the built-in method list().
'''
thislist = ["apple", "banana", "cherry"]
mylist = thislist.copy()
print(mylist)

thislist = ["apple", "banana", "cherry"]
mylist = list(thislist)
print(mylist)






