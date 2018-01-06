import json, sys
from xml.etree import ElementTree

def read_file(xmlfile):
    tree = ElementTree.parse(xmlfile)
    return tree.getroot()

def read_sprite(sprite):
    spr = {}
    animations = []
    for animation in sprite:
        animations.append(read_animation(animation))
    spr['name'] = sprite.attrib['name']
    spr['animations'] = animations
    return spr

def read_animation(animation):
    anim = animation.attrib.copy()
    frames = []
    for child in animation:
        if child.tag == "image":
            anim['image'] = read_image(child)
        elif child.tag == "frame":
            frames.append(read_frame(child))
    anim['frames'] = frames
    return anim

def read_image(image):
    return image.attrib.copy()

def read_frame(frame):
    return frame.attrib.copy()

def sprite_to_json(sprite):
    sprite_json = {"animations":{}}
    for animation in sprite['animations']:
        sprite_json['animations'][animation['name']] = animation_to_json(animation)
    return collapse_lists(json.dumps(sprite_json, indent=2))

def collapse_lists(json_sprite):
    level = 0
    in_list = False
    output = []
    for c in json_sprite:
        if c == '[':
            level += 1
            if level == 1:
                in_list = True
        elif c == ']':
            level -= 1
            if level == 0:
                in_list = False
        if c in '\n ' and in_list:
            pass
        elif c == ',' and in_list:
            output.append(c)
            output.append(' ')
        else:
            output.append(c)

    return "".join(output)

def animation_to_json(animation):
    animation_json = {}
    animation_json['image'] = image_to_json(animation['image'])
    animation_json['origin'] = offset_to_origin_json(animation['image'])
    animation_json['looping'] = (animation['loop'] == "true")
    animation_json['frames'] = []
    for frame in animation['frames']:
        animation_json['frames'].append(frame_to_json(frame))
    return animation_json

def image_to_json(image):
    image_json = {}
    image_json['path'] = image['path']
    image_json['width'] = int(image['size_x'])
    image_json['height'] = int(image['size_y'])
    return image_json

def offset_to_origin_json(image):
    return [int(image.get('orig_x', '0')), int(image.get('orig_y', '0'))]

def frame_to_json(frame):
    if('empty' in frame and frame['empty'] == 'true'):
        return [-1, int(frame['dur'])]
    else:
        return [int(frame['x']), int(frame['dur'])]

def write_sprite(sprite_def):
    sprite_json = sprite_to_json(sprite_def)
    with open(sprite_def['name'] + ".json", "w") as f:
        f.write(sprite_json)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: " + sys.argv[0] + " <sprite_def.xml>")
    else:
        root = read_file(sys.argv[1])
        sprite_def = read_sprite(root)
        write_sprite(sprite_def)
