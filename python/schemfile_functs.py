from structs import *
from share   import lib

lib.nbt_save_schem.argtypes = [nbt_p, schem_p]
lib.nbt_save_schem.restype  = None
lib.nbt_load_schem.argtypes = [nbt_p]
lib.nbt_load_schem.restype  = schem_p

lib.file_save_schem.argtypes = [file_p, schem_p]
lib.file_save_schem.restype  = None
lib.file_load_schem.argtypes = [file_p]
lib.file_load_schem.restype  = schem_p

