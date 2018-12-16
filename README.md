# Simple Library Management (SLM)

SLM is a program to **manage data of resources and members** for library. (v 1.0)

**Space Management** is added in new version. (v 1.1)

**Memeber type** is added. (v 1.2)



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
	* check 15 ~ 16	: Check each fail case. (for resource)(v 1.2)
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

# Statistics shell

Library_statistics.sh is a shell that executes LSM with extracted inputs and puts the result into a suitable path automatically.

* Shell usage
	* Type "./library_statistics.sh" with some parameters.
	* The first, second and third parameter is "all" by default.
	* resource.dat, input.dat and space.dat must be in data directory to use this shell.
```
./library_statistics.sh <param1> <param2> <param3> <param4>
```
* Parameters

param1 | param2 | param3 | param4 
------ | ------ | ------ | ------
resource | book | | |
resource | e-book | | |
resource | magazine | | |
resource | all | | |
input | date | from(yy/mm/dd) | to(yy/mm/dd) |
input | book | | |
input | e-book | | |
input | magazine | | |
input | undergraduate | | |
input | graduate | | |
input | faculty | | |
input | all(without date.dat) | | |
space | date | from(yy/mm/dd/hh) | to(yy/mm/dd/hh) |
space | studyromm | room number(1 ~ 10, all) | |
space | seat | floor(1 ~ 3, all) | |
space | undergraduate | | |
space | graduate | | |
space | faculty | | |
space | all(without date.dat) | | |
ouput | stat_table | | |

---------------
