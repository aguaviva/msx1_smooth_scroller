

#### code

def join_hex(v):
    return ",".join(["0x%02x" % k for k in v])

def get_quarter(data, index):
    x = index % 2
    y = index // 2
    out = []
    for i in data[y*8:(y+1)*8]:
        out.append(i[x*8:(x+1)*8])
    return out

def get_quarter_index(data, definitions):
    d = list(definitions.keys())
    
    for key in definitions.keys():
        ii = d.index(key)*4
        for i in range(4):
            if data==get_quarter(definitions[key],i):
                return ii+i
    return -1

def expand(level, definitions):
    d = list(definitions.keys())
    out = []
    for row in level:
        r1 = []
        r2 = []
        for a in row:
            i = d.index(a)
            r1.append(i*4+0)
            r1.append(i*4+1)
            r2.append(i*4+2)
            r2.append(i*4+3)
        out.append(r1)
        out.append(r2)
    return out

def generate_pairs(level):
    pairs = {}
    i = 0
    for x in range(len(level[0])-1):
        for y in range(len(level)):
            k = (level[y][x], level[y][x+1])
            if k not in pairs:
                pairs[ k ]=i
                i+=1
    return pairs

def pack_indices(oo):
    o = []
    for i in oo:
        o+=i
    o=list(set(o))

    d={}
    for i in range(len(o)):
        d[o[i]] = i
        #d[i] = o[i]

    o = []
    for i in oo:
        io = []
        for j in i:
            io.append(d[j])
        o.append(io)
    return o, d

def find_block(k, definitions):
    di = list(definitions.keys())
    d = get_quarter(definitions[di[k//4]], k%4)

    for l in range(k+1):    
        if d == get_quarter(definitions[di[l//4]], l%4):
            return l
    return -1

def rotate_tiles(definitions, pairs):
    out = {}
    di = list(definitions.keys())

    for a,b in pairs.keys():
        c = []
        qa = get_quarter(definitions[di[a//4]], a%4)
        qb = get_quarter(definitions[di[b//4]], b%4)

        c = [ qa[i]+qb[i] for i in range(8)]

        rot = []
        for i in range(8):            
            rot.append( [int(k[i:i+8],2) for k in c] )
        out[(a,b)] = rot
    return out

def generate_sprite(sp, idx):
    s = [[],[],[],[]]
    for y in range(0,8):
        s[0].append(sp[y][0:8])
        s[2].append(sp[y][8:16])
    for y in range(8,16):
        s[1].append(sp[y][0:8])
        s[3].append(sp[y][8:16])

    o=[]
    for i in s:    
        for j in i:
              o.append(int("".join(["%i" % (1 if b==idx else 0) for b in j]),2))
    return join_hex(o)

def usingned_char(name, data):
    print(f"const unsigned char {name}[]=","{")
    print(data)
    print("};")


def compute_unique_mapping(definitions):
    return [find_block(i, definitions) for i in range(len(definitions)*4)]

#for i in o:
#    print(i)
def remove_repeated_blocks(o, definitions):

    mapping = [find_block(i, definitions) for i in range(len(definitions)*4)]

    oo = []
    for row in o:
        t = [mapping[i] for i in row]
        oo.append(t)

    return oo, mapping

def generate_level(level, transparents, solids):

    definitions = transparents | solids

    level_expanded = expand(level, definitions)

    solid_expanded = []
    for i in definitions.keys():
        solid_expanded += [1 if i in solids else 0]*4

    mapping = compute_unique_mapping(definitions)

    #apply mapping, this reduces the unique indices count, this reduces pair's count
    level_mapped = []
    for row in level_expanded:
        t = [mapping[i] for i in row]
        level_mapped.append(t)

    pairs = generate_pairs(level_mapped)

    print("#define LEVEL_WIDTH", len(level_mapped[0]))
    print("#define LEVEL_HEIGHT", len(level_mapped))

    level_pairs = []
    for s in level_mapped:     
        level_pairs.append([pairs[i] for i in list(zip(s[:-1],s[1:]))])


    print(f"const unsigned char level[LEVEL_HEIGHT][LEVEL_WIDTH] = ")
    print("{")
    for s in level_pairs:
        print("{", ",".join(["%2i" % i for i in s]), "},")
    print("};")    
    print() 

    #print(f"const unsigned char *levelY[LEVEL_HEIGHT] = {{",",".join([f" level[{i}]" for i in range(len(level_mapped))]), "};")

    print("// tiles to upload at each offset")
    for i in range(0,len(level_pairs[0]),8):
        l = []
        for s in level_pairs:
            l+=s[i:i+32]
        l = list(set(l))
        print("//",i, len(l), ":",l)

    solid = []
    for pair in pairs.keys():
        a, b = pair
        solid.append(0 if (solid_expanded[a]==0) or ((solid_expanded[a]==0) and (solid_expanded[b]==0)) else 1)

    usingned_char("solid",join_hex(solid))
    print()

    d = rotate_tiles(definitions, pairs)

    #no need to rotate blank-blank pair
    blank_index = mapping[list(definitions.keys()).index(" ")]
    del d[(blank_index, blank_index)] 

    print("#define PATTERN_COUNT", len(d.keys()))
    #print(f"unsigned char patterns[] = {{")
    for i in range(8):
        print(f"// offset {i}")
        print(f"const unsigned char pattern_{i}[PATTERN_COUNT * 8] = {{")
        for k,v in d.items():
            a,b = k
            s = ",".join(["0x%02x" % k for k in v[i]])
            print(f"{s}, // {a}_{b}")
        print("};") 
    #print("};") 
    
    print("const unsigned char *pPatterns[8] = {", ",".join([f" pattern_{i}" for i in range(8)]), "};\n")


##### Sprites

def rev(a):
    return [i[::-1] for i in a]

def generate_sprites(sprites):    
    data = ""

    data+="// right\n"
    for i in sprites:
        data += generate_sprite(i, "1") + f", // \n"

    data+="// left\n"
    for i in sprites:
        data += generate_sprite(rev(i), "1") + f", // \n"

    usingned_char("mario1", data)



