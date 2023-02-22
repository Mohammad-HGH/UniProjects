"""
  The 'try' block lets you test a block of code for errors.

 The 'except' block lets you handle the error.

 The 'else' block lets you execute code when there is no error.

 The 'finally' block lets you execute code, regardless of the result of the try- and except blocks.

"""


try:
    print(x)
except Exception as e:
    print(str(e))
finally:
    print("The 'try except' is finished")


# Raise an error and stop the program if x is lower than 0
# stop program
x = -1
if x < 0:
    raise Exception("Sorry, no numbers below zero")


# check is not integer
x = "hello"
if not type(x) is int:
    raise TypeError("Only integers are allowed")
