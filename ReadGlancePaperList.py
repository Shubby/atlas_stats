#!/usr/bin/python

import fileinput,io
import re,sys,os
import string
import ROOT
from ROOT import TH1F
from array import array
import xml.etree.ElementTree as ET
#reload(sys)  
#sys.setdefaultencoding('utf8')
import unicodedata
import argparse
### for fuzzy matching
from fuzzywuzzy import fuzz
from fuzzywuzzy import process
#import datetime


parser = argparse.ArgumentParser(
    description='Produce ntuple with ATLAS papers statistics.')

parser.add_argument('-i', metavar='in-file',
                    type=str, help='input file \
                    (.xml file issued by Glance)',
                    default = "data/Glance_analysis_papers.xml")
parser.add_argument('-o', metavar='out-file',
                    type=str, help='output file \
                    (ROOT file)', default ="data/atlaspaperstest.root")

parser.add_argument('-y', nargs='?', type=str,
                    help='delimited lower and upper bound for years \
                    (e.g. 2013-2016) --> years 2013, 2014, 2015 and 2016 \
                    (e.g. 2015-2015) --> year 2015 only \
                    (e.g. 2013,2015) --> years 2013 and 2015 \
                    (e.g. 2016 )     --> year 2016 only',
                    dest='years_input', default="2009-2016")

parser.add_argument('-l', action='store_true',
                    help='counts statistics from 1st of September')


parser.add_argument('-d',
                    help="directory where the author config files are",
                    dest='authorlist_dir', type=str, default="data")

parser.add_argument('-auth',
                    help="author list files (the year will be \
                    added before the .xml suffix)",
                    dest='authorlist_file',
                    type = str, default="atlas_authlist.xml")

parser.add_argument('-v',
                    help="verbosity level",
                    dest='verbosity',
                    type = int, default=0)


results = parser.parse_args()
input_filename = str(results.i)
output_filename = str(results.o)
verb = int(results.verbosity)
# build list of year
year_list=[]
commaMatch = re.compile(',')
dashMatch =  re.match( r'(\d*)-(\d*)', results.years_input, re.M|re.I)

if dashMatch:
    lo_year = int(dashMatch.group(1))
    hi_year = int(dashMatch.group(2))
    if lo_year>hi_year:
        temp_year = lo_year
        lo_year = hi_year
        hi_year = temp_year
    year_list = range(lo_year, hi_year+1)
elif commaMatch.search(results.years_input):
    year_list = [int(item) for item in results.years_input.split(',')]
    year_list.sort()
else:
    year_list.append(int(results.years_input))

print "Extracting stat for year: ",year_list
#
count_stat_sept = results.l
if count_stat_sept:
    print "Counting statistics from 1st September"
else :
    print "Counting statistics from 1st January"
    pass
# getting name of author list files
author_list_dict ={}
auth_list_def = results.authorlist_dir+"/"+results.authorlist_file
print "Default author list=", auth_list_def
for it in year_list:
    xmlMatch =  re.match( r'(.*)\.xml',auth_list_def, re.M|re.I)
    if xmlMatch:
        author_list_dict[it] = xmlMatch.group(1)+"_"+str(it)+".xml"

print "lists of author lists:",author_list_dict 
print "Input file is:", input_filename
print "Output file name is:", output_filename
#

##global scope
CountryNAuthors={}

CountryDef = ['All','Italy','France', 'United Kingdom', 'Germany',
              'United States of America']

#match "'24/06/16'"
dateMatchSlash = re.compile('(\d\d)/(\d\d)/(\d\d)')
#match "02-May-16"
dateMatchDash = re.compile('(\d\d)-(\w+)-(\d\d)')

countryMatch = re.compile(',\W(\w*)$')
ns = {'foaf': 'http://xmlns.com/foaf/0.1/',
      'cal': 'http://www.slac.stanford.edu/spires/hepnames/authors_xml/'}

