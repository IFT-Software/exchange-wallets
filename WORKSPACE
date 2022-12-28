load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_googletest",
    sha256 = "199e68f9dff997b30d420bf23cd9a0d3f66bfee4460e2cd95084a2c45ee00f1a",
    strip_prefix = "googletest-5376968f6948923e2411081fd9372e71a59d8e77",
    urls = ["https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip"],
)

http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.9.0/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

load("//openssl:openssl_repositories.bzl", "openssl_repositories")
openssl_repositories()

load("//openssl:openssl_setup.bzl", "openssl_setup")
openssl_setup()

http_archive(
    name = "libsecp256k1",
    sha256 = "6ece280c0e6ea9d861051077c28a25b7f48800c43a4098a800b7d3b0c124e406",
    build_file = "//libsecp256k1:BUILD.libsecp256k1.bazel",
    strip_prefix = "secp256k1-0.2.0",
    url = "https://github.com/bitcoin-core/secp256k1/archive/refs/tags/v0.2.0.zip"
)
