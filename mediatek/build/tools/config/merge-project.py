import re, os, sys
pattern = [
  re.compile("^([^=\s]+)\s*=\s*(.+)$"),
  re.compile("^([^=\s]+)\s*=$"),
  re.compile("\s*#")
]

config = {}
for line in [re.sub(r"[\r\n]","",x) for x in sys.stdin.readlines()]:
    result = (filter(lambda x:x,[x.search(line) for x in pattern]) or [None])[0]
    if not result: continue
    name,value = None,None
    if len(result.groups())==0: continue
    name = result.group(1)
    try:
       value = result.group(2)
    except IndexError:
       value = ""
    config[name] = value.strip()

for item in sorted(config.keys()):
    print "%s = %s"%(item,config[item])

