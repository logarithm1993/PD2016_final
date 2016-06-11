import os, sys, re, argparse

class Rect:
    x0 = int(0)
    y0 = int(0)
    x1 = int(0)
    y1 = int(0)
    def __init__(self, name, x0, y0):
	self.name = name
	self.x0 = int(x0)
	self.y0 = int(y0)
    def set (self, w, h):
	self.x1 = self.x0 + int(w);
	self.y1 = self.y0 + int(h);
    def intersect (self, other):
	if (self.x0 >= other.x1 or self.x1 <= other.x0): return False
	if (self.y0 >= other.y1 or self.y1 <= other.y0): return False
	return True
    def printme (self):
	print "%s: (%s, %s) (%s, %s)" % (self.name, self.x0, self.y0, self.x1, self.y1)
    def write_svg (self, fp, zoom):
	fp.write ('\t\t<rect x="%s" y="%s" width="%s" height="%s" style="fill:%s;stroke:%s;stroke-width:%d"/>\n' 
	    % (self.x0 * zoom, self.y0 * zoom, (self.x1-self.x0) * zoom, (self.y1-self.y0) * zoom, "lightpink", "black", 2))


def read_fp (filename, dict):
    f = open(filename)
    f.seek(0)
    for line in f:
	tokens = re.findall(r'[^\s+]+', line)
	#print "line: ", line
	if (len(tokens) != 6): continue
	#print "token: ", tokens[-1]
	name = tokens[0]
	x = tokens[1]
	y = tokens[2]
	o = Rect(name, x, y)
	#o.printme()
	dict[name] = o;
    f.close()
    return

def read_nodes (filename, dict):
    error = False
    f = open(filename)
    f.seek(0)
    for line in f:
	tokens = re.findall(r'[^\s+]+', line)
	if (len(tokens) != 4): continue
	if (tokens [3] != "terminal"): continue
#	print "line: ", line
	name = tokens[0]
	if (not dict.has_key(name)):
	    print "Node %s not found in fp." % name
	    error = True
	    continue
	w = tokens[1]
	h = tokens[2]
#	print "n=%s w=%s h=%s" % (name, w, h)
	dict[name].set(w, h)
    f.close()
    return error

def get_bounding_box (dict):
    x0 = sys.maxint
    y0 = sys.maxint
    x1 = 0
    y1 = 0

    items = dict.items()
    length = len(dict)
    for i in range(0, length):
	obj = items[i][1]
	if (obj.x0 < x0):
	    x0 = obj.x0
	if (obj.y0 < y0):
	    y0 = obj.y0
	if (obj.x1 > x1):
	    x1 = obj.x1
	if (obj.y1 > y1):
	    y1 = obj.y1

    return (x0, y0, x1, y1)


def write_svg_header (fp, x0, y0, x1, y1):
    w = x1 - x0
    h = y1 - y0
    w1 = round(w/100 + 0.5) * 100
    h1 = round(h/100 + 0.5) * 100
    fp.write('<svg width="%d" height="%d" viewBox="%d %d %d %d" xmlns="http://www.w3.org/2000/svg" version="1.1" xmlns:xlink="http://www.w3.org/1999/xlink">\n'
	% (w1, h1, x0-10, y0-10, x1+10, y1+10))
    fp.write('\t<g transform="translate(0,%d) scale(1,-1)">\n' % h)
    fp.write ('\t\t<rect x="%s" y="%s" width="%s" height="%s" style="fill:%s;stroke:%s;stroke-width:%s"/>\n' 
	% (x0, y0, w, h, "lightgray", "black", 1))

def write_svg_footer (fp):
    fp.write('\t</g>\n');
    fp.write('</svg>\n');

def write_svg (filename, dict, zoom=0.1):
    f = open(filename, 'w')

    box = get_bounding_box(dict)
    x0 = box[0] * zoom
    y0 = box[1] * zoom
    x1 = box[2] * zoom
    y1 = box[3] * zoom

    write_svg_header(f, x0, y0, x1, y1)

    items = dict.items()
    length = len(dict)
    for i in range(0, length):
	obj = items[i][1]
	obj.write_svg(f, zoom)

    write_svg_footer(f)
    f.close()

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Check macro overlap')

    parser.add_argument("-f", "--fp", help=".fp file to be read", dest="fp", required=True)
    parser.add_argument("-n", "--nodes", help=".nodes file to be read", dest="nodes", required=True)

    args = parser.parse_args()

    design_name = os.path.splitext(args.fp)[0]

    objs = {}

    read_fp (args.fp, objs)
    ret = read_nodes (args.nodes, objs)

    write_svg (design_name + ".svg", objs, 0.1)

    if (ret != 0):
	print "Abort due to error"
	exit ()

    items = objs.items()

    len = len(objs)
    for i in range(0, len):
	for j in range(i+1, len):
	    obj1 = items[i][1]
	    obj2 = items[j][1]
	    if (obj1.intersect(obj2)):
		print "==============================================================="
		print "%s intersects with %s:" % (obj1.name, obj2.name)
		obj1.printme()
		obj2.printme()
		print "==============================================================="