# dictionary of retry per aythor {name, int}
retry_per_author = {}
max_retry = 3

# dictionaries that contain { year : {Country: int}}
EditCountryY = {}
CoedCountryY = {}
EdboCountryY = {}
ChaiCountryY = {}
AlleCountryY = {}
###################################################################
def UpdateDict(my_dict,year,country):
    if my_dict.has_key(year):
        my_dict2 = my_dict[year]
        if my_dict2.has_key(country):
            my_dict2[country]=my_dict2[country]+1
        else:
            my_dict2[country]=1
            pass
        my_dict[year] = my_dict2
    else:
        my_dict2 = {}
        my_dict2[country]=1
        my_dict[year] = my_dict2
        pass
    pass
###################################################################
def AnalyzeDict(my_dict,year, string_to_print):
    num_country = {}
    for ic in CountryDef:
        num_country[ic]=0
        pass

    if not my_dict.has_key(year):
        print "Cannot find year ", year , " in :", my_dict
        print "Skipping ...."
    else:
        for k, v in my_dict[year].items():
            num_country['All'] += v
            for ic in num_country:
                if k == ic:
                    num_country[ic]+=v
                    pass
                pass
            pass
        pass
    print "Numbers of :", string_to_print, " for year:", year
    for ic in num_country:
        if num_country['All']>0:
            print ic, "  Num:", num_country[ic], "  Frac:",  float(num_country[ic])/float(num_country['All'])
        else:
            print ic, "  Num:", num_country[ic], "  Frac:", 0
        pass
    pass
###################################################################
def PrintTable(year):
    num_country_edit = {}
    num_country_coed = {}
    num_country_edbo = {}
    num_country_chai = {}
    num_country_alle = {}
    for ic in CountryDef:
        num_country_edit[ic]=0
        num_country_coed[ic]=0
        num_country_edbo[ic]=0
        num_country_chai[ic]=0
        num_country_alle[ic]=0
        pass

    if EditCountryY.has_key(year):
        for k, v in EditCountryY[year].items():
            num_country_edit['All'] += v
            for ic in num_country_edit:
                if k == ic:
                    num_country_edit[ic]+=v
                    pass
                pass
            pass
        pass

    if CoedCountryY.has_key(year):
        for k, v in CoedCountryY[year].items():
            num_country_coed['All'] += v
            for ic in num_country_coed:
                if k == ic:
                    num_country_coed[ic]+=v
                    pass
                pass
            pass
        pass

    if EdboCountryY.has_key(year):
        for k, v in EdboCountryY[year].items():
            num_country_edbo['All'] += v
            for ic in num_country_edbo:
                if k == ic:
                    num_country_edbo[ic]+=v
                    pass
                pass
            pass
        pass

    if ChaiCountryY.has_key(year):
        for k, v in ChaiCountryY[year].items():
            num_country_chai['All'] += v
            for ic in num_country_chai:
                if k == ic:
                    num_country_chai[ic]+=v
                    pass
                pass
            pass
        pass
    if AlleCountryY.has_key(year):
        for k, v in AlleCountryY[year].items():
            num_country_alle['All'] += v
            for ic in num_country_alle:
                if k == ic:
                    num_country_alle[ic]+=v
                    pass
                pass
            pass
        pass
    
    
    print  " Stats for year:", year
    for ic in CountryDef:
        print ic, " | ", num_country_edit[ic], " | ", num_country_coed[ic], " | ",  num_country_edbo[ic], " | ", num_country_chai[ic], " | ",  num_country_alle[ic]
        pass
    

###################################################################
def strip_non_ascii(string):
    ''' Returns the string without non ASCII characters'''
    stripped = (c for c in string if 0 < ord(c) < 127)
    return ''.join(stripped)
