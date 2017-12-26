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

if __name__ == "__main__":
    root = read_file("good.xml")
    sprite_def = read_sprite(root)
    print("Sprite name:", sprite_def['name'])
    for animation in sprite_def['animations']:
        print("  Animation name:", animation['name'])
