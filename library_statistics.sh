#!/bin/bash

echo -e "script start\n"

RPATH=./data/resource.dat
IPATH=./data/input.dat
SPATH=./data/space.dat
ROOT=./result

is_exist(){
	if [ ! -e "$1" ]
	then
		echo "\"$1\" does not exist."
		echo -e "\nscript end" 
		exit
	fi
}

resource_data(){
	if [ ! -e "$RPATH" ]
	then
		echo "\"resource.dat\" dose not exist."
	else
		var=${1^}
		cat $RPATH | awk -v var="$var" '{if($1 == var){print}}' > $1.dat.tmp
	fi
}

exe_lib(){
	./lib $1 $2 $3
	rm $1 $2 $3
	mv output.dat $4
}

rst_lib(){
	cat $RPATH > resource.dat.tmp

	if [ "$2" == "input" ]
	then
		sed -n '1p' $SPATH > space.dat.tmp
	elif [ "$2" == "space" ]
	then
		sed -n '1p' $IPATH > input.dat.tmp
	fi

	if [ ! -e "resource.dat.tmp" ] || [ ! -e "input.dat.tmp" ] || [ ! -e "space.dat.tmp" ]
	then
		echo -e "\tfailed"
	else
		exe_lib resource.dat.tmp input.dat.tmp space.dat.tmp $ROOT/$2/$1.dat
		echo -e "\tdone"
	fi
}

