default:
	echo ""

build:
	bazel build ...

test:
	bazel test --test_output=all ...

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

build-wasm:
	emcc cs/app/hello_world.cc -o cs/app/server/index.html

setup-wasm:
	mkdir -p ext
	# Remove existing ext/emsdk folder
	sudo rm -rf ext/emsdk
	# Setup WebAssembly with EMScripten
	git clone https://github.com/emscripten-core/emsdk.git ext/emsdk
	sudo ext/emsdk/emsdk install latest
	sudo ext/emsdk/emsdk activate latest
	sudo sh ext/emsdk/emsdk_env.sh
	sudo apt-get install python3 -y
	sudo apt-get install cmake -y
	emcc -v

lint:
	go run github.com/bazelbuild/buildtools/buildifier@latest -r .
	find . -iname *.h -o -iname *.cc | xargs clang-format -i

serve:
	bazel run //cs/app/server:server
