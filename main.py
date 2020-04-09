import string

letters = list(string.ascii_lowercase)
letters.extend(list(string.ascii_uppercase))
letters.append('_')

chars = list(letters)
digits = list(string.digits)

chars.extend(digits)


def get_identifiers(substring):
    out = ''
    for i in substring:
        if(i in chars):
            out += i
        else:
            break
    return out, len(out)


def get_ints(substring):
    out = ''
    for i in substring:
        if(i in digits):
            out += i
        else:
            break
    return out, len(out)


def get_tokens(line):
    tokens = []
    strlen = len(line)
    i = 0
    while(i < strlen):
        if(line[i:i+3] == "let"):
            tokens.append('TOK_LET')
            i += 3
        elif(line[i] == '['):
            tokens.append('TOK_SQ_BKT_L')
            i += 1
        elif(line[i] == ']'):
            tokens.append('TOK_SQ_BKT_R')
            i += 1
        elif(line[i] == ':'):
            tokens.append('TOK_COLON')
            i += 1
        elif(line[i] == ';'):
            tokens.append('TOK_SEMICOLON')
            i += 1
        elif(line[i] == ','):
            tokens.append('TOK_COMMA')
            i += 1
        elif(line[i] == '='):
            tokens.append('TOK_EQ')
            i += 1
        elif(line[i] in letters):
            name, total = get_identifiers(line[i:])
            tokens.append('TOK_IDENT'+"(\""+name+"\")")
            i += total
        elif(line[i] in digits):
            name, total = get_ints(line[i:])
            tokens.append('TOK_INT_LIT'+"(\""+name+"\")")
            i += total
        elif(line[i] == ' ' or line[i] == '\n'):
            i += 1
        else:
            tokens.append('TOK_INVALID')
            i += 1

    return tokens


file = open('input.mk')

lines = file.readlines()

for line in lines:
    print(get_tokens(line))
print(['TOK_EOF'])
