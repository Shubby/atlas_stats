#!/usr/bin/python

import fileinput,io
import re,sys,os
import string

##global scope
CountryNAuthors={}
OrgDict={}
OrgNameDict={}
PersDict={}
PersOrgDict={}

def FindCountryAuthors(FileName='atlas_authlist.xml'):
    OrgList=[]
    global OrgDict
    global OrgNameDict
    global PersDict
    global PersOrgDict
    Org=""
    orgflag=False
    fin=open(FileName)
    for line in fin:
        m=re.search(r'<foaf:Organization id="(.*)">',line)
        m2=re.search(r'<foaf:name>(.*)</foaf:name>',line)
        m3=re.search(r'.*\,(.*)</foaf:name>',line)
        if m:
            Org=m.group(1)            
            OrgList.append(Org)
            orgflag=True
            pass
        if m2 and orgflag:
            orgflag=False
            #print m2.group(1)
            cou=m2.group(1)
            listaOrg=cou.split(",")
            country=listaOrg[-1]                
            country=country.rstrip()
            country=country.lstrip()
            OrgDict.update({Org:country})
            OrgNameDict.update({Org:cou})
            Org=""
            pass
        if m3 and orgflag:
            orgflag=False
            #print m3.group(1)
            cou=m3.group(1)
            listaOrg=cou.split(",")
            country=listaOrg[-1]
            country=country.rstrip()
            country=country.lstrip()
            OrgDict.update({Org:country})
            OrgNameDict.update({Org:cou})
            Org=""
            pass
        pass    
    fin.close()
    #print OrgDict
    Author=False
    isPerson=False
    Organization=""
    author=""
    fin=open(FileName)
    for line in fin:
        if Author:
            m=re.search(r'<cal:authorAffiliation organizationid="(\w+)" connection=""/>',line)
            m2=re.search(r'<cal:authorAffiliation connection="" organizationid="(\w+)"/>',line)
            if m:
                #print m.group(1)
                Organization=m.group(1)
                country=OrgDict[Organization]
                PersDict.update({author:country})
                PersOrgDict.update({author:Organization})
                Author=False
                author=""
                pass
            if m2:
                Organization=m2.group(1)
                country=OrgDict[Organization]
                PersDict.update({author:country})
                PersOrgDict.update({author:Organization})
                Author=False
                author=""
                pass                        
            pass
        if line.find("<foaf:Person>")!=-1:
            isPerson=True
            pass
        m3=re.search(r'<foaf:name>(.*)</foaf:name>',line)
        if m3 and isPerson:
            #print "found author",line
            author=m3.group(1)
            Author=True
            isPerson=False
            pass
        pass
    
    fin.close()
    
    #print PersDict
    global CountryNAuthors
    CountryNAuthors={}
    for authors in PersDict.keys():
        country=PersDict[authors]
        if CountryNAuthors.has_key(country):
            CountryNAuthors[country] += 1
        else:
            CountryNAuthors[country] =1
            pass
        pass
    pass

########## MAIN ##############################   

MyCountry="Italy"

CountryAuthorList={}
ForeignAuthorList={}
FullCountryList={}
FullName=False# True per fullname organizzazione, False per solo Nazione

ListaAnni= ['2010','2011','2012','2013','2014']
for yea in ListaAnni:
    CountryNAuthors={}
    OrgDict={}
    OrgNameDict={}
    PersDict={}
    PersOrgDict={}
    FileListName="atlas_authlist_"+yea+".xml"
    FindCountryAuthors(FileListName)
    PersOrgList={}
    ForeignOrgList={}
    for author in PersDict:
        if PersDict[author]==MyCountry:
            if FullName: organizzazione=OrgNameDict[PersOrgDict[author]]
            else: organizzazione=PersDict[author]
            #print author," ",organizzazione
            PersOrgList.update({author:organizzazione})
            FullCountryList.update({author:MyCountry})
        else:            
            if FullName: organizzazione=OrgNameDict[PersOrgDict[author]]
            else: organizzazione=PersDict[author]
            #print author," ",organizzazione
            ForeignOrgList.update({author:organizzazione})
            pass
        pass
    CountryAuthorList.update({yea:PersOrgList})
    ForeignAuthorList.update({yea:ForeignOrgList})    
    pass


