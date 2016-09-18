#!/usr/bin/python

import fileinput
import sys,os
import string
import re
import ROOT
from array import array
import xml.etree.ElementTree as ET
#from lxml import etree as ET
ns = {'foaf': 'http://xmlns.com/foaf/0.1/',
      'cal': 'http://www.slac.stanford.edu/spires/hepnames/authors_xml/'}
countryMatch = re.compile(',\W(\w*)$')


with open('atlas_authlist.xml', 'rt') as f:
    tree = ET.parse(f)
author_input = "Antonio Sidoti"
author_org = ""
#tree = ET.parse("atlas_authlist.xml")
root = tree.getroot()
#for node in tree.iter():
#    print "ciccio ",node.tag,"pip ", node.attrib

for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Person'):
    author_name =  node.find('foaf:name',ns).text
    if author_input == author_name:
        inst_node = node.find('cal:authorAffiliations',ns).find('cal:authorAffiliation',ns)
        author_org = inst_node.get('organizationid')
        break;
    
for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Organization'):
    my_id = node.get('id')
    if my_id == author_org:
        org_name = node.find('foaf:name',ns).text
        if countryMatch.search(org_name):
            Country=countryMatch.search(org_name).group(1)
            print "Country is:",Country
            break;

    #    for name, value in sorted(node.attrib.items()):
#        print '--->  %-4s = "%s"' % (name, value)

#for pip in root.findall('cal:authors',ns):
#    print "cazzo1",pip
#    for author in pip.findall('foaf:Person',ns):
#        print "cazzo2",author.attrib
#        #print author.text
#    for aa in pip.findall('cal:authorAffiliations',ns):
#        print "cazzo3",aa.attrib

