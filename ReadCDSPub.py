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
##global scope
# set True this flag for debugging
isVerb=False
Country=""
CountryNAuthors={}
#match "'24/06/16'"
dateMatchSlash = re.compile('(\d\d)/(\d\d)/(\d\d)')
#match "02-May-16"
dateMatchDash = re.compile('(\d\d)-(\w+)-(\d\d)') 
#match '24/06/16'| 02-May-16
#dateMatch = re.compile((dd)/(dd)/(dd)|(dd)-(\w+)-(dd))
countryMatch = re.compile(',\W(\w*)$')
ns = {'foaf': 'http://xmlns.com/foaf/0.1/',
      'cal': 'http://www.slac.stanford.edu/spires/hepnames/authors_xml/'}

def strip_non_ascii(string):
    ''' Returns the string without non ASCII characters'''
    stripped = (c for c in string if 0 < ord(c) < 127)
    return ''.join(stripped)
#key author () value list of countries, one item per year 2016, 2015, etc....
# dictionary of dictonay indexed by year
authorToCountryDictY = {} 
authorToCountryDict = {}

# contains dictionary with
# 0 is All, 1 Italy, 2 France, 3 Germany, 4 United Kingdom, 5 United States, 6 Rest
authorPerCathList = [] 
authorPerCathDict = {}
# Dictionary with note per cathegory
notePerCathDict = {}
notePerCathCountryDict = {}


auth_name=""
def FillAuthToCount(FileName='atlas_authlist.xml'):
    author_org = ""
    tree = ET.parse(FileName)
    root = tree.getroot()
    for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Person'):
        author_name =  node.find('foaf:name',ns).text
        if not author_name in  authorToCountryDict:
            inst_node = node.find('cal:authorAffiliations',ns).find('cal:authorAffiliation',ns)
            author_org = inst_node.get('organizationid')
            for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Organization'):
                my_id = node.get('id')
                if my_id == author_org:
                    org_name = node.find('foaf:name',ns).text
                    org_name_arr = org_name.split(",")
                    Country=org_name_arr[len(org_name_arr)-1].strip()
                    authorToCountryDict[author_name]=Country
    pass
################################# USED FOR note cathegory:Country

class CathCountry:
    def __init__(self,cath,country):
        self.cath = cath
        self.country = country


    def __hash__(self):
        return hash((self.cath, self.country))

    def __eq__(self, other):
        return (self.cath, self.country) == (other.cath, other.country)

    def __ne__(self, other):
        # Not strictly necessary, but to avoid having both x==y and x!=y
        # True at the same time
        return not(self == other)

class YearCountryCath:
    def __init__(self,year,country,cath):
        self.year = year
        self.country = country
        self.cath = cath

    def __hash__(self):
        return hash((self.year, self.country, self.cath))

    def __eq__(self, other):
        return (self.year, self.country, self.cath) == (other.year, other.country, self.cath)

    def __ne__(self, other):
        # Not strictly necessary, but to avoid having both x==y and x!=y
        # True at the same time
        return not(self == other)
    
################################# MAIN #####################


# Read author lists per year annd store them in a List of dictionaries
# list is authorToCountryDictY and and dictionary is authorToCountryDict
# TODO create a dictionary with (Author,Year) as a Key
#for yea in ['2010','2011','2012','2013','2014','2015','2016']:
for yea in ['2015']:
    FileListName="data/atlas_authlist_"+yea+".xml"
    print "Reading and parsing author list for year: ", yea, "  File: ", FileListName
    FillAuthToCount(FileListName)
    authorToCountryDictY[yea] = authorToCountryDict.copy()
    authorToCountryDict={}
    pass

print "DUMP Author to Country Map for year: ", yea
print authorToCountryDictY[yea]





###########



filename = sys.argv[1]
outputfile_name = sys.argv[2]


print "Reading CDS file (xml dublin format): ",filename
print "Dumping excel readable file in: ",outputfile_name

## now open and parse xml file from CDS
ns_cds = {'dc': 'http://purl.org/dc/elements/1.1/',
          'invenio': 'http://invenio-software.org/elements/1.0',
          'xsi':'http://www.w3.org/2001/XMLSchema-instance'}

