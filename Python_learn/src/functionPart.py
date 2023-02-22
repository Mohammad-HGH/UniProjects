
'''----------------------------------------'''
'''                  Function              '''
'''----------------------------------------'''


def my__function(fname):  # fname param
    print(fname + " Refsnes")


my__function("Emil")  # "Emil" arg
my__function("Tobias")
my__function("Linus")


# send multiple value to function
def MyFunc(*kids):
    print("The youngest child is " + kids[2])


MyFunc("Emil", "Tobias", "Linus")


def my___function(**kid):  # If the number of keyword arguments is unknown
    print("His last name is " + kid["lname"])


my___function(fname="Tobias", lname="Refsnes")


# passing a list
def my_function(food):
    for x in food:
        print(x)


fruits = ["apple", "banana", "cherry"]
my_function(fruits)


# function with no body
def myfunction():
    pass





'''----------------------------------------'''
'''            Lambda  Function            '''
'''----------------------------------------'''
# like arrow func in js
x = lambda a, b, c : a + b + c
print(x(5, 6, 2))
