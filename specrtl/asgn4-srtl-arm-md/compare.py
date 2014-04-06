#! /usr/bin/python
import sys
import re
import string
#p=open('log_and_rel/log_and_rel_gen.md','r')
#q=open('log_and_rel/log_and_rel.md','r')
if len(sys.argv) < 3:
	print('Usage: ' + sys.argv[0] + ' <orig_md> <gen_md>')
	quit(-1)
p=open(sys.argv[1],'r')
q=open(sys.argv[2],'r')
log_orig_data=p.read()
log_orig=log_orig_data.split('(define_')
log_read_data=q.read()
log_gen= log_read_data.split('(define_')




j=1
flag1=1
while j<min(len(log_gen),len(log_orig)):
	string1=re.sub(r'\s', '', log_orig[j])	
	string1 = string1.replace ("\q","")
	string2=re.sub(r'\s', '', log_gen[j])	
	string2 = string2.replace ("\q","")
	if(cmp(string1,string2)!=0):
		print re.sub(r'\s', '', log_orig[j])	
		print re.sub(r'\s', '', log_gen[j])
		flag1=0
	j=j+1
if(flag1==0):
	print "log_fail"

if(flag1==1):
		print "complete_success"
#print x[1].translate(trantab)
#print y[1].translate(trantab)


	#print "hurray"
	#i=x
#x=string.find(read_data[i:],'(define_')
#print x
#print read_data
#pattern=re.search('(\(define\_\(define\_',read_data)

