total = 0
out = 0
with open("input.txt") as f:
    for line in f:
        if line == '' or line == '\n':
            out = max(out, total)
            total = 0
        else:
            total += int(line)
print(out)