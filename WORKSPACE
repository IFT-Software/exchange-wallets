load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "bazel_skylib",
  sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
  urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
)

http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

load("//third_party/openssl:openssl_repositories.bzl", "openssl_repositories")
openssl_repositories()

load("//third_party/openssl:openssl_setup.bzl", "openssl_setup")
openssl_setup()

load("//third_party/zlib:zlib_repositories.bzl", "zlib_repositories")
zlib_repositories()

load("//third_party/libidn2:libidn2_repositories.bzl", "libidn2_repositories")
libidn2_repositories()

load("//third_party/libssh2:libssh2_repositories.bzl", "libssh2_repositories")
libssh2_repositories()

load("//third_party/curl:curl_repositories.bzl", "curl_repositories")
curl_repositories()

http_archive(
  name = "com_google_absl",
  sha256 = "54707f411cb62a26a776dad5fd60829098c181700edcd022ea5c2ca49e9b7ef1",
  strip_prefix = "abseil-cpp-20220623.1",
  urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20220623.1.zip"],
)

http_archive(
    name = "com_google_googletest",
    sha256 = "199e68f9dff997b30d420bf23cd9a0d3f66bfee4460e2cd95084a2c45ee00f1a",
    strip_prefix = "googletest-5376968f6948923e2411081fd9372e71a59d8e77",
    urls = ["https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip"],
)

http_archive(
    name = "libsecp256k1",
    sha256 = "6ece280c0e6ea9d861051077c28a25b7f48800c43a4098a800b7d3b0c124e406",
    build_file = "//third_party:BUILD.libsecp256k1.bazel",
    strip_prefix = "secp256k1-0.2.0",
    url = "https://github.com/bitcoin-core/secp256k1/archive/refs/tags/v0.2.0.zip"
)

http_archive(
    name = "libzmq",
    sha256 = "622bf650f7dab6de098c84d491052ad5a6d3e57550cd09cc259e0ab24ec83ec3",
    build_file = "//third_party:BUILD.libzmq.bazel",
    strip_prefix = "zeromq-4.3.4",
    url = "https://github.com/zeromq/libzmq/releases/download/v4.3.4/zeromq-4.3.4.zip"
)

http_archive(
    name = "libgmp",
    sha256 = "fd4829912cddd12f84181c3451cc752be224643e87fac497b69edddadc49b4f2",
    build_file = "//third_party:BUILD.libgmp.bazel",
    strip_prefix = "gmp-6.2.1",
    url = "https://gmplib.org/download/gmp/gmp-6.2.1.tar.xz"
)

http_archive(
    name = "libpqxx",
    sha256 = "6a1aba33b08448795c621bcb36949158eb4a7c8db2946be4701700b6210ec985",
    build_file = "//third_party:BUILD.libpqxx.bazel",
    strip_prefix = "libpqxx-7.7.4",
    url = "https://github.com/jtv/libpqxx/archive/refs/tags/7.7.4.zip"
)

http_archive(
    name = "com_github_nelhage_rules_boost",
    sha256 = "5ea00abc70cdf396a23fb53201db19ebce2837d28887a08544429d27783309ed",
    url = "https://github.com/nelhage/rules_boost/archive/96e9b631f104b43a53c21c87b01ac538ad6f3b48.tar.gz",
    strip_prefix = "rules_boost-96e9b631f104b43a53c21c87b01ac538ad6f3b48",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()
