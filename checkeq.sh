source testlib.sh

function test_eq
{
	for map in xyz xzy zxy zyx yzx yxz; do
		echo $1:$2 | tr xyz $map
	done \
		| sort | uniq | test_eq_stdin
}

function test_eq_single
{
	echo $1:$2 \
		| test_eq_stdin
}

function test_eq_stdin
{
	errors=0

	while read -r flags; do
		flags1=$(echo $flags | cut -d: -f1)
		flags2=$(echo $flags | cut -d: -f2)

		hash1=$(cat test.schematic | run_test_cmd "./test $flags1" | shasum)
		hash2=$(cat test.schematic | run_test_cmd "./test $flags2" | shasum)

		if [[ $hash1 != $hash2 ]]; then
			err_print $(cmd_print $flags1) did not produce the same results as $(cmd_print $flags2)
			errors=1
		fi
	done

	return $errors
}

make -q || err_print "Binary not up-to-date"

echo
info_print "Starting tests!"
echo 

test_eq "--rotate x1 --rotate x1 --rotate x1" \
		"--rotate x3" &&                      \
	info_print "Multiple rotations in one axis passed"

test_eq "--flip x1y1"    \
		"--rotate z2" && \
	info_print "Flip equivilence to rotation passed"

test_eq "--flip x1y1z1 --rotate x4y4z4 --flip x1y1z1" \
		"" &&                                         \
	info_print "360° rotation and double flipping passed"

test_eq "--rotate z2"      \
		"--rotate x2y2" && \
		info_print "Rotating 180° on different axes passed"

test_eq_single "--rotate z1"                              \
			   "--rotate x1 --rotate y-1 --rotate x-1" && \
	info_print "Rotating 90° on different axes passes"

test_eq "--flip y1 --stack y2"    \
		"--stack y2 --flip y1" && \
	info_print "Flipping and stacking commutativity passed"

test_eq "--flip x1 --rotate y2 --flip z1"    \
		""                                && \
	info_print "Flip-rotate-flip equivilence to identity passed"

test_eq_single "--rotate x1y1z1"                        \
			   "--rotate x1 --rotate y1 --rotate z1" && \
	info_print "Execution order of rotations passed"

test_eq_single "--rotate x1y2z3"                        \
			   "--rotate x1 --rotate y2 --rotate z3" && \
	info_print "Execution order of rotations double-passed"

test_eq "--rotate x3"     \
		"--rotate x-1" && \
	info_print "-90° Rotations equivilence to 270° rotations passed"
