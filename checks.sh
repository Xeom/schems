function err_print
{
	echo -e "\e[31m [\e[91m!\e[31m]\e[0m" $* 1>&2
}

function cmd_print
{
	echo -e "\e[33m[" $* "]\e[0m"
}

function run_test_cmd
{
	$* 2>/dev/null
	errcode=$?
	if [[ $errcode == 139 ]]; then
		err_print Running $(cmd_print $*) Caused a segfault
	elif [[ $errcode != 0 ]]; then
		err_print Running $(cmd_print $*) Returned error code $errcode
	fi
}

function test_eq
{
	ops1=$1
	ops2=$2

	for map in xyz xzy zxy zyx yzx yxz; do
		echo $ops1:$ops2 | tr xyz $map
	done | sort | uniq | \

		while read -r flags; do
			flags1=$(echo $flags | cut -d: -f1)
			flags2=$(echo $flags | cut -d: -f2)

			hash1=$(cat test.schematic | run_test_cmd "./test $flags1" | shasum)
			hash2=$(cat test.schematic | run_test_cmd "./test $flags2" | shasum)

			if [[ $hash1 != $hash2 ]]; then
				err_print $(cmd_print $flags1) did not produce the same results as $(cmd_print $flags2)
			fi
		done
}

make -q || err_print "Binary not up-to-date"

test_eq "--rotate x1 --rotate x1 --rotate x1" \
		"--rotate x3"

test_eq "--rotate x3"  \
		"--rotate x-1"

test_eq "--rotate x2y2z2"  \
		"--rotate x2 --rotate y2 --rotate z2"

test_eq "--flip x1y1z1 --rotate x4y4z4 --flip x1y1z1" \
		""

test_eq "--flip y1x1" \
		"--rotate z2"

test_eq "--flip x1y1" \
		"--rotate x2y2"

test_eq "--flip y1 --stack y2" \
		"--stack y2 --flip y1"