rst_data(){
	if [ "$1" == "studyroom" ]
	then
		var=StudyRoom
	else
		var=${1^}
	fi

	echo -n "$3/$1.dat"
	printf "%s\n" "${ori[0]}" > $3.dat.tmp

	for i in $(seq ${#a[*]})
	do
		b=(${a[$i - 1]})
		if [ ${b[$2]} == "$var" ]
		then
			printf "%s\n" "${ori[$i]}" >> $3.dat.tmp
		fi
	done

	rst_lib $1 $3
}

space_data(){
	if [ "$1" == "studyroom" ]
	then
		var=StudyRoom
	else
		var=${1^}
	fi

	echo -n "space/$1.dat"
	printf "%s\n" "${ori[0]}" > space.dat.tmp

	for i in $(seq ${#a[*]})
	do
		b=(${a[$i - 1]})
		if [ ${b[1]} == "$var" ] && [ ${b[2]} == "$2" ]
		then
			printf "%s\n" "${ori[$i]}" >> space.dat.tmp
		fi
	done

	rst_lib $1 space
}

is_exist $RPATH
is_exist $IPATH
is_exist $SPATH

if [ -z "$1" ]
then
	set  -- "all"
fi

if [ -z "$2" ]
then
	set -- "$1" "all"
fi

if [ -z "$3" ]
then
	set -- "$1" "$2" "all"
fi

if [ ! -e "lib" ]
then
	echo -e "------------------------------- compile start -------------------------------"
	is_exist Makefile
	make
	echo -e "------------------------------ compile complete -----------------------------\n"
fi

if [ ! -d "$ROOT" ]
then
	mkdir "$ROOT"
	mkdir "$ROOT/resource"
	mkdir "$ROOT/input"
	mkdir "$ROOT/space"
	mkdir "$ROOT/output"
fi

if [ "$1" == "resource" ] || [ "$1" == "all" ]
then
	if [ "$2" == "book" ] || [ "$2" == "all" ]
	then
		echo -n "resource/book.dat"
		resource_data book
		if [ -e "book.dat.tmp" ]
		then
			mv book.dat.tmp $ROOT/resource/book.dat
			echo -e "\tdone"
		else
			echo -e "\tfailed"
		fi
	fi

	if [ "$2" == "e-book" ] || [ "$2" == "all" ]
	then
		echo -n "resource/e-book.dat"
		resource_data e-book
		if [ -e "e-book.dat.tmp" ]
		then
			mv e-book.dat.tmp $ROOT/resource/e-book.dat
			echo -e "\tdone"
		else
			echo -e "\tfailed"
		fi
	fi
	
	if [ "$2" == "magazine" ] || [ "$2" == "all" ]
	then
		echo -n "resource/magazine.dat"
		resource_data magazine
		if [ -e "magazine.dat.tmp" ]
		then
			mv magazine.dat.tmp $ROOT/resource/magazine.dat
			echo -e "\tdone"
		else
			echo -e "\tfailed"
		fi
	fi
fi

if [ "$1" == "input" ] || [ "$1" == "all" ]
then
	TMP=$IFS
	IFS=$'\n' 
	ori=($(cat $IPATH))
	a=($(sed -n '2,$p' $IPATH))
	IFS=$TMP

	if [ "$2" == "date" ]
	then
		if [ -z "$3" ]
		then
			echo "Type 3rd parameter: from(yy/mm/dd)." 
			exit
		else
			if [ -z "$4" ]
			then
				echo "Type 4th parameter: to(yy/mm/dd)." 
				exit
			else
				echo -n "input/date.dat"

				printf "%s\n" "${ori[0]}" > input.dat.tmp

				for i in $(seq ${#a[*]})
				do
					b=(${a[$i - 1]})
					if [ ${b[0]} \> "$3" ] && [ ${b[0]} \< "$4" ]
					then
						printf "%s\n" "${ori[$i]}" >> input.dat.tmp
					elif [ ${b[0]} == "$3" ] || [ ${b[0]} == "$4" ]
					then
						printf "%s\n" "${ori[$i]}" >> input.dat.tmp
					fi
				done
				rst_lib date input
			fi
		fi
	fi

	if [ "$2" == "book" ] || [ "$2" == "all" ]
	then
		rst_data book 1 input
	fi	

	if [ "$2" == "e-book" ] || [ "$2" == "all" ]
	then
		rst_data e-book 1 input
	fi	

	if [ "$2" == "magazine" ] || [ "$2" == "all" ]
	then
		rst_data magazine 1 input
	fi	

	if [ "$2" == "undergraduate" ] || [ "$2" == "all" ]
	then
		rst_data undergraduate 4 input
	fi	

	if [ "$2" == "graduate" ] || [ "$2" == "all" ]
	then
		rst_data graduate 4 input
	fi	

	if [ "$2" == "faculty" ] || [ "$2" == "all" ]
	then
		rst_data faculty 4 input
	fi	
fi

if [ "$1" == "space" ] || [ "$1" == "all" ]
then
	TMP=$IFS
	IFS=$'\n' 
	ori=($(cat $SPATH))
	a=($(sed -n '2,$p' $SPATH | sed 's/\([0-9]\{2\}\)//'))
	IFS=$TMP

	if [ "$2" == "date" ]
	then
		if [ -z "$3" ]
		then
			echo "Type 3rd parameter: from(yy/mm/dd/hh)." 
			exit
		else
			if [ -z "$4" ]
			then
				echo "Type 4th parameter: to(yy/mm/dd/hh)." 
				exit
			else
				echo -n "space/date.dat"

				printf "%s\n" "${ori[0]}" > space.dat.tmp

				for i in $(seq ${#a[*]})
				do
					b=(${a[$i - 1]})
					if [ ${b[0]} \> "$3" ] && [ ${b[0]} \< "$4" ]
					then
						printf "%s\n" "${ori[$i]}" >> space.dat.tmp
					elif [ ${b[0]} == "$3" ] || [ ${b[0]} == "$4" ]
					then
						printf "%s\n" "${ori[$i]}" >> space.dat.tmp
					fi
				done
				rst_lib date space
			fi
		fi
	fi

	if [ "$2" == "studyroom" ] || [ "$2" == "all" ]
	then
		if [ "$3" == "all" ]
		then
			rst_data studyroom 1 space
		else
			space_data studyroom $3
		fi
	fi	

	if [ "$2" == "seat" ] || [ "$2" == "all" ]
	then
		if [ "$3" == "all" ]
		then
			rst_data seat 1 space
		else
			space_data seat $3
		fi
	fi	

	if [ "$2" == "undergraduate" ] || [ "$2" == "all" ]
	then
		rst_data undergraduate 4 space
	fi	

	if [ "$2" == "graduate" ] || [ "$2" == "all" ]
	then
		rst_data graduate 4 space
	fi	

	if [ "$2" == "faculty" ] || [ "$2" == "all" ]
	then
		rst_data faculty 4 space
	fi	
fi

if [ "$1" == "output" ] || [ "$1" == "all" ]
then
	if [ "$2" == "stat_table" ] || [ "$2" == "all" ]
	then
		echo -n "output.dat"
		./lib $RPATH $IPATH $SPATH
		if [ ! -e "output.dat" ]
		then
			echo -e "\tfailed"
		else
			echo -e "\tdone"

			echo -n "output/stat_table.dat"

			echo -e "Return_code\tNumber" > $ROOT/output/stat_table.dat

			TMP=$IFS
			IFS=$'\n' 
			a=($(sed -n '2,$p' output.dat))
			IFS=$TMP

			cnt=(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)

			for i in $(seq ${#a[*]})
			do
				b=(${a[$i - 1]})
				idx=$(($((${b[1]} + 18)) % 18))
				cnt[$idx]=$((${cnt[$idx]} + 1))
			done

			echo -e "-1\t${cnt[17]}" >> $ROOT/output/stat_table.dat

			for j in ${!cnt[*]}
			do
				if [ $j -ne 17 ] 
				then
					echo -e "$j\t${cnt[$j]}" >> $ROOT/output/stat_table.dat
				fi
			done

			echo -e "\tdone"
		fi
	fi
fi

echo -e "\nscript end" 