#################################################################
### looking for organization for author
# global dictionary that stores the author from glance to the most
# probable in the author_list
author_glance2auth_list={}
def FindAuthorOrg(author,year,FileName='atlas_authlist.xml'):
    if verb>=2:
        print "in  FindAuthorOrg(",author.encode('utf-8'),",",year,",",FileName,")"
        pass
    author=re.sub('\s{2}'," ",author)
    author=re.sub('\-'," ",author)
    author_org = "-1"
    tree = ET.parse(FileName)
    root = tree.getroot()
    author_list =[]
    for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Person'):
        author_name =  node.find('foaf:name',ns).text
        author_list.append(author_name)
        author2 = author
        if author_glance2auth_list.has_key(author): # is fuzzy
            author2 = author_glance2auth_list[author]
        if author2 == author_name:
            inst_node = node.find('cal:authorAffiliations',ns).find('cal:authorAffiliation',ns)
            author_org = inst_node.get('organizationid')
            if verb>2:
                print "Author:",author.encode('utf-8'), "belongs to",author_org
                pass
            isOrganization=True
            return author_org
            break
        pass
    if author_org =="-1":
        print "Warning! Can't find author", author.encode('utf-8')," in authorlist file:", FileName
        pass
    # try fuzzywazzy
    # print "looking using fuzzywazzy in",author_list
    if verb >1:
        print "looking using fuzzywazzy"
        pass
    fuzzy_author = process.extractOne(author, author_list) # find best choice
    if fuzz.partial_ratio(author,fuzzy_author)>75:
        author_glance2auth_list[author]=fuzzy_author[0]
        print "Author:", author.encode('utf-8')  ," found with fuzzywazzy is:", fuzzy_author[0]
        author_org = FindAuthorOrg(fuzzy_author[0],year,FileName)
        print "Org for author is:", author_org
        return author_org
    else:
        if verb >1:
            print "Checking in other author lists",author.encode('utf-8')
            pass
        if retry_per_author.has_key(author):
            if verb >1:
                print "Author:", author.encode('utf-8')," has already been checked ",retry_per_author[author],"times"
                pass
            if retry_per_author[author]>=max_retry:
                print "Warning! cannot find author:", author.encode('utf-8'), " in author lists. Giving Up!"
                return author_org
            else:
                retry_per_author[author] = retry_per_author[author]+1
                pass
        else:
            retry_per_author[author]=1
            pass
        if verb >2:
            print "Looking for author", author.encode('utf-8')," in other author lists"
            pass
        ### use other author_lists
        # get index of year in year_list
        year_id = year_list.index(year)
        if verb >3:
            print "year id is:", year_id
            print "year_list length:",len(year_list), " year list:", year_list
            pass
        new_id = year_id
        # try next year
        new_id=year_id+1
        if verb >2:
            print "-----> author_org:",author_org, " new_id:", new_id
            pass
        if new_id<len(year_list):
            new_file_name = str(FileName)
            if verb >2:
                print "Trying year+1:", year_list[new_id]," old filename:",new_file_name
                pass
            new_file_name = new_file_name.replace(str(year),str(year_list[new_id]))
            if verb >2:
                print "Trying filename+1",new_file_name
                pass
            # check if file exist
            if os.path.exists(new_file_name):
                author_org = FindAuthorOrg(author,year_list[new_id],new_file_name)
                if author_org != "-1":
                    retry_per_author[author]=0
                    return author_org
                    pass
                pass
            pass
        # try previous year
        new_id=year_id-1
        if year_id>0:
            new_file_name = str(FileName)
            if verb >2:
                print "Trying year-1:", year_list[new_id]," old filename:",new_file_name
                pass
            new_file_name = new_file_name.replace(str(year),str(year_list[new_id]))
            if verb >2:
                print "Trying filename-1",new_file_name
                pass
            #            # check if file exist
            if os.path.exists(new_file_name):
                author_org = FindAuthorOrg(author,year_list[new_id],new_file_name)
                if author_org != "-1":
                    retry_per_author[author]=0
                    return author_org
                pass
            pass
        pass
    return author_org
