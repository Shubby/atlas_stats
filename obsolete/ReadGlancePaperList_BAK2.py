#!/usr/bin/python

import fileinput,io
import re,sys,os
import string
import ROOT
from array import array
import xml.etree.ElementTree as ET
reload(sys)  
sys.setdefaultencoding('utf8')
from collections import Counter

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
EditorTypeList=["Editors","Contact Editors","Editorial Board","Editorial Board Chair"]
YearList = ["2015"]
CountryList=["All","Italy","France","Germany","United Kingdom","United States of America"]
################################# USED FOR note Year:Country
class YeaCountry:
    def __init__(self,yea,country):
        self.yea = yea
        self.country = country


    def __hash__(self):
        return hash((self.yea, self.yea))

    def __eq__(self, other):
        return (self.yea, self.country) == (other.yea, other.country)

    def __ne__(self, other):
        # Not strictly necessary, but to avoid having both x==y and x!=y
        # True at the same time
        return not(self == other)
######################################################3

################################# USED FOR note Year:Country:EditorTyp
class YeaCountryEdtyp:
    def __init__(self,yea,country,edtyp):
        self.yea = yea
        self.country = country
        self.edtyp = edtyp


    def __hash__(self):
        return hash((self.yea, self.yea, self.edtyp))

    def __eq__(self, other):
        return (self.yea, self.country, self.edtyp) == (other.yea, other.country, other.edtyp)

    def __ne__(self, other):
        # Not strictly necessary, but to avoid having both x==y and x!=y
        # True at the same time
        return not(self == other)
######################################################3

def strip_non_ascii(string):
    ''' Returns the string without non ASCII characters'''
    stripped = (c for c in string if 0 < ord(c) < 127)
    return ''.join(stripped)
#key author () value list of countries, one item per year 2016, 2015, etc....
# dictionary of dictonay indexed by year
authorToCountryDictY = {} 
authorToCountryDict = {}
editor_per_country_Y = {}

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


################################# MAIN #####################

for yea in ['2015']:
#    FileListName="data/atlas_authlist_"+yea+".xml"
    FileListName="data/atlas_authlist_test.xml"
    print "Reading and parsing author list for year: ", yea, "  File: ", FileListName
    FillAuthToCount(FileListName)
    authorToCountryDictY[yea] = authorToCountryDict.copy()
    authorToCountryDict={}
    pass

print "DUMP Author to Country Map for year: ", yea
print authorToCountryDictY[yea]

total_number_authors = len(authorToCountryDictY["2015"])
aut_cth = Counter(authorToCountryDictY["2015"].values())
print "auth_cth",aut_cth["Italy"]
total_author_per_country = {}
for ict in CountryList:
    if not total_author_per_country.has_key(ict):
        total_author_per_country[ict]=aut_cth[ict]

    


###########



glance_filename = sys.argv[1]
outputfile_name = sys.argv[2]


print "Reading papers file note from Glance: ",glance_filename
print "Dumping excel readable file in: ",outputfile_name


## now open and parse xml file from glance

tree = ET.parse(glance_filename)
root = tree.getroot()
## loop on entries
for node_h in root.findall('row'):
    isOK = False
    my_sep="|"
    string_to_print = ""
    short_title=""
    for i in  node_h.findall(".//column[@description='Short Title']"):
        #print "i.text:",i.text," i.attrib",i.attrib
        short_title=i.text
    #year
    my_year=""
    for i in  node_h.findall(".//column[@description='Publication Date']"):
        #print "i.text:",i.text," i.attrib",i.attrib
        if i.text is None:
            print "Warning! Cannot find publication date for", short_title, " Skipping publication"
            break;
        print "Date of publication is:", i.text
        date_string=i.text
        my_year=date_string.split("/")[0].strip()
        print "my_year=",my_year
        if my_year != "2015":
            print "breaking loop"
            break;

        
        print "I am in 2015!"        
        string_to_print+=short_title+my_sep
        #Dump long fields
        if i.attrib["description"]=="Full Title":
            if i.text is not None:
                string_to_print+=i.text+my_sep
        if i.attrib["description"]=="Journal of final submission":
            if i.text is not None:
                string_to_print+=i.text+my_sep
        if  i.attrib["description"]=="All links":
            if i.text is not None:
                #print "CAZZO", i.text, i.attrib,i.tail,i.tag
                list_of_links = i.text.split(",")
                doi_expr=""
                for il in list_of_links:
                    my_match=re.match('.*DOI.*(http.*)\@final_journalPublicationURL.*',il)
                    if my_match:
                        doi_expr=my_match.group(1)
                        doi_expr = re.sub('http%3A//', "http://", doi_expr)
                        string_to_print+=my_sep+doi_expr
                        break
        if i.attrib["description"]=="arXiv Reference":
            if i.text is not None:
                string_to_print+=i.text+my_sep
                print "VVV",string_to_print

        isAlreadyFilled = {}    
        # editor
        for i_edtyp in EditorTypeList:
            if i.attrib["description"]==i_edtyp:
                editor_list=i.text.split(",")
                #            print "---> Editor List:", editor_list
                for ed_id in editor_list:
                    my_country=authorToCountryDictY["2015"].get(ed_id, None)
                    if  my_country is None:
                        print "Warning! Can't find country for author: ",ed_id, " Skipping author for publication : ", short_title
                    else:
                        my_yea_All = YeaCountryEdtyp("2015", "All",i_edtyp)
                        #                        if not isAlreadyFilled.has_key(my_yea_All):
                        #                            isAlreadyFilled[my_yea_All] = True
                        if editor_per_country_Y.has_key(my_yea_All):
                            editor_per_country_Y[my_yea_All]+=1
                        else:
                            editor_per_country_Y[my_yea_All]=1
                        
                        my_yea_count = YeaCountryEdtyp("2015", my_country,i_edtyp)
                        #if not isAlreadyFilled.has_key(my_yea_count):
                        #    isAlreadyFilled[my_yea_count] = True
                        if editor_per_country_Y.has_key(my_yea_count):
                            editor_per_country_Y[my_yea_count]+=1
                        else:
                            editor_per_country_Y[my_yea_count]=1
    if isOK:
        print "-->",string_to_print
print "editor_per_country_Y:"
for k, v in editor_per_country_Y.items():
    print "Year:", k.yea,"  country:", k.country, "  edtype:", k.edtyp, "  ", v


sep="|"


# now print in a useful excel format
outfile  = open(outputfile_name,"w")


for i_yea in YearList:
    outfile.write(i_yea+"|||||"+str(total_number_authors)+"\n")
    
    string_to_print=""
    for i_edtyp in EditorTypeList:
        string_to_print+=str(sep)+str(i_edtyp)
    outfile.write(string_to_print+"|\n")

    for ict in CountryList:
        string_to_print=str(ict)
        for i_edtyp in EditorTypeList:
            isThere=False
            for k, v in editor_per_country_Y.items():
                if (k.country == ict) and  (k.edtyp == i_edtyp) and (k.yea==i_yea):
                    string_to_print += str(sep)+str(v)
                    isThere=True
                    break
            if not isThere:
                string_to_print += str(sep)+str(0)
        outfile.write(string_to_print+sep+str(total_author_per_country[ict])+"\n")

outfile.close()
