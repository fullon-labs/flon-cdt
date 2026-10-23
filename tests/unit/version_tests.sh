#!/bin/bash
set -eo pipefail
# Ensure every published wrapper and the compiler's predefined macros use the
# version configured by the top-level build.
echo '##### CDT Version Consistency Test #####'
# orient ourselves
[[ -z "$BUILD_ROOT" ]] && export BUILD_ROOT="$(pwd)"
echo "Using BUILD_ROOT=\"$BUILD_ROOT\"."
# test expectations
if [[ -z "$VERSION" ]]; then
    if [[ -n "$1" ]]; then
        export VERSION="$1"
    else
        export VERSION="$(echo "${BUILDKITE_TAG:-$GIT_TAG}" | sed "s/^v//")"
    fi
fi
if [[ -z "$VERSION" ]]; then
    echo "Missing version input."
    exit 1
fi

for TOOL in cdt-cpp cdt-cc cdt-ld cdt-abidiff cdt-init; do
    EXPECTED="${TOOL} version ${VERSION}"
    ACTUAL=$("$BUILD_ROOT/bin/$TOOL" --version)
    if [[ "$EXPECTED" != "$ACTUAL" ]]; then
        echo "Failed: \"$EXPECTED\" != \"$ACTUAL\""
        exit 1
    fi
    echo "Passed with \"$ACTUAL\"."
done

IFS=. read -r VERSION_MAJOR VERSION_MINOR VERSION_PATCH_WITH_SUFFIX <<<"$VERSION"
VERSION_PATCH="${VERSION_PATCH_WITH_SUFFIX%%-*}"
MACROS=$("$BUILD_ROOT/bin/cdt-cpp" -dM -E -x c++ /dev/null)
for EXPECTED in \
    "#define __eosio_cdt_major__ ${VERSION_MAJOR}" \
    "#define __eosio_cdt_minor__ ${VERSION_MINOR}" \
    "#define __eosio_cdt_patchlevel__ ${VERSION_PATCH}"; do
    if ! grep -Fxq "$EXPECTED" <<<"$MACROS"; then
        echo "Failed: missing or incorrect compiler macro \"$EXPECTED\""
        exit 1
    fi
done

echo "All CDT version checks passed."
