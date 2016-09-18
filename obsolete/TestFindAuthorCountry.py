#!/usr/bin/python

import fileinput,io
import re,sys,os
import string
##global scope
Country=""



def FindAuthorCountry(author,FileName='atlas_authlist.xml'):
    global Country
    Country="NONE"
    Author=False
    Organization="";
    #for line in fileinput.input(FileName):
    fin=open(FileName)
    for line in fin:
        if Author:
            m=re.search(r'<cal:authorAffiliation organizationid="(\w+)" connection=""/>',line)
            m2=re.search(r'<cal:authorAffiliation connection="" organizationid="(\w+)"/>',line)
            if m:
                #print m.group(1)
                Organization=m.group(1)
                break
            if m2:
                Organization=m2.group(1)
                break            
            pass
        #if line.find(author)!=-1: print line
        if line.find("<foaf:name>"+author+"</foaf:name>")!=-1 or line.find("<foaf:name>"+author+" </foaf:name>")!=-1:
            print "found author",line
            Author=True
            pass
        pass
    fin.close()
    print Organization
    isOrg=False
    listaOrg=[]
    fin=open(FileName)
    for line in fin:
        if isOrg:
            m=re.search(r'<foaf:name>(.*)</foaf:name>',line)
            m3=re.search(r'.*\,(.*)</foaf:name>',line)
            if m:
                #print m.group(1)
                Org=m.group(1)
                listaOrg=Org.split(",")
                Country=listaOrg[-1]
                break
            if m3:
                #print m3.group(1)
                Org=m3.group(1)
                listaOrg=Org.split(",")
                Country=listaOrg[-1]
                break
            pass
        if line.find("<foaf:Organization id=\""+Organization+"\">")!=-1:
            isOrg=True
            pass
        pass
    fin.close()
    pass
########## MAIN ##############################   
filename = sys.argv[1]
author="Claudio Gatti"
FindAuthorCountry(author,filename)
print "Country ",Country
author="Sandro Palestini"
FindAuthorCountry(author,filename)
print "Country ",Country


    
    