#################################################################
def FindOrgCountry(org,FileName='atlas_authlist.xml'):
    Country=""
    isCountry=False
    tree = ET.parse(FileName)
    root = tree.getroot()
    for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Organization'):
        my_id = node.get('id')
        if my_id == org:
            org_name = node.find('foaf:name',ns).text
            org_name_arr = org_name.split(",")
            Country=org_name_arr[len(org_name_arr)-1].strip()
            if Country is None:
                print "Warning cannot find country for organization:", org_name
            else:
                isCountry = True
                return Country
            pass
        pass
    if not isCountry:
        print "Warning! cannot find matching organization for org id:", org
    pass

###################################################################                
# Count the total number of authors for each country and store it
# in CountryNAuthors
def FindCountryAuthors(year,FileName='atlas_authlist.xml'):
    if verb>2:
        print "----------------------->"
        print "in FindCountryAuthors(",FileName,")"
        pass
    #global CountryNAuthors
    #CountryNAuthors={}
    author_org = ""
    my_country = ""
    tree = ET.parse(FileName)
    root = tree.getroot()
    for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Person'):
        author_name =  node.find('foaf:name',ns).text
        # this is very time consuming
        #        my_country = GetAuthorCountry(author_name,year,FileName)
        # try this one instead
        inst_node = node.find('cal:authorAffiliations',ns).find('cal:authorAffiliation',ns)
        author_org = inst_node.get('organizationid')
        for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Organization'):
            my_id = node.get('id')
            if my_id == author_org:
                org_name = node.find('foaf:name',ns).text
                #               if type(org_name)== unicode:
                #                  unicodedata.normalize('NFKD', org_name).encode('asc#ii','ignore')
                org_name_arr = org_name.split(",")
                if verb>2:
                    print "org_name_arr:",  org_name_arr, " length=",len(org_name_arr)
                    pass
                my_country=org_name_arr[len(org_name_arr)-1].strip()
                if verb>2:
                    print "author2: ",author_name, "belongs to:", my_country
                    pass
                break;
            pass
        
        if CountryNAuthorsY.has_key(year):
            CountryNAuthors = CountryNAuthorsY[year]
            if CountryNAuthors.has_key(my_country):
                CountryNAuthors[my_country]+=1
            else:
                CountryNAuthors[my_country] =1
                pass
            CountryNAuthorsY.update({year:CountryNAuthors})
        else:
            CountryNAuthors = {}
            CountryNAuthors[my_country] =1
            CountryNAuthorsY.update({year:CountryNAuthors})
        pass
    pass
    
#        inst_node = node.find('cal:authorAffiliations',ns).find('cal:authorAffiliation',ns)
#        author_org = inst_node.get('organizationid')
#        for node in tree.findall('.//{http://xmlns.com/foaf/0.1/}Organization'):
#            my_id = node.get('id')
#            if my_id == author_org:
#                org_name = node.find('foaf:name',ns).text
#                #                if type(org_name)== unicode:
#                #                    unicodedata.normalize('NFKD', org_name).encode('asc#ii','ignore')
#                org_name_arr = org_name.split(",")
#                if verb>2:
#                    print "org_name_arr:",  org_name_arr, " length=",len(org_name_arr)
#                    pass
#                my_country=org_name_arr[len(org_name_arr)-1].strip()
#                if verb>2:
#                    print "author2: ",author_name, "belongs to:", my_country
#                    pass
#                if CountryNAuthors.has_key(my_country):
#                    CountryNAuthors[my_country] += 1
#                else:
#                    CountryNAuthors[my_country] =1
#                    pass
#                
#                break;
#    pass

