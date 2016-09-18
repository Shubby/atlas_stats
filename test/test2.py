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

with open('atlas_internal_notes_2015_Dublin_4.xml', 'rt') as f:
    tree = ET.parse(f)
root = tree.getroot()
#for node in tree.iter():
#    print "ciccio ",node.tag,"pip ", node.attrib
#for node in root.iter():
#for node in root.iter():
for node_h in root.findall('{http://purl.org/dc/elements/1.1/}dc'):
    for node in node_h.findall('{http://purl.org/dc/elements/1.1/}creator'):
        auth_text = node.text; 
        if not re.match('.*Collaboration',auth_text):
            if re.match('.*,.*',auth_text):
                auth_tmp = auth_text.split(',')
                new_auth = auth_tmp[1].strip()+" "+auth_tmp[0].strip()
            else:
                new_auth = auth_text
            
            print "new_author=",new_auth
            for note_id in node_h.findall('{http://purl.org/dc/elements/1.1/}identifier'):
                print note_id.tag, note_id.attrib, note_id.text, note_id.tail
                if note_id.text is not None:
                    print "note_id1 ",note_id.text
                    match_note_cath = re.match(r'^ATL-(.*)-2015-.*',note_id.text)
                    if match_note_cath:
                        print "note_id2 ",match_note_cath.group(1)
            
