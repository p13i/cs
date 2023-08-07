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
	sudo apt update
	sudo apt install bazel
	# Install Buildifier
	sudo apt-get install golang-go
	go install github.com/bazelbuild/buildtools/buildifier@latest
	# Install clang-format
	sudo apt install clang-format

lint:
	go run github.com/bazelbuild/buildtools/buildifier@latest -r .
	find . -iname *.h -o -iname *.cc | xargs clang-format -i
