#!/usr/bin/env python

"""
Copyright(c)2009 Internet Archive. Software license AGPL version 3.

This script converts the output of pdftohtml-ia -xml -hidden -dpi xxx to a
xml file that can be parsed by the djvu tools.

example usage:
xmltodjvuxml.py --id librariesoffutur00lickuoft --dpi 400 -i output_low_preset.xml -o djvu.xml

example usage if you need a custom PYTHONPATH or LD_LIBRARY_PATH:
PYTHONPATH=/petabox/sw/lib/lxml/lib/python2.5/site-packages LD_LIBRARY_PATH=/petabox/sw/lib/lxml/lib xmltodjvuxml.py --id librariesoffutur00lickuoft --dpi 400 -i output_low_preset.xml -o djvu.xml

"""

from lxml import etree
import sys

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i", "--in",  dest="filein")
parser.add_option("-o", "--out", dest="fileout")
parser.add_option("--id")
parser.add_option("--dpi", type="int")
(options, args) = parser.parse_args()

assert None != options.filein
assert None != options.fileout
assert None != options.id
assert None != options.dpi
assert 0 == len(args)


# getText()
#   the "text" element might contain a word as a text value, or might contain html
#   <text>word</text> or <text><b><i>word</i></b></text>
#   in either case, we just want to return "word"
#_______________________________________________________________________________
def getText(el):
    return ''.join(el.itertext())

# getWidthOfSpace()
#   the coordinates from pdftohtml -xml include the width of space characters.
#   we want to remove the spaces and adjust the coordinates, but right now
#   we just hard code a value
#_______________________________________________________________________________
def getWidthOfSpace():
    return 8 #TODO: calculate width based on font and font size

# removeSpaces()
#_______________________________________________________________________________
def removeSpaces(text, l, r):
    text = text.rstrip() #trailing space doesn't seem to effect coords
    
    while(text):
        if ' ' == text[0]:
            text = text[1:]
            l = l + getWidthOfSpace()
        else:
            break
    
    return (text, l, r)

# processPageText()
#   temporarily put all text inside a single paragraph tag, which is passed in
#   as the second arg
#_______________________________________________________________________________
def processPageText(page, root):
    linetop = -1
    for el in page:
        if 'text' == el.tag:
            #djvu.xml WORD elements have a coords attribute with coordinate date
            #in the form "left,bottom,right,top[,baseline]". yes, really.
            #we will leave baseline off for now, since it is optional
            
            #The pdfs we get from Abbyy seem to have very consistant 'top' coords
            #so we can use these to determine if we need to create a new LINE

            t = int(el.get('top'))
            l = int(el.get('left'))
            w = int(el.get('width'))
            h = int(el.get('height'))
            b = t+h
            r = l+w
            
            if t != linetop:
                line = etree.SubElement(root, 'LINE')
                linetop = t
                
            word = etree.SubElement(line, 'WORD')
            
            text = getText(el)
            (text, l, r) = removeSpaces(text, l, r)
            word.text = text
            word.set('coords', "%d,%d,%d,%d"%(l,b,r,t))
            

# main()
#_______________________________________________________________________________

root = etree.Element("DjVuXML")
djvu = etree.SubElement(root, "BODY")

context = etree.iterparse(options.filein,
                        tag='page',
                        resolve_entities=False)

pageNum = 1

for event, page in context:
    pageObj = etree.SubElement(djvu, "OBJECT")
    pageObj.set('width', page.get('width'))
    pageObj.set('height', page.get('height'))
    pageObj.set('data', "file://localhost/tmp/derive/%s/%s.djvu" % (options.id, options.id)) #removed double //
    pageObj.set('type', 'image/x.djvu')
    pageObj.set('usemap', "%s_%04d.djvu" % (options.id, pageNum))
    
    param = etree.SubElement(pageObj, 'PARAM')
    param.set('name', 'PAGE')
    param.set('value', "%s_%04d.djvu"%(options.id, pageNum))

    param = etree.SubElement(pageObj, 'PARAM')
    param.set('name', 'DPI')
    param.set('value', str(options.dpi))

    htext  = etree.SubElement(pageObj, 'HIDDENTEXT')
    col    = etree.SubElement(htext,   'PAGECOLUMN')
    region = etree.SubElement(col,     'REGION')
    para   = etree.SubElement(region,  'PARAGRAPH')
    
    processPageText(page, para)

    map = etree.SubElement(djvu, "MAP")
    map.set('name', "%s_%04d.djvu"%(options.id, pageNum))

    page.clear()
    pageNum+=1

print(etree.tostring(root, pretty_print=True))
