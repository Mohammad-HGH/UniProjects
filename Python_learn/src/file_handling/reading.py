f = open("demofile.txt", "rt")
print(f.read())


# Return the 5 first characters of the file:
print(f.read(5))

# Because "r" for read, and "t" for text are the default values, you do not need to specify them.
# Note: Make sure the file exists, or else you will get an error.
