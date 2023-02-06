"""A module defining the third party dependency libssh2"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def libidn2_repositories():
    maybe(
        http_archive,
        name = "libidn2",
        urls = [
            "https://gitlab.com/libidn/libidn2/uploads/54f6d239283bbdc75d4dbd732ced04e4/libidn2-2.3.4.tar.gz"
        ],
        type = "tar.gz",
        sha256 = "93caba72b4e051d1f8d4f5a076ab63c99b77faee019b72b9783b267986dbb45f",
        strip_prefix = "libidn2-2.3.4",
        build_file = Label("//third_party/libidn2:BUILD.libidn2.bazel"),
    )