###################################################################
# dictionary that stores {author : {Year: Country)}}
AuthorCountryY={}
def GetAuthorCountry(author,year,FileName='atlas_authlist.xml'):
    my_country="NONE"
    
    #print "in GetAuthorCountry(",author,",",year,",",FileName,")"
    if AuthorCountryY.has_key(author):
        if verb>2:
            print " AuthorCountryY dict has author:", author.encode('utf-8')
            pass
        AuthorCountry=AuthorCountryY[author]
        if AuthorCountry.has_key(year):
            if verb>2:
                print "Getting country for author:", author.encode('utf-8')," in :", year ," from dictionary"
                pass
            return AuthorCountry[year]
        else:
            if verb>2:
                print "Getting country for author:", author," in :", year ," from xml"
                pass
            AuthorCountryCopy = AuthorCountry.copy()
            my_org = FindAuthorOrg(author,year,FileName)
            my_country = FindOrgCountry(my_org,FileName)
            AuthorCountryCopy[year]=my_country
            if verb>2:
                print "---->", AuthorCountryCopy[year]
                print "---->", AuthorCountryY[author]
                pass
            AuthorCountryY[author].update({year:my_country})
        return my_country
    else:
        if verb>2:
#            if type(author)== unicode:
#                unicodedata.normalize('NFD', author)
                #unicodedata.normalize('NFKD', author).encode('ascii','ignore')
                #unicodedata.normalize('NFC', author).encode('ascii','ignore')
                #unicodedata.normalize('NFD', author).encode('ascii','ignore')
                #unicodedata.normalize('NFKC', author).encode('ascii','ignore')
#                print "type:", type(author)
#                print "after conversion", author
#                pass
            print " AuthorCountryY dict has no author:", author.encode('utf-8')
        pass
        my_org = FindAuthorOrg(author,year,FileName)
        my_country = FindOrgCountry(my_org,FileName)
        AuthorCountry = {year:my_country}
        AuthorCountryY[author] = AuthorCountry
        return my_country
    return my_country
########## MAIN ##############################   



##########ROOT output file
outfile = ROOT.TFile(output_filename,"recreate")
outtree = ROOT.TTree("PUB",outfile.GetName())

EditCountry =  ROOT.std.vector( ROOT.std.string )()
outtree._EditCountry = EditCountry
CoedCountry =  ROOT.std.vector( ROOT.std.string )()
outtree._CoedCountry = CoedCountry
EdboCountry =  ROOT.std.vector( ROOT.std.string )()
outtree._EdboCountry = EdboCountry
ChaiCountry =  ROOT.std.vector( ROOT.std.string )()
outtree._ChaiCountry = ChaiCountry
AlleCountry =  ROOT.std.vector( ROOT.std.string )()
outtree._AlleCountry = AlleCountry
Title = ROOT.std.vector( ROOT.std.string )()
outtree._Title = Title
Status = ROOT.std.vector( ROOT.std.string )()
outtree._Status = Status
Group = ROOT.std.vector( ROOT.std.string )()
outtree._Group = Group
#Year = 2009
Year = array('i',[0])
#outtree._Year = Year
#PublRun = 0
PublRun = array('i',[0])
#outtree._PublRun = PublRun

NEditors = array('i',[0])
NEBoard = array('i',[0])
maxedit=400
NCountry = array('i',maxedit*[0])
NBCountry = array('i',maxedit*[0])

outtree.Branch('EditCountry',EditCountry)
outtree.Branch('CoedCountry',CoedCountry)
outtree.Branch('EdboCountry',EdboCountry)
outtree.Branch('ChaiCountry',ChaiCountry)
outtree.Branch('AlleCountry',AlleCountry)
#legacy for root file
outtree.Branch('NEditors',NEditors,'NEditors/I')
outtree.Branch('NEBoard',NEBoard,'NEBoard/I')
outtree.Branch('NCountry', NCountry, 'NCountry[NEditors]/I' )
outtree.Branch('NBCountry', NBCountry, 'NBCountry[NEBoard]/I' )

outtree.Branch('Title', Title )
outtree.Branch('Group', Group )
outtree.Branch('Status', Status )

outtree.Branch('Year', Year, 'Year/I' )
outtree.Branch('PublRun',  PublRun, 'PublRun/I')

# define histograms




