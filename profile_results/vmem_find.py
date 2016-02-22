import sys;
import re;


if len(sys.argv) != 2:
	print "Usage: python {0} <filename>".format(sys.argv[0])
	exit(1)

with open(sys.argv[1], "r") as log:
	begin_pattern = re.compile(".*?BEGIN$")
	end_pattern = re.compile(".*?END$")
	main_pattern = re.compile(".*?\(main\.cpp\:[0-9]+\)$")

	started = False
	found_main = False
	ended = False
	content = ""
	main_error_count = 0

	for line in log:
		if begin_pattern.match(line):
			started = True
		elif started and main_pattern.match(line):
			found_main = True
		elif end_pattern.match(line):
			ended = True

		if started:
			content += line

		if ended:
			if started and found_main:
				print content
				main_error_count += 1
			started = False
			found_main = False
			ended = False
			content = ""

	print "\n\nErrors = {0}".format(main_error_count)
