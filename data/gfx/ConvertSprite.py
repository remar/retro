from xml.etree import ElementTree

def read_file(xmlfile):
    """Read a XML sprite definition

    :xmlfile: TODO
    :returns: TODO

    """
    tree = ElementTree.parse('good.xml')
    root = tree.getroot()

    if root.tag != "sprite":
        raise TypeError("Not a sprite definition")

    return root

def handle_animation(animation):
    """Handle one animation

    :animation: TODO
    :returns: TODO

    """

    print(animation.attrib)
    for child in animation:
        if child.tag == "image":
            print("Handle image:", child)
        elif child.tag == "frame":
            print("Handle frame:", child)

if __name__ == "__main__":
    root = read_file("good.xml")
    for animation in root:
        handle_animation(animation)