##########
# dictionary that stores { year : (dictionary Country: number of authors)} 
CountryNAuthorsY={}

for yea in year_list:
    FileListName=author_list_dict[yea]
    FindCountryAuthors(yea,FileListName)
    print "--------------------------------------------"
    print "Year: ", yea, " ", CountryNAuthorsY[yea]
    total_auth = 0
    CountryNAuthors = CountryNAuthorsY[yea]
    for k,v in CountryNAuthors.items():
        total_auth += v
        pass
    print "Total authors is:",total_auth
    pass


###########
#Now reads glance.xml using  ElementTree
# and fill ntuple

title=""
year=""
status=""
physgroup=""
publrun=""
editors=[]
coeditors=[]
editboard=[]
edbchair=[]
alleditors=[]
publ_run=0
paper=-1

# build list of histograms
RunNum = ['All','Run1', 'Run2']
PhysGroup = ['All','BPHY','STDM','TOPQ', 'HIGG', 'EXOT', 'SUSY', 'HION', 'DAPR', 'IDET', 'LARG', 'PERF', 'MUON', 'JETM', 'TAUP', 'IDTR', 'FTAG', 'SOFT', 'EGAM', 'TCAL', 'TRIG']
hist_year_dict = {}
for irun in RunNum:
    for iphy in PhysGroup:
        for ictr in CountryDef:
            key_val = irun+"_"+iphy+"_"+ictr
            name = "histo"+key_val
            title = "Year for Run:"+irun+" Group:"+iphy+" Country:"+ictr
            hist = TH1F(name, title, 8, 2009, 2017)
            hist_year_dict[key_val] = hist
            pass
        pass
    pass


