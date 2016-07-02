*****************************************************************
*								*
*	HALLOWEEN DEV TOOLS					*
*	How to make your own Halloween object & map files	*
*								*
*	Copyright (C) 1998-2003 By Jadeware			*
*	ALL RIGHT RESERVED.					*
*								*
*****************************************************************



How to build a new 3D scene file ?
===================================

Use 3DS Studio MAX (tm) or GMAX (tm) to build a map file.
Export the scene into a ASE ascii map export file.

Note: illegal geometry may be buggy ingame. Study the sample map geometry.



How to calculate a halloween map file ?
=========================================

ase.exe *.ASE (ascii map export)
bsp.exe -v *.pts
vis.exe *.map
rad.exe *.map

Put the *.map file into the hgame /system/maps subdirectoty.

Note : Use HTC.exe for better GUI command access.



How to calculate a halloween object file ?
===========================================

obj.exe *.ASE (ascii object export)
Put the *.ent into the hgame /system/entity subdirectory.



How to launch my map file ?
============================

Launch Halloween game then type 'map yourmapfile' in the console to laod your mapfile.

