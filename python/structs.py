from ctypes import *

class vec3(Structure):
	pass
vec3._fields_ = [
	("x", c_int),
	("y", c_int),
	("z", c_int)
]

class block_t(Structure):
	pass
block_t._fields_ = [
	("blockid", c_int8),
	("data",    c_int8)
]
block_p = POINTER(block_t)

class schematic(Structure):
	pass
schematic._fields_ = [
	("size", vec3),
	("blocks", block_p)
]
schem_p = POINTER(schematic)

class file_t(Structure):
	pass
file_p = POINTER(file_t)

class nbt_t(Structure):
	pass
nbt_p = POINTER(nbt_t)
