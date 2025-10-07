#!/usr/bin/env bash
set -e
set -o pipefail

echo "Updating all submodules to their upstream versions..."

# Fetch and merge updates in all submodules
git submodule update --remote --merge --init --recursive

# Collect commit info from each submodule
changes=()
while IFS= read -r subpath; do
    [ -z "$subpath" ] && continue

    pushd "$subpath" > /dev/null
    sha=$(git rev-parse --short HEAD)
    msg=$(git log -1 --pretty=format:"%s")
    changes+=("$subpath@$sha: $msg")
    popd > /dev/null
done < <(git submodule foreach --quiet 'echo $sm_path')

# Check if there are actual changes to commit
if ! git diff-index --quiet HEAD -- || [ -n "$(git ls-files --others --exclude-standard)" ]; then
    has_changes=true
else
    has_changes=false
fi

# Exit if no updates
if [ "$has_changes" = false ]; then
    echo "No updates were applied."
    exit 0
fi

# Stage all submodule pointer updates
for info in "${changes[@]}"; do
    subdir="${info%%@*}"
    git add "$subdir"
done

# Commit with detailed message
echo "Committing submodule updates..."
commit_msg="Update dependencies:"
for info in "${changes[@]}"; do
    commit_msg="$commit_msg"$'\n'"  $info"
done

git commit -m "$commit_msg"

echo ""
echo "✓ Done! Submodule updates committed."
echo ""
echo "Next steps:"
echo "  git push              # Push superproject changes"
echo ""
echo "Optional (if you have write access to submodules):"
echo "  git submodule foreach --recursive 'git push origin HEAD'"