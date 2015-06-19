import os
import sys
import difflib
import glob

a = glob.glob(sys.argv[1] + "/*.in")
for i in a:
	j = "{}".format(i)[:-3] + ".out"
	j_new = j.replace("tests/", "tests/my_")
	ret = os.system("./chess.exe {} > ".format(i) + j_new)
	diff = os.system("diff " + j + " " + j_new)
	if diff == 0:
		print i + " passed"
	else:
		print i + " not passed:"
		print os.system("diff " + j + " " + j_new)

