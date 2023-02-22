# write to file and append to exst file body
f = open("demofile.txt", "a")
f.write(" Now the file has more content!")
f.close()


# overwrite the file
f = open("demofile.txt", "w")
f.write("Woops! I have deleted the content!")
f.close()