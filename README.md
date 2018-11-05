# Simple Library Management (SLM)

SLM is a program to **manage data of resources and members** for library.

# Building SLM

* It is simple. Just type `make`.

```
make
```

# Usage

* Type ./lib with resource and input on command line.

```
./lib <resource file> <input file>
```

# Code

* Library.h
	* mem_add		: Add new member to undergraduates map.
	* check_1 ~ 7	: Check each fail case.
	* borrowRes		: Borrow a resource.
	* returnRes		: Return a resource.

* member.h
	* addInfo		: Add resource information which the member borrow.
	* eraseInfo		: Erase resource which the member return.
	* isExist		: Check whether the resource is in vector.
	* isOver		: Check whether the limit of member is over.
	* isRestricted	: Check whether the member is restricted or not.

* resource.h
	* isOccupied	: Check whether the resource is occupied.
	* isLate		: check whether the return date is over.

---------------