tree = ET.parse(filename)
root = tree.getroot()
## loop on entries
for node_h in root.findall('{http://purl.org/dc/elements/1.1/}dc'):
    # loop on note
    isNote=False
    note_text=""
    # this is the note id
    for note_id in node_h.findall('{http://purl.org/dc/elements/1.1/}identifier'):
        note_text=note_id.text
        if note_id.text is not None:
            if isVerb:
                print "Note Id is: ", note_text
            match_note_cath = re.match(r'^ATL-(.*)-2\d\d\d-.*',note_id.text)
            if match_note_cath:
                note_cath = match_note_cath.group(1)
                if notePerCathDict.has_key("All"):
                    notePerCathDict["All"]+=1
                else:
                    notePerCathDict["All"]=1
                if notePerCathDict.has_key(note_cath):
                    notePerCathDict[note_cath]+=1
                else:
                    notePerCathDict[note_cath]=1
                if isVerb:
                    print "--> Note cath: ",note_cath
                isNote=True
                # this is inizialization for each note
                isAlreadyFilled={}
                isYearCountryFilled={}
                ## loop on all authors
                for node in node_h.findall('{http://purl.org/dc/elements/1.1/}creator'):
                    auth_text = node.text; 
                    if re.match('.*Collaboration',auth_text):
                        break
                    if re.match('.*,.*',auth_text):
                        auth_tmp = auth_text.split(',')
                        new_auth = auth_tmp[1].strip()+" "+auth_tmp[0].strip()
                    else:
                        new_auth = auth_text
                    if isVerb:
                        print "----> author is: ",new_auth
 
                    # finally fill counters if country
                    # has not been found for that note  yet
                    my_country=authorToCountryDictY["2015"].get(new_auth, None)
                    if my_country is None:
                        print "Warning! Can't find country for author: ",new_auth, " Skipping author for publication : ", note_id.text
#                        # use fuzzywuzzy to found best match and store it in a dict
#                        if isVerb:
#                            print "lookin for fuzzy match for author: ", new_auth                       

                    else:
                        if isVerb:
                            print "----> from: ",my_country
                        my_cath_count = CathCountry(note_cath, my_country)
                        my_cath_count_All = CathCountry(note_cath, "All")
                        my_year_count = YearCountryCath("2015",my_country,note_cath)
                        my_year_count_All = YearCountryCath("2015",my_country,"All")
                        # numero di note con almeno un autore di una nazione
                        if not isYearCountryFilled.has_key(my_year_count_All):
                            if notePerCathCountryDict.has_key(my_year_count_All):
                                notePerCathCountryDict[my_year_count_All]+=1
                            else:
                                notePerCathCountryDict[my_year_count_All]=1
                            isYearCountryFilled[my_year_count_All]=True
                     
                        if not isYearCountryFilled.has_key(my_year_count):
                            if notePerCathCountryDict.has_key(my_year_count):
                                notePerCathCountryDict[my_year_count]+=1
                            else:
                                 notePerCathCountryDict[my_year_count]=1
                            isYearCountryFilled[my_year_count]=True
                        # nota gia fillata
                        if not isAlreadyFilled.has_key("All"):
                            if authorPerCathDict.has_key(my_cath_count_All):
                                authorPerCathDict[my_cath_count_All]=authorPerCathDict[my_cath_count_All]+1
                            else:
                                authorPerCathDict[my_cath_count_All]=1
                                isAlreadyFilled["All"]=True
                        if not isAlreadyFilled.has_key(my_country):
                            if authorPerCathDict.has_key(my_cath_count):
                                authorPerCathDict[my_cath_count]=authorPerCathDict[my_cath_count]+1
                            else:
                                authorPerCathDict[my_cath_count]=1
                                isAlreadyFilled[my_country]=True

#DUMP
#Pretty print
# before loop on All to find all cathegories in values

list_cath=[]
list_country=["All","Italy","France","Germany","United Kingdom","United States of America"]

for k, v in authorPerCathDict.items():
    if k.country == "All":
        if not k.cath in list_cath:
            list_cath.append(k.cath)

if isVerb:
    for k, v in authorPerCathDict.items():
        print k.cath,"  ", k.country, "  ",v


print "Number of papers per cathegory per country"
for k, v in notePerCathCountryDict.items():
    print k.cath,"  ", k.country, "  ",v

sep="|"


# now print in a useful excel format
outfile  = open(outputfile_name,"w")

string_to_print=""
for icath in list_cath:
    string_to_print+=str(sep)+str(icath)
outfile.write(string_to_print+"\n")

string_to_print="Notes"+str(sep)
for icath in list_cath:
    string_to_print+=str(sep)+str(notePerCathDict[icath])
outfile.write(string_to_print+"\n")
    


for ict in list_country:
    string_to_print=str(ict)
    for icath in list_cath:
        isThere=False
        for k, v in authorPerCathDict.items():
            if (k.country == ict) and  (k.cath == icath):
                string_to_print += str(sep)+str(v)
                isThere=True
                break
        if not isThere:
            string_to_print += str(sep)+str(0)
    outfile.write(string_to_print+"\n")

outfile.write("Number of notes with at least one outer of each country"+"\n")
for ict in list_country:
    string_to_print=str(ict)
    for icath in list_cath:
        isThere=False
        for k, v in notePerCathCountryDict.items():
            if (k.country == ict) and  (k.cath == icath) and (k.year=="2015"):
                string_to_print += str(sep)+str(v)
                isThere=True
                break
        if not isThere:
            string_to_print += str(sep)+str(0)
    outfile.write(string_to_print+"\n")
outfile.close()
    


                            