##for yea in CountryAuthorList.keys():
##    #print "ANNO ",yea
##    #print CountryAuthorList[yea]
##    for author in CountryAuthorList[yea].keys():
##        #print "Anno ",yea," Nome ",author," ",CountryAuthorList[yea][author]
##        for yea2 in ForeignAuthorList.keys():
##            if ForeignAuthorList[yea2].has_key(author):
##                print "Anno ",yea," Nome ",author," ",CountryAuthorList[yea][author]
##                print "Nel ",yea2," presso ", ForeignAuthorList[yea2][author]
##                pass
##            pass
##        for yea3 in CountryAuthorList.keys():
##            if CountryAuthorList[yea3].has_key(author):
##                print "Anno ",yea," Nome ",author," ",CountryAuthorList[yea][author]
##                print "Nel ",yea3," presso ", CountryAuthorList[yea2][author]
##                pass
##            pass        
##        pass    
##    pass
##


AuthorYearOrg={}

for author in FullCountryList:
    AuthorOrg={}
    for yea in CountryAuthorList.keys():
        if CountryAuthorList[yea].has_key(author):
            AuthorOrg.update({yea:CountryAuthorList[yea][author]})
            pass
        pass
    for yea in ForeignAuthorList.keys():
        if ForeignAuthorList[yea].has_key(author):
            AuthorOrg.update({yea:ForeignAuthorList[yea][author]})
            pass
        pass
    AuthorYearOrg.update({author:AuthorOrg})
    pass

for author in AuthorYearOrg.keys():
    for year in AuthorYearOrg[author].keys():
        print "Nome ",author, " nel ",year, " presso ",AuthorYearOrg[author][year]
        
        pass
    pass


AuthorCode={}
for author in AuthorYearOrg.keys():
    code=""    
    for year in  ListaAnni:
        if AuthorYearOrg[author].has_key(year):
            if AuthorYearOrg[author][year]==MyCountry:
                code=code+"IT"
            else:
                code=code+"ES"
                pass
            pass
        else:
            if code!="" : code=code+"NO"
            pass
        pass
    print author," ",code
    AuthorCode.update({author:code})
    pass

NITIT=0
NITES=0
NESIT=0
NITNO=0
NITESIT=0
NESITES=0
NITESNO=0


for author in AuthorCode:
    type=""
    code=AuthorCode[author]
    #print code
    if code.find("ES")!=-1 and code[:2]!="ES" and code[-2:]!="ES":
        type=code[:2]+"ES"+code[-2:]
    elif code.find("IT")!=-1 and code[:2]!="IT" and code[-2:]!="IT":
        type=code[:2]+"IT"+code[-2:]
    else:
        type=code[:2]+code[-2:]
        pass
    #print type
    if type=="ITIT":NITIT=NITIT+1
    if type=="ITNO":NITNO=NITNO+1
    if type=="ITES":NITES=NITES+1
    if type=="ESIT":NESIT=NESIT+1
    if type=="ITESIT":NITESIT=NITESIT+1
    if type=="ESITES":NESITES=NESITES+1
    if type=="ITESNO":NITESNO=NITESNO+1
    pass

NTOT=NITIT+NITNO+NITES+NESIT+NITESIT+NESITES+NITESNO
print "************************************************"
print "*************** ",NTOT," *************************"
print "************************************************"
print "fraction(IT->IT)=",NITIT/float(NTOT)
print "fraction(IT->ES)=",NITES/float(NTOT)
print "fraction(IT->NONE)=",NITNO/float(NTOT)
print "fraction(ES->IT)=",NESIT/float(NTOT)
print "fraction(IT->ES->IT)=",NITESIT/float(NTOT)
print "fraction(IT->ES->NONE)=",NITESNO/float(NTOT)
print "fraction(ES->IT->ES)=",NESITES/float(NTOT)
print "************************************************"
print "************************************************"
