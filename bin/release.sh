#
# Define your project and the dependencies for homebrew
# DEPENDENCIES will be evaled later!
# Then run this as
#   ./bin/release.sh --publisher mulle-nat --publisher-tap mulle-kybernetik/alpha/
#

PROJECT="MyProject"      # your project name, requires camel-case
DESC="MyProject does this and that"
# DEPENDENCIES='${DEPENDENCY_TAP}mulle-concurrent
# libz'
LANGUAGE=c               # c,cpp, objc used for finding the version header file
HOMEBREW_PARENT_PATH=".."

#######
# Ideally you don't hafta change anything past this line
#######


#
# source mulle-homebrew.sh (clumsily)
#
# it 's expected that release.sh is in $(PROJECT_DIR)/bin
# and mulle-homebrew is also there
#
MULLE_BOOTSTRAP_FAIL_PREFIX="`basename -- $0`"
DIR="`dirname -- "$0"`"
. ${DIR}/mulle-homebrew/mulle-homebrew.sh || exit 1
cd "${DIR}/.."

# parse options
homebrew_parse_options "$@"

#
# dial past options now as they have been parsed
#
while [ $# -ne 0 ]
do
   case "$1" in
      -*)
         shift
      ;;

      --*)
         shift
         shift
      ;;

      *)
         break;
      ;;
   esac
done

#
# require PUBLISHER and PUBLISHER_TAP as command line parameters, so
# that forks don't have to edit this constantly
#
if [ -z "${PUBLISHER}" ]
then
   fail "you need to specify a publisher with --publisher (hint: https://github.com/<publisher>)"
fi

if [ -z "${PUBLISHER_TAP}" ]
then
   fail "you need to specify a publisher tap with --publisher-tap (hint: <mulle-kybernetik/software/>)"
fi

DEPENDENCY_TAP="${DEPENDENCY_TAP:-${PUBLISHER_TAP}}"
BOOTSTRAP_TAP="${DEPENDENCY_TAP:-mulle-kybernetik/software/}"
HOMEBREW_PATH="${HOMEBREW_PATH:-..}"
BRANCH="{BRANCH:-release}"

#
# these can usually be deduced, if you follow the conventions
#
if [ -z "${NAME}" ]
then
   NAME="`get_name_from_project "${PROJECT}" "${LANGUAGE}"`"
fi

if [ -z "${VERSIONFILE}" ]
then
   VERSIONFILE="`get_header_from_name "${NAME}"`"
fi

if [ -z "${VERSIONNAME}" ]
then
   VERSIONNAME="`get_versionname_from_project "${PROJECT}"`"
fi

if [ -f VERSION ]
then
   VERSION="`head -1 VERSION`"
else
   VERSION="`get_project_version "${VERSIONFILE}" "${VERSIONNAME}"`"
fi

[ -z "${VERSION}" ] && fail "No version found"


# --- HOMEBREW TAP ---
# Specify to where and under what name to publish via your brew tap
#
RBFILE="${NAME}.rb"                    # ruby file for brew
HOMEBREWTAP="${HOMEBREW_PARENT_PATH}/homebrew-`basename -- ${PUBLISHER_TAP}`"


# --- GIT ---
# tag to tag your release
# and the origin where
TAG="${TAG:-${TAGPREFIX}${VERSION}}"


main()
{
   git_main "${BRANCH}" "${ORIGIN}" "${TAG}" || exit 1
   homebrew_main
}

main "$@"
