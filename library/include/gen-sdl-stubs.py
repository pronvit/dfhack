import argparse, re, os, sys

parser = argparse.ArgumentParser()
parser.add_argument('--sdl-dir', help='Path to SDL include directory',
    default='libs/SDL.framework/Headers')
parser.add_argument('--sdl-image-dir', help='Path to SDL_image include directory',
    default='libs/SDL_image.framework/Headers')
parser.add_argument('--sdl-ttf-dir', help='Path to SDL_ttf include directory',
    default='libs/SDL_ttf.framework/Headers')
args = parser.parse_args()

print('// Generated - do not edit')

funcs = []
structs = []
enums = []

def uniq(list):
    result = []
    for item in list:
        if item not in result:
            result.append(item)
    return result

def process(lib_id, path):
    headers = filter(lambda h: h.endswith('.h'), os.listdir(path))
    for h in headers:
        with open(os.path.join(path, h)) as f:
            contents = f.read()
        matches = re.findall(r'extern\s+DECLSPEC\s+(.+?)\s+SDLCALL\s+([^\s]+)\s*\(([^)]+)\);', contents, re.MULTILINE)
        funcs.extend(map(lambda match: (lib_id,) + match, matches))
        for m in matches:
            args = m[2].split(',')
            for a in args:
                if '*' in a:
                    type = a.split('*')[0].replace('struct ', '').replace('const ', '').strip()
                    if 'void' not in type and 'char' not in type and 'Uint' not in type \
                            and type not in ('int', 'size_t', 'FILE'):
                        structs.append(type)
                elif a.startswith('SDL_'):
                    structs.append(a.split(' ')[0])
        enums.extend(re.findall(r'(typedef enum.*?\{[^\}]+\} .+?;)', contents, re.MULTILINE))

process('sdl', args.sdl_dir)
process('sdl_image', args.sdl_image_dir)
process('sdl_ttf', args.sdl_ttf_dir)

funcs = uniq(funcs)
structs = uniq(structs)

print('#ifdef SDL_WRAP_DEFINE_STRUCTS')
for e in enums:
    name = e.strip().strip(';').split(' ')[-1]
    if (name in structs):
        structs.remove(name)
for s in structs:
    print('struct %s;' % s)
for e in enums:
    print(e)
print('#endif')
for f in funcs:
    print('SDL_WRAP_FUNC(%s, %s, %s, (%s));' % f)
