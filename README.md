# Simple Library Management (SLM)

SLM is a program to **manage data of resources and members** for library.
**Space Management** is added in new version (v 1.1).

# Building SLM

* It is simple. Just type `make`.

```
make
```

# Usage

* ~~Type ./lib with resource and input on command line.~~
* Type ./lib with resource, input and space file on command line.

```
./lib <resource file> <input file> <space file>
```

# Code

* Library.h
	* mem_add		: Add new member to undergraduates map.
	* check_1 ~ 7	: Check each fail case. (for resource)
	* check 8 ~ 14	: Check each fail case. (for space)(v 1.1)
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
	* isLate		: Check whether the return date is over.

* space.h (v 1.1)
	* borrowRoom	: Save some information of a borrower, time and during. The room status is changed to "OCCUPIED".
	* ReturnRoom	: Change status of studyroom from "OCCUPIED" to "FREE".
	* empty			: Change seat status to "EMPTY". Also, save when the user leave the seat.
	* comback		: Change seat status to "OCCUPIED".
	* getEtime		: Return empty time.
---------------

