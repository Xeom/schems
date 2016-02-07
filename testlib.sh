function err_print
{
	echo -e "\e[31m [\e[91m!\e[31m]\e[0m" $* 1>&2
}

function info_print
{
	echo -e "\e[32m [\e[92mi\e[32m]\e[0m" $* 1>&2
}

function cmd_print
{
	echo -e "\e[33m[" $* "]\e[0m"
}

function run_test_cmd
{
	$* 2>/dev/null
	errcode=$?
	if   [[ $errcode == 139 ]]; then
		err_print Running $(cmd_print $*) Caused a segfault
	elif [[ $errcode !=   0 ]]; then
		err_print Running $(cmd_print $*) Returned error code $errcode
	fi
}
