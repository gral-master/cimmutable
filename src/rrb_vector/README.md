
RRB vectors
===========

## RRB vectors and RRB trees
Relaxed Radix Balanced trees are used as an extension of immutable Vectors
in functional languages. The immutable vector is based on balanced wide trees,
called RB-trees (Radix Balanced trees).

Bagwell and Rompf allow the structure to be partially filled. The RB tree has m-branches that
are fixed and filled, while the RRB tree uses relaxing technique on  the fixed
branching factor m.

At first, the structure has only one level, when the first level is filled with
32 elements, a new level is created. The new level has the possibility to pointto
32 different vectors, making the structure max size 32^n, where n is the number of levels.
When elemented are inserted the structure is filled without leaving any space, but
when the structure is splitted of merged, there is no need for changing the location of elements.


## Articles
-[2011 technical report, Bagwell and Rompf](https://infoscience.epfl.ch/record/169879/files/RMTrees.pdf)
	here is the [git repo](https://github.com/TiarkRompf/rrbtrees)
-[2015 ICFP's article Stucki et al.](https://github.com/nicolasstucki/scala-rrb-vector)
- [Nicolas Stucki's thesis] (https://github.com/nicolasstucki/scala-rrb-vector) "Turning Relaxed Radix Balanced Vector from Theory to Practice for Scala Collections"


## Functions
-create
-size
-update
-lookup
-push
-pop
-split
-merge
-unref
-dump

##Tests
TODO
