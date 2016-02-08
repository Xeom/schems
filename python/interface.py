from share  import lib
from ctypes import *

from structs import *
import helpers
import schematic_functs, vec_functs

class Rotate(Transform):
	def __init__(self, x, y, z):
		self.x, self.y, self.z = x, y, z

class CutieSchem:
	def __init__(self):
		self.schem = None

	def loadfile(self, path):
		f=helpers.open_file_p(path, "r")
		self.schem = lib.nbt_load_schem(f)
		helpers.close_file_p(f)

	def __del__(self):
		self.freeschem()

	def freeschem(self):
		lib.schem_kill(self.schem)
		self.schem = None

	def copy(self):
		newschem = schem_copy(self.schem)
		ret = CutieSchem()
		ret.schem = newschem

		return ret

	def rotate(self, x=0, y=0, z=0):
		newschem = lib.schem_rotate(self.schem, vec3(x, y, z))
		self.freeschem()
		self.schem = newschem

	def flip(self, x=0, y=0, z=0):
		newschem = lib.schem_flip(self.schem, vec3(x, y, z))
		self.freeschem()
		self.schem = newschem

	def resize(self, x=0, y=0, z=0, stacking=False):
		if stacking:
			newschem = lib.schem_stackng_resize(self.schem, vec3(x, y, z))
		else:
			newschem = lib.schem_resize(self.schem, vec3(x, y, z))

		self.freeschem()
		self.schem = newschem

	def stack(self, x=0, y=0, z=0):
		newschem = lib.schem_stack(self.schem, vec3(x, y, z))
		self.freeschem()
		self.schem = newschem

	def fill(self, x, y, z, xsize, ysize, zsize, blockid, data=0):
		offset = vec3(x,     y,     z    )
		size   = vec3(xsize, ysize, zsize)

		block  = block_t(blockid, data)

		lib.schem_fill(self.schem, offset, size, block)

	def insert(self, x, y, z, other):
		offset = vec3(x, y, z)

		if isinstance(other, CutieSchem):
			other = other.schem

		schem_insert(self.schem, offset, other)

	def shift(self, x, y, z):
		offset = vec3(x, y, z)

		newschem = lib.schem_shift(self.schem, offset)
		self.freeschem()
		self.schem = newschem