print "parsing .xml file"
tree_gl = ET.parse(input_filename)
root_gl = tree_gl.getroot()
for node in tree_gl.findall('row'):
    for st in node.findall('column'):
        if st.attrib['description']=='Short Title':
            title = str(st.text)
            break
        
    for st in node.findall('column'):
        if st.attrib['description']=='Journal Submission':
            # year of submission
            year = int((st.text).split("/")[0])
            if count_stat_sept:
                month = int((st.text).split("/")[1])
                day = int((st.text).split("/")[2])
                if month>=9:
                    year = year+1
                    pass
                pass
            pass
            #print "year of subission",year
            break

    for st in node.findall('column'):
        if st.attrib['description']=='Global Status':
            status = str(st.text)
            break
    for st in node.findall('column'):
        if st.attrib['description']=='Lead Group':
            physgroup = str(st.text)
            break
    for st in node.findall('column'):
        if st.attrib['description']=='Editors':
            editors = st.text.split(',')
            break
    for st in node.findall('column'):
        if st.attrib['description']=='Contact Editors':
            coeditors = st.text.split(',')
            break
    for st in node.findall('column'):
        if st.attrib['description']=='Editorial Board':
            editboard = st.text.split(',')
            break
    for st in node.findall('column'):
        if st.attrib['description']=='Editorial Board Chair':
            edbchair = st.text.split(',')
            break
    for st in node.findall('column'):
        if st.attrib['description']=='All Editors':
            alleditors =  st.text.split(',')
            break
    for st in node.findall('column'):
        if st.attrib['description']=='Publication Run':
            publ_run = int(st.text)
            break
    # fill histogram
    # get key
    histo_key =  "All_All_All"
    hist_year_dict[histo_key].Fill(year)
    histo_key = "All_"+physgroup+"_All"
    histo_key2 = ""
    hist_year_dict[histo_key].Fill(year)
    if publ_run==2:
        histo_key = "Run2_"+physgroup+"_All"
        histo_key2 = "Run2_All_All"
    else:
        histo_key = "Run1_"+physgroup+"_All"
        histo_key2 = "Run1_All_All"
        pass
    hist_year_dict[histo_key].Fill(year)
    hist_year_dict[histo_key2].Fill(year)
    # dump
    if verb>1:
        print "------------------------------------"
        print "Title:",title, "Year:", year, " stat:",status, " Lead Group:", physgroup, " Run:", publ_run
        print " Editors:", editors
        print " Contact Editors:", coeditors
        print " Editorial Board:", editboard
        print " Chair:", edbchair
        print " All Editors",alleditors
        pass
    # check that year is the one required
    has_year=False
    if not year in year_list:
        continue
    #        continue
    #    InitializeBranches
    EditCountry.clear()
    CoedCountry.clear()
    EdboCountry.clear()
    ChaiCountry.clear()
    AlleCountry.clear()
    Title.clear()
    Status.clear()
    Group.clear()
    #   Year[0] = 2009
    #   PublRun[0] = 0
    Title.push_back(title)
    Status.push_back(status)
    Group.push_back(physgroup)
    PublRun[0] = publ_run
    Year[0] = year
    NEditors[0]=len(editors)
    NEBoard[0]=len(editboard)
    # now look for editor countries
    ned=0
    for auth in editors:
        edit_country=GetAuthorCountry(auth,year,author_list_dict[int(year)])
        # insert country of editors in root-uple and dictionary
        EditCountry.push_back(str(edit_country))
        UpdateDict(EditCountryY, year, edit_country)
        if edit_country:
            NCountry[ned]=CountryNAuthorsY[year][edit_country]
        else:
            NCountry[ned]=999999
            pass
        ned+=1
        pass
    # look for ContactEditors
    for auth in coeditors:
        coed_country=GetAuthorCountry(auth,year,author_list_dict[int(year)])
        # insert country of editors in root-uple and dictionary
        CoedCountry.push_back(str(coed_country))
        UpdateDict(CoedCountryY, year, coed_country)
        pass
    # look for Edboard
    nedb=0
    for auth in editboard:
        edbo_country=GetAuthorCountry(auth,year,author_list_dict[int(year)])
        # insert country of editors in root-uple and dictionary
        EdboCountry.push_back(str(edbo_country))
        UpdateDict(EdboCountryY, year, edbo_country)
        if edbo_country:
            NBCountry[nedb]=CountryNAuthorsY[year][edbo_country]
        else:
            NBCountry[ned]=999999
            pass
        nedb+=1
        pass
    # look for Chair
    for auth in edbchair:
        chai_country=GetAuthorCountry(auth,year,author_list_dict[int(year)])
        # insert country of editors in root-uple and dictionary
        ChaiCountry.push_back(str(chai_country))
        UpdateDict(ChaiCountryY, year, chai_country)
        pass
    # look for all Editors
    for auth in alleditors:
        alle_country=GetAuthorCountry(auth,year,author_list_dict[int(year)])
        # insert country of editors in root-uple and dictionary
        AlleCountry.push_back(str(alle_country))
        UpdateDict(AlleCountryY, year, alle_country)
        pass
    
    outtree.Fill()
    pass
for k in hist_year_dict:
    hist_year_dict[k].Write()
    pass

print "end parsing .xml file"
print "############################################################"
print "Analyzing Editors"
print "Country of Editors", EditCountryY
for yea in year_list:
    AnalyzeDict(EditCountryY, yea, "Editors")

print "Country of Contact Editors", CoedCountryY
for yea in year_list:
    AnalyzeDict(CoedCountryY, yea, "Contact Editors")

print "Country of members of Editorial Board", EdboCountryY
for yea in year_list:
    AnalyzeDict(EdboCountryY, yea, "Members of Editorial Board")

print "Country of chair of Editorial Board", ChaiCountryY
for yea in year_list:
    AnalyzeDict(ChaiCountryY, yea, "Chairs of Editorial Board")


print "Country of All Editors", AlleCountryY
for yea in year_list:
    AnalyzeDict(AlleCountryY, yea, "All Editors")


print "---------------- Print Table ----------------"
for yea in year_list:
    PrintTable(yea)
    pass




outfile.Write()
outfile.Close()
