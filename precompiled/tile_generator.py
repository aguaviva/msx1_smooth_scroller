from PIL import Image
import numpy as np

im_frame = Image.open('smb.png')
np_frame_org = np.array(im_frame)

np_frame = np_frame_org[3*16:7*16, 0*16:6*16,1]

def get_block(np_frame, x,y):
    b = np_frame[y*16:(y+1)*16, x*16:(x+1)*16]
    o=[]
    for y in range(16):
        s = ""
        for x in range(16):
            if b[y,x]>0:
                s+="0"
            else:
                s+="1"
        o.append(s)
    return o

#o= get_block(np_frame, 2,0)
#for i in o:
#    print(i)

space = [    
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
]    

mountain_left = [    
	"0000000000000001",
	"0000000000000010",
	"0000000000000100",
	"0000000000001000",
	"0000000000010000",
	"0000000000100000",
	"0000000001000000",
	"0000000010000000",
	"0000000100000000",
	"0000001000000000",
	"0000010000000000",
	"0000100000000000",
	"0001000000000000",
	"0010000000000000",
	"0100000000000000",
	"1000000000000000",
]    

mountain_right = [    
	"1000000000000000",
	"0100000000000000",
	"0010000000000000",
	"0001000000000000",
	"0000100000000000",
	"0000010000000000",
	"0000001000000000",
	"0000000100000000",
	"0000000010000000",
	"0000000001000000",
	"0000000000100000",
	"0000000000010000",
	"0000000000001000",
	"0000000000000100",
	"0000000000000010",
	"0000000000000001",
]    

mountain_top = [    
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000011111100000",
	"0011100000011100",
	"1100000000000011",
]    

mountain_dots = [    
	"0000000000000110",
	"0000000000001111",
	"0000000000001111",
	"0000000000001111",
	"0000000001001111",
	"0000000011100110",
	"0000000011100000",
	"0000000001000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
	"0000000000000000",
]  
pole = [    
	"0000000110000000", 
	"0000000110000000",  
	"0000000110000000", 
	"0000000110000000",  
	"0000000110000000", 
	"0000000110000000",  
	"0000000110000000", 
	"0000000110000000",  
	"0000000110000000", 
	"0000000110000000",  
	"0000000110000000", 
	"0000000110000000",  
	"0000000110000000", 
	"0000000110000000",  
	"0000000110000000", 
	"0000000110000000",  
]    

mario_idle= [
    "0011111000000000",
	"0111111111000000",    
	"0111001100000000",    
	"1010001011000000",    
	"1011000100010000",    
	"1100001111100000",    
	"0010000001000000",    
	"0011111110000000",    
	"0111111111100000",    
	"1111011011110000",    
	"1011111111010000",    
	"1101111110110000",    
	"1111111111110000",    
	"0011100111000000",    
	"0101000010100000",    
	"1111000011110000",    
]

mario1=[
    "0000011111000000",
    "0000111111111000",
    "0000111001100000",
    "0001010001011000",
    "0001011000100100",
    "0001100001111000",
    "0000010000010000",
    "0011111111100110",
    "0111111111111101",
    "1010111010111110",
    "1100111111100100",
    "0001111111111100",
    "0011111111101100",
    "0101110001111100",
    "0111000000000000",
    "0011100000000000",
]

mario2=[
    "0000000000000000",
    "0000011111000000",
    "0000111111111000",
    "0000111001100000",
    "0001010001011000",
    "0001011000100100",
    "0001100001111000",
    "0000010000010000",
    "0001111111100000",
    "0010111111011000",
    "0011110111001000",
    "0001111010110000",
    "0010111111100000",
    "1111110111000000",
    "1100001010000000",
    "0000001111000000",
]

mario3=[
    "0000011111000000",
    "0000111111111000",
    "0000111001100000",
    "0001010001011000",
    "0001011000100100",
    "0001100001111000",
    "0000010000010000",
    "0001111111100000",
    "0011111111100000",
    "0011110110111000",
    "0011111011111100",
    "0001100001110100",
    "0000100011011000",
    "0000111111110000",
    "0000111101111000",
    "0000111110000000",
]

mario_jump=[
    "0000000000000110",
    "0000001111100101",
    "0000011111111111",
    "0000011100110011",
    "0000101000101111",
    "0000101100010011",
    "0000110000111110",
    "0000001000001000",
    "0011111111111000",
    "0111011111101001",
    "1011101111111001",
    "1110111101110111",
    "0001011111111011",
    "0011111111111111",
    "0111000111000000",
    "0100111000000000",
]


pipe1 = get_block(np_frame, 4, 0)     
pipe2 = get_block(np_frame, 5, 0)     
pipe3 = get_block(np_frame, 4, 1)     
pipe4 = get_block(np_frame, 5, 1)     

nube1 = get_block(np_frame, 0, 2) 
nube2 = get_block(np_frame, 1, 2) 
nube3 = get_block(np_frame, 2, 2) 

block = get_block(np_frame, 0, 1) 
brick = get_block(np_frame, 1, 0) 
stones = get_block(np_frame, 0, 0)
q_mark = get_block(np_frame, 2,0)
coin = get_block(np_frame, 3,1)  

solids = {
    "X": block,
    "#": stones,
    "=": brick,
    "?": q_mark,
    "q": pipe1,
    "w": pipe2,
    "a": pipe3,
    "s": pipe4,
}

transparents = {
    " ": space,
    "e": nube1,
    "r": nube2,
    "t": nube3,
    "|": pole,
    "C": coin,
    "[": mountain_left,
    "]": mountain_right,
    "^": mountain_top,
    "o": mountain_dots,
}

definitions = transparents | solids

level = [
    "                         ?                                                                   ###########   ###?             ?                                 ",
    "#                                                                                                                                                             ",
    "#                                                                                                                                          X      |      =    ",
    "#                   ?  #?#?#                          qw             qw                   ###                 #     ##   ?  ?  ?          XX      |     ===   ",
    "#    ^                                       qw       as   ^         as                                           ^                      XXX      |    =====  ",
    "#   [o]              ^              qw       as       as  [o]        as         ^                                [o]             ^      XXXX      |    == ==  ",
    "#  [o  ]      errrt [o]     ert     as       as  ert  as [o  ]       as  errt  [o]     ert                      [o  ]     errt  [o]    XXXXX      X    == ==  ",                                  
    "####################################################################################  #################   ####################################################"
]


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
    print(f"unsigned char {name}[]=","{")
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

di = list(definitions.keys())
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


print(f"unsigned char level[LEVEL_HEIGHT][LEVEL_WIDTH] = ")
print("{")
for s in level_pairs:
    print("{", ",".join(["%2i" % i for i in s]), "},")
print("};")    
print() 

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

print("#define PATTERN_COUNT", len(d.keys()))

print("unsigned char patterns[] ={")
for i in range(8):
    print(f"// offset {i}")
    for k,v in d.items():
        a,b = k
        s = ",".join(["0x%02x" % k for k in v[i]])
        print(f"{s}, // {a}_{b}")
print("};") 

def rev(a):
    return [i[::-1] for i in a]
    
data = ""
for i in [mario1, mario2, mario3, mario_jump, mario_idle]:
    data += generate_sprite(i, "1") + ",\n"
for i in [mario1, mario2, mario3, mario_jump, mario_idle]:
    data += generate_sprite(rev(i), "1") + ",\n"

usingned_char("mario1", data)



