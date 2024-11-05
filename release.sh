#!/bin/bash

# Be sure to install git-semver
# (git clone https://github.com/markchalloner/git-semver.git && sudo git-semver/install.sh)

# Check if git-semver is installed
if ! command -v git-semver &> /dev/null
then
    echo "git-semver is not installed. Please install it first."
    exit 1
fi

echo "Select release type:"
echo "1) Patch"
echo "2) Minor"
echo "3) Major"
read -p "Enter your choice (1-3): " choice

case $choice in
  1) target="patch" ;;
  2) target="minor" ;;
  3) target="major" ;;
  *) echo "Invalid choice"; exit 1 ;;
esac

new_version=$(git-semver -target $target)
echo "New version will be: $new_version"

read -p "Proceed with tagging? (y/n): " confirm
if [[ $confirm == [yY] || $confirm == [yY][eE][sS] ]]; then
  git tag -a "v$new_version" -m "Release $new_version"
  git push origin "v$new_version"
  echo "Tagged and pushed v$new_version"
else
  echo "Aborted"
fi