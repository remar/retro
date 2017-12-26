from xml.etree import ElementTree

def read_file(xmlfile):
    tree = ElementTree.parse('good.xml')
    return tree.getroot()

def handle_animation(animation):
    anim = animation.attrib.copy()
    frames = []
    for child in animation:
        if child.tag == "image":
            anim['image'] = handle_image(child)
        elif child.tag == "frame":
            frames.append(handle_frame(child))
    anim['frames'] = frames
    return anim

def handle_image(image):
    return image.attrib.copy()

def handle_frame(frame):
    return frame.attrib.copy()

if __name__ == "__main__":
    root = read_file("good.xml")
    animations = []
    for animation in root:
        animations.append(handle_animation(animation))
    print(animations[0])

