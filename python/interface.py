from share  import lib
from ctypes import *

from structs import *
import helpers
import schematic_functs, vec_functs, schemfile_functs

class CutieSchem:
	def __init__(self):
		self._schem = None

	def loadfile(self, path):
		fptr = helpers.open_file_p(path, "r")

		self.schem = lib.file_load_schem(fptr)
		helpers.close_file_p(fptr)

	def savefile(self, path):
		fptr = helpers.open_file_p(path, "w+")

		lib.file_save_schem(fptr, self.schem)
		helpers.close_file_p(fptr)

	def freeschem(self):
		if self._schem:
			lib.schem_kill(self.schem)

		print("HI")
		self._schem = None

	def __del__(self):
		self.freeschem()

	def get_schem(self):
		if self._schem:
			return self._schem

		else:
			raise helpers.NullPointerException("Schematic not loaded")

	def set_schem(self, value):
		if self._schem:
			self.freeschem()

		self._schem = value

	def del_schem(self):
		self.freeschem()

	schem = property(get_schem, set_schem, del_schem)

	def copy(self):
		newschem = schem_copy(self.schem)
		ret = CutieSchem()
		ret.schem = newschem

		return ret

	def rotate(self, x=0, y=0, z=0):
		self.schem = lib.schem_rotate(self.schem, vec3(x, y, z))

	def flip(self, x=0, y=0, z=0):
		self.schem = lib.schem_flip(self.schem, vec3(x, y, z))

	def resize(self, x=0, y=0, z=0, stacking=False):
		if stacking:
			self.schem = lib.schem_stackng_resize(self.schem, vec3(x, y, z))
		else:
			self.schem = lib.schem_resize(self.schem, vec3(x, y, z))

	def stack(self, x=0, y=0, z=0):
		self.schem = lib.schem_stack(self.schem, vec3(x, y, z))

	def fill(self, x, y, z, xsize, ysize, zsize, blockid, data=0):
		offset = vec3(x,     y,     z    )
		size   = vec3(xsize, ysize, zsize)

		block  = block_t(blockid, data)

		lib.schem_fill(self.schem, offset, size, block)

	def insert(self, x, y, z, other):
		offset = vec3(x, y, z)

		if isinstance(other, CutieSchem):
			other = other.schem

		lib.schem_insert(self.schem, offset, other)

	def shift(self, x, y, z):
		self.schem = lib.schem_shift(self.schem, vec3(x, y, z))

s=CutieSchem()
s.loadfile("../test.schematic")
s.loadfile("../out.schematic")
s.rotate(x=1)
