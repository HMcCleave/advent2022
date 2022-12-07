total = 0
out = []
with open("input.txt") as f:
    for line in f:
        if line == '' or line == '\n':
            out.append(total)
            out = sorted(out, reverse=True)[:3]
            total = 0
        else:
            total += int(line)
print(sum(out))