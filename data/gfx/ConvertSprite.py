import json
from xml.etree import ElementTree

def read_file(xmlfile):
    tree = ElementTree.parse('good.xml')
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
    return json.dumps(sprite_json, sort_keys=True, indent=2)

def animation_to_json(animation):
    animation_json = {}
    animation_json['image'] = image_to_json(animation['image'])
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

def frame_to_json(frame):
    if('empty' in frame and frame['empty'] == 'true'):
        return [-1, frame['dur']]
    else:
        return [frame['x'], frame['dur']]

if __name__ == "__main__":
    root = read_file("good.xml")
    sprite_def = read_sprite(root)
    print("Sprite name:", sprite_def['name'])
    print(sprite_to_json(sprite_def))
