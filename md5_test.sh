#!/usr/bin/env bash

if [[ -z "$(diff <(echo test string|./ft_ssl md5) <(echo test string|openssl md5))" ]]; then
	echo $? good
else
	echo $? bad
fi
if [[ -z "$(diff <(echo test string|./ft_ssl md5 -p) <(echo test string|md5 -p))" ]]; then
	echo $? good
else
	echo $? bad
fi
if [[ -z "$(diff <(echo test string|./ft_ssl md5 -q -r) <(echo test string|md5 -q -r))" ]]; then
	echo $? good
else
	echo $? bad
fi
echo test file > inp
if [[ -z "$(diff <(echo test string|./ft_ssl md5 -r inp) <(echo test string|openssl md5 -r inp))" ]]; then
	echo $? good
else
	echo $? bad
fi
if [[ -z "$(diff <(echo test string|./ft_ssl md5 -r -p -s "foo" inp -s "bar") <(echo test string|md5 -r -p -s "foo" inp -s "bar"))" ]]; then
	echo $? good
else
	echo $? bad
fi
if [[ -z "$(diff <((echo test string;echo test string)|./ft_ssl md5 -r -q -p -s "foo" inp) <((echo test string;echo test string)|md5 -r -q -p -s "foo" inp))" ]]; then
	echo $? good
else
	echo $? bad
fi
