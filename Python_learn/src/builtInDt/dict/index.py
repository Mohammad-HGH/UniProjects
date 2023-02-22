thisdict = {
    "brand": "Ford",
    "model": "Mustang",
    "year": 1964
}
print(thisdict)
print(thisdict["brand"])


# ! duplicate NOT ALLOWED
thisdict = {
    "brand": "Ford",
    "model": "Mustang",
    "year": 1964,
    "year": 2020
}
print(thisdict)
print(len(thisdict))

thisdict = {
    "brand": "Ford",
    "electric": False,
    "year": 1964,
    "colors": ["red", "white", "blue"]
}


# create dict with constructor
thisdict = dict(name="John", age=36, country="Norway")
print(thisdict)

x = thisdict["model"]  # * Get value of model
x = thisdict.get("model")  # * Get value of model
x = thisdict.keys()
x = thisdict.values()
x = thisdict.items()  # * Get a list of the key:value pairs


# add item to dict
car = {
    "brand": "Ford",
    "model": "Mustang",
    "year": 1964
}
x = car.keys()
print(x)  # before the change
car["color"] = "white"  # add new item
thisdict.pop("model")  # remove item from dict
del thisdict["model"]  # remove item from dict
thisdict.popitem()  # removes the last inserted item
car["year"] = 2020  # update existing item
thisdict.update({"year": 2020})  # update multi existing item
print(x)  # after the change


# check if key is existing in dict
if "model" in thisdict:
    print("Yes, 'model' is one of the keys in the thisdict dictionary")


# delete dict
thisdict = {
    "brand": "Ford",
    "model": "Mustang",
    "year": 1964
}
del thisdict
# print(thisdict) # ! this will cause an error because "thisdict" no longer exists.

thisdict = {
    "brand": "Ford",
    "model": "Mustang",
    "year": 1964
}
thisdict.clear()
print(thisdict)

# loop dict & get key & val
thisdict = {
    "brand": "Ford",
    "model": "Mustang",
    "year": 1964
}
for x, y in thisdict.items():
    print(x, y)


# copy dict
thisdict = {
    "brand": "Ford",
    "model": "Mustang",
    "year": 1964
}
mydict = thisdict.copy()
print(mydict)
'''--------------'''
mydict = dict(thisdict)
print(mydict)
