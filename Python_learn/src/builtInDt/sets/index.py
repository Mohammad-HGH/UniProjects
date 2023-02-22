# ! Note: Set items are unchangeable, but you can remove items and add new items.
# ! sets are unordered. Set items can appear in a different order every time you use them, and cannot be referred to by index or key.
# ! Once a set is created, you cannot change its items, but you can remove items and add new items.


thisset = {"apple", "banana", "cherry"}
print(thisset)

# block repeated items
thisset = {"apple", "banana", "cherry", "apple"}
print(thisset)  # {'apple', 'cherry', 'banana'}

# get length
thisset = {"apple", "banana", "cherry"}
print(len(thisset))


# get items and checking
thisset = {"apple", "banana", "cherry"}
for x in thisset:
    print(x)

thisset = {"apple", "banana", "cherry"}
print("banana" in thisset)


# add item to sets
thisset = {"apple", "banana", "cherry"}
thisset.add("orange")
print(thisset)


# add sets to another sets
'''To add items from another set into the current set, use the update() method.'''
thisset = {"apple", "banana", "cherry"}
tropical = {"pineapple", "mango", "papaya"}
thisset.update(tropical)  # tropical, thisset
print(thisset)


# update add sets to a list
thisset = {"apple", "banana", "cherry"}
mylist = ["kiwi", "orange"]
thisset.update(mylist) # mylist, thisset
print(thisset)

# The union() method returns a new set with all items from both sets
set1 = {"a", "b" , "c"}
set2 = {1, 2, 3}
set3 = set1.union(set2)
print(set3)


# remove item from sets
thisset = {"apple", "banana", "cherry"}
thisset.remove("banana") # ? If the item to remove does not exist, remove() will raise an error.
print(thisset)

thisset.discard("banana")
print(thisset) # ? If the item to remove does not exist, discard() will NOT raise an error.

# pop()
thisset = {"apple", "banana", "cherry"} 
x = thisset.pop()
print(x)
print(thisset) # ! Sets are unordered, so when using the pop() method, you do not know which item that gets removed.

# clear()
thisset = {"apple", "banana", "cherry"}
thisset.clear()
print(thisset)

# del
thisset = {"apple", "banana", "cherry"}
del thisset

