from ctypes  import *
from share   import lib
from structs import vec3

lib.vec3_init.argtypes = [c_int, c_int, c_int]
lib.vec3_init.restype  = vec3
lib.vec3_trip.argtypes = [c_int]
lib.vec3_trip.restype  = vec3

lib.vec3_add.argtypes = [vec3, vec3]
lib.vec3_add.restype  = vec3
lib.vec3_sub.argtypes = [vec3, vec3]
lib.vec3_sub.restype  = vec3
lib.vec3_mul.argtypes = [vec3, vec3]
lib.vec3_mul.restype  = vec3
lib.vec3_div.argtypes = [vec3, vec3]
lib.vec3_div.restype  = vec3
lib.vec3_mod.argtypes = [vec3, vec3]
lib.vec3_mod.restype  = vec3

lib.vec3_min.argtypes = [vec3, vec3]
lib.vec3_min.restype  = vec3
lib.vec3_max.argtypes = [vec3, vec3]
lib.vec3_max.restype  = vec3

lib.vec3_abs.argtypes = [vec3]
lib.vec3_abs.restype  = vec3
lib.vec3_neg.argtypes = [vec3]
lib.vec3_neg.restype  = vec3

lib.vec3_scale.argtypes = [vec3, c_int]
lib.vec3_scale.restype  = vec3

lib.vec3_sqr.argtypes = [vec3]
lib.vec3_sqr.restype  = c_int
lib.vec3_dot.argtypes = [vec3, vec3]
lib.vec3_dot.restype  = c_int
lib.vec3_vol.argtypes = [vec3]
lib.vec3_vol.restype  = c_int

lib.vec3_gt.argtypes  = [vec3, vec3]
lib.vec3_gt.restype   = c_int
lib.vec3_lt.argtypes  = [vec3, vec3]
lib.vec3_lt.restype   = c_int
