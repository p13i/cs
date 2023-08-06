default:
	echo ""

test:
	bazel test --test_output=errors ...

lint:
	go run github.com/bazelbuild/buildtools/buildifier@latest -r .
	sudo apt install clang-format
	chmod +x ./scripts/clang_format.sh && ./scripts/clang_format.sh