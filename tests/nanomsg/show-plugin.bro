# @TEST-EXEC: bro -NN Thermite::Nanomsg |sed -e 's/version.*)/version)/g' >output
# @TEST-EXEC: btest-diff output
