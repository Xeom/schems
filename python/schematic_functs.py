from structs import *
from share   import lib
from ctypes  import *

lib.schem_init.argtypes = [vec3]
lib.schem_init.restype  = schem_p

lib.schem_copy.argtypes = [schem_p]
lib.schem_copy.restype  = schem_p

lib.schem_copy_blocks.argtypes = [schem_p, block_p]
lib.schem_copy_blocks.restype  = None

lib.schem_kill.argtypes = [schem_p]
lib.schem_kill.restype  = None

lib.schem_size.argtypes = [schem_p]
lib.schem_size.restype  = vec3
lib.schem_vol.argtypes  = [schem_p]
lib.schem_vol.restype   = c_int

lib.schem_get.argtypes  = [schem_p, vec3]
lib.schem_get.restype   = block_t
lib.schem_set.argtypes  = [schem_p, vec3, block_t]
lib.schem_set.restype   = None

lib.schem_get_index.argtypes  = [schem_p, c_size_t]
lib.schem_get_index.restype   = block_t
lib.schem_set_index.argtypes  = [schem_p, c_size_t, block_t]
lib.schem_set_index.restype   = None

lib.schem_resize.argtypes = [schem_p, vec3]
lib.schem_resize.restype  = schem_p
lib.schem_stack.argtypes  = [schem_p, vec3]
lib.schem_stack.restype   = schem_p
lib.schem_stacking_resize.argtypes  = [schem_p, vec3]
lib.schem_stacking_resize.restype   = schem_p

lib.schem_shift.argtypes = [schem_p, vec3]
lib.schem_shift.restype  = schem_p
lib.schem_flip.argtypes = [schem_p, vec3]
lib.schem_flip.restype  = schem_p
lib.schem_rotate.argtypes = [schem_p, vec3]
lib.schem_rotate.restype  = schem_p

lib.schem_fill.argtypes = [schem_p, vec3, vec3, block_t]
lib.schem_fill.restype  = None

lib.schem_insert.argtypes = [schem_p, vec3, schem_p]
lib.schem_insert.restype  = None
