from ctypes  import *
from structs import file_p
from share import lib
import os

class NullPointerException(Exception):
	pass

str2char = lambda c:c_char_p(bytes(c, "ascii"))

lib.fopen.argtypes = [c_char_p, c_char_p]
lib.fopen.restype = file_p
def open_file_p(path, mode):
	path = os.path.abspath(path)

	if not os.path.isfile(path):
		raise IOError("File not found '{}'".format(path))

	fptr = lib.fopen(str2char(path), str2char(mode))

	if not fptr:
		raise NullPointerException("Could not open file '{}'".format(path))

	return fptr

def close_file_p(f):
	if f:
		lib.fclose(f)

	else:
		raise NullPointerException("File pointer to close was NULL")
