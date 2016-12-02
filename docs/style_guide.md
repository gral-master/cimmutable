
Style guide
===========

Namespacing
-----------

API prefix:

```
imc_<name>
````

Naming
------

 - function names: `snake_case`
 
 - typedef names: suffix `<name>_t`
 
Spacing
-------

  - no tab characters
  
  - indent 4 spaces
  
Indent style
------------

  - linux style

```c
if (<cond>) {
    <body>
}
```

Same for function defs, loops, etc. 

Misc
----

 - separation between names and types

Example:

```c
int* toto = ....
```

Memory management
-----------------

 - safer allocator ?

 - no collector e.g. Boehm 

Documentation
-------------

Doxygen comments (doxygen style)

Line width
----------

  - 80 columns (with exceptions, e.g. error messages)
  
Contracts
---------

 - invariant checking for structures

 - preconditions/postconditions for functions

