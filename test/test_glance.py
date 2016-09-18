#!/usr/bin/python

import fileinput
import sys,os
import string
import re
import ROOT
from array import array
import xml.etree.ElementTree as ET
reload(sys)  
sys.setdefaultencoding('utf8')

#from lxml import etree as ET
ns = {'foaf': 'http://xmlns.com/foaf/0.1/',
      'cal': 'http://www.slac.stanford.edu/spires/hepnames/authors_xml/'}
countryMatch = re.compile(',\W(\w*)$')
ns_cds = {'dc': 'http://purl.org/dc/elements/1.1/',
          'invenio': 'http://invenio-software.org/elements/1.0',
          'xsi':'http://www.w3.org/2001/XMLSchema-instance'}

with open('data/Glance_-_analysis--summary_-_2016-07-07_23-18.xml', 'rt') as f:
    tree = ET.parse(f)
root = tree.getroot()

for note_id in root.findall('row'):
    #    print note_id.tag, note_id.attrib, note_id.text, note_id.tail
    #if note_id.tag=='{http://purl.org/dc/elements/1.1/}identifier':
    #                    print "note_id0 ",note_id.text
    #my_ent = note_id.findall('column description=""Editors"" name=""EDITORS""')
    #    my_ent = note_id.findall('column')
    for i in  note_id.findall('column'):
        print i.tag, i.attrib, i.text, i.tail
        if i.attrib["description"]=="Editors":
            print "---->",i.text
#    editors_field = note_id.get("description=\"Editors\"")
#    if editors_field is not None:
#        print editors_field.tag, editors_field.attrib, editors_field.text, editors_field.tail
        
    
    
