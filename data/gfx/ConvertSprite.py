from xml.etree import ElementTree

def read_file(xmlfile):
    tree = ElementTree.parse('good.xml')
    return tree.getroot()

def handle_animation(animation):
    print(animation.attrib)
    anim = animation.attrib.copy()
    for child in animation:
        if child.tag == "image":
            print("Handle image:", child)
        elif child.tag == "frame":
            print("Handle frame:", child)
    return anim

if __name__ == "__main__":
    root = read_file("good.xml")
    animations = []
    for animation in root:
        animations.append(handle_animation(animation))
    print(animations)

