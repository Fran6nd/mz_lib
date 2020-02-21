import fputs
# Write to an empty file named `write.txt`
fputs.fputs("Real Python!", "write.txt")
with open("write.txt", "r") as f:
     print(f.read())

