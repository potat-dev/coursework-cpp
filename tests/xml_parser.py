# script to pretty print xml files in json format

import xmltodict
import pprint

with open('bench.xml') as fd:
    doc = xmltodict.parse(fd.read())

doc = doc['Catch2TestRun']['TestCase']['BenchmarkResults']
pprint.pprint(doc)