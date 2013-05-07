print "site_init"

def build_common_include(target, source, env):
  os.popen('output/host/tools/GenerateSineTables > $TARGET')
  os.popen('perl ' + topdir + 'tools/Text2Event.pl $SOURCE >$TARGET')
  os.popen('perl ' + topdir + 'tools/Text2GCE.pl $SOURCE > $TARGET')
  os.popen('perl ' + topdir + 'tools/Text2NE.pl $SOURCE > $TARGET')
  os.popen('perl ' + topdir + 'tools/xci2cpp.pl $SOURCE > $TARGET')
  os.popen('perl ' + topdir + 'tools/xcs2cpp.pl $SOURCE > $TARGET')
  os.popen('touch ' + topdir + 'output/include/dirstamp')
  return None

prebuild = Builder(action = build_common_include)
