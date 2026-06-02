CC := "gcc"

tests   := "$(find tests -name \"*.c\")"
headers := "$(find include/btrstd -name \"*.h\")"
xargs   := "$(xargs < compile_flags.txt)"

build-a:
	mkdir -p build
	gcc {{ xargs }} src/_btrstd.c -c -o build/btrstd.a

build-single-file:
	mkdir -p build/dist
	for header in {{ headers }}; do name="${header##*/}"; ./build_single_file.py $header "src/${name%.h}.c" "build/dist/${name}"; done

test: build-a
	mkdir -p build/tests
	for test in {{ tests }}; do gcc {{ xargs }} -fsanitize=address build/btrstd.a $test -o build/$test; echo "TESTING $test"; build/$test; done
