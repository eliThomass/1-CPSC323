def idFSM():
    pass
def intrealFSM():
    pass

keywords = {}

lines = []
comment = False

with open('input1.txt') as f:
    

    # Parse each line, eliminate blank lines and comments
    for line in f:
        curr = ""
        # If line is blank, skip it
        if not line or line == "\n":
            continue
        # Check each character in every line
        for c in line:
            # Skip characters that are comments or are enclosed by comments
            if c == "\"" and not comment:
                comment = True
            elif c == "\"" and comment:
                comment = False
            elif not comment:
                curr += c

        # Do not add current line if it is all comments
        if not curr or curr == "\n":
            continue
        
        lines.append(curr)


    print(lines)

    f.close()