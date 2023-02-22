import os

# Delete file
if os.path.exists("demofile.txt"):
    os.remove("demofile.txt")
else:
    print("The file does not exist")


# Delete folder
os.rmdir("myfolder") # remove folder just it empty
