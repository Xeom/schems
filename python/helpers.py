from share   import std
from ctypes  import *
from structs import file_p

clib = CDLL("libc.so.6")

str2char = lambda c:c_char_p(bytes(c, "ascii"))

clib.fopen.argtypes = [c_char_p, c_char_p]
clib.fopen.restype = file_p
def open_file_p(path, mode):
	return clib.fopen(str2char(path), str2char(mode))

def free_file_p(f):
	clib.fclose(f)
