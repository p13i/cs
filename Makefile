SHELL=/usr/bin/bash
# Always run the site rule regardless of whether make thinks the action can be skipped
.PHONY: site

default: build test site serve

ci: clean setup lint build test site

build:
	bazel build --config=debug --jobs=1 --local_cpu_resources=1 --local_ram_resources=1 -- //...

test:
	bazel test --test_output=errors -- //...

setup:
	# Install Bazel
	sudo apt install apt-transport-https curl gnupg -y
	curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg
	sudo mv bazel-archive-keyring.gpg /usr/share/keyrings
	echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
	sudo apt update -y
	sudo apt install bazel -y
	# Install Buildifier
	sudo apt-get install golang-go -y
	go install github.com/bazelbuild/buildtools/buildifier@latest
	# Install clang-format
	sudo apt install clang-format -y
	# Install GDB
	sudo apt-get install libc6-dbg gdb valgrind -y

lint:
	go run github.com/bazelbuild/buildtools/buildifier@latest -r .
	find . -iname *.h -o -iname *.hh -o -iname *.cc | xargs clang-format -i

sync:
	git add .
	git commit --allow-empty -m "[make sync]"
	git pull --rebase
	git push

site:
	rm -rf site
	mkdir -p site
	cp bazel-bin/cs/app/index.js/index.{js,wasm} site
	cp cs/app/{index.html,wasm.js} site

serve:
	python3 -m http.server --directory site

clean:
	bazel clean
	rm -rf site

http:
	fuser -k 8080/tcp || echo "No-op"
	bazel run //cs/app:main

web-app: http

test-json:
	bazel test --test_output=errors --test_arg=--gtest_filter=ParseArrayTest.ArrayWithOneElement -- //cs/net/json:parsers_test

probe:
	bazel run //cs/app:prober

watch:
	watch -n 1 "make build"

watch-probe:
	watch -n 10 "make probe"

db:
	bazel run //cs/db:main

pull:
	git pull --rebase --all