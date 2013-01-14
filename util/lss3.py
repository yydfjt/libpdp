#! /usr/bin/python

import boto
import os
from boto.s3.connection import OrdinaryCallingFormat

def sizeof_fmt(num):
	for x in ['b ','KB','MB','GB','TB', 'XB']:
		if num < 1024.0:
			return "%3.1f %s" % (num, x)
		num /= 1024.0
	return "%3.1f %s" % (num, x)

def list_bucket(b, prefix=None):
	"""List everything in a bucket"""
	from boto.s3.prefix import Prefix
	from boto.s3.key import Key
	total = 0
	query = b
	if prefix:
		if not prefix.endswith("/"):
			prefix = prefix + "/"
		query = b.list(prefix=prefix, delimiter="/")
		print "%s" % prefix
	num = 0
	for k in query:
		num += 1
		mode = "-rwx---"
		if isinstance(k, Prefix):
			mode = "drwxr--"
			size = 0
		else:
			size = k.size
			for g in k.get_acl().acl.grants:
				if g.id == None:
					if g.permission == "READ":
						mode = "-rwxr--"
					elif g.permission == "FULL_CONTROL":
						mode = "-rwxrwx"
		if isinstance(k, Key):
		   print "%s\t%s\t%010s\t%s" % (mode, k.last_modified,
				 sizeof_fmt(size), k.name)
		else:
		   #If it's not a Key object, it doesn't have a last_modified time, so
		   #print nothing instead
		   print "%s\t%s\t%010s\t%s" % (mode, ' '*24,
			  sizeof_fmt(size), k.name)
		total += size
	print "="*80
	print "\t\tTOTAL: \t%010s \t%i Files" % (sizeof_fmt(total), num)

def list_buckets(s3):
	"""List all the buckets"""
	for b in s3.get_all_buckets():
		print b.name

if __name__ == "__main__":
	import sys


	Id = os.environ['S3_ACCESS_KEY_ID']
	Key = os.environ['S3_SECRET_ACCESS_KEY']
	Host, Port = os.environ['S3_HOSTNAME'].split(':')
	Port = int(Port)

	pairs = []
	mixedCase = False
	for name in sys.argv[1:]:
			if "/" in name:
				pairs.append(name.split("/",1))
			else:
				pairs.append([name, None])
			if pairs[-1][0].lower() != pairs[-1][0]:
				mixedCase = True
	
	s3 = boto.connect_s3(
		Id,
		Key,
		host=Host,
		port=Port,
		is_secure=False,
		calling_format=OrdinaryCallingFormat())

	if len(sys.argv) < 2:
		list_buckets(s3)
	else:
		for name, prefix in pairs:
			list_bucket(s3.get_bucket(name), prefix)
