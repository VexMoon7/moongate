#!/usr/bin/env bash
set -euo pipefail

say() { printf "%b\n" "$*"; }
hdr() { printf "\n\033[1m%s\033[0m\n" "$*"; }

in_repo() { git rev-parse --is-inside-work-tree >/dev/null 2>&1; }

require_repo() { in_repo || { say "Not inside a Git repo."; exit 1; }; }

has() { command -v "$1" >/dev/null 2>&1; }

require_repo

hdr "🔎 Repo Audit"
default_branch="$(git symbolic-ref --short HEAD 2>/dev/null || echo '?')"
origin_url="$(git remote get-url origin 2>/dev/null || echo 'no origin')"
say "Current branch: $default_branch"
say "Origin: $origin_url"

hdr "🌿 Branching Signals"
pull_rebase="$(git config --get pull.rebase || echo 'unset')"
autosetuprebase="$(git config --get branch.autosetuprebase || echo 'unset')"
rebase_autostash="$(git config --get rebase.autoStash || echo 'unset')"
if git show-ref --verify --quiet refs/heads/develop; then has_develop="yes"; else has_develop="no"; fi
say "pull.rebase: $pull_rebase | branch.autosetuprebase: $autosetuprebase | rebase.autoStash: $rebase_autostash"
say "develop branch present: $has_develop"

hdr "🧰 Hooks"
hooks_path="$(git config --get core.hooksPath || echo ".git/hooks")"
active_hooks="$(ls -1 "$hooks_path" 2>/dev/null | grep -v sample || true)"
say "hooks path: $hooks_path"
say "active hooks:"
[ -n "$active_hooks" ] && printf "%s\n" "$active_hooks" | sed 's/^/  - /' || say "  (none)"

hdr "📦 Submodules"
if [ -f .gitmodules ]; then
  say "Submodules detected:"
  git config -f .gitmodules --get-regexp path | sed 's/^/  - /'
else
  say "(none)"
fi

hdr "🖼️ LFS & Attributes"
if git check-attr -a -- . >/dev/null 2>&1; then
  git check-attr -a -- . | head -n 20 | sed 's/^/  /'
else
  say "(no attributes found or Git <2.11)"
fi
if has git-lfs && git lfs env >/dev/null 2>&1; then
  say "Git LFS is installed and configured."
else
  say "Git LFS not detected."
fi

hdr "🪶 Ignore Rules"
git check-ignore -v -- **/* 2>/dev/null | head -n 10 | sed 's/^/  /' || say "(no matches)"

hdr "🧩 Sparse/Worktrees"
sparse="$(git config --get core.sparseCheckout || echo 'disabled')"
say "sparseCheckout: $sparse"
say "worktrees:"
git worktree list 2>/dev/null | sed 's/^/  /' || say "  (none)"

hdr "🏷️ Tags & Versioning"
git tag -ln | head -n 10 | sed 's/^/  /' || say "  (no tags)"
say "describe: $(git describe --tags --always --dirty 2>/dev/null || echo 'n/a')"

hdr "🏗️ Monorepo Hints"
hint="no"
[ -d packages ] && hint="yes"
[ -d apps ] && hint="yes"
[ -f pnpm-workspace.yaml ] && hint="yes"
[ -f turbo.json ] && hint="yes"
[ -f nx.json ] && hint="yes"
say "monorepo hints: $hint"

hdr "🤖 CI/CD"
if [ -d .github/workflows ]; then
  say "GitHub Actions workflows:"
  ls -1 .github/workflows | sed 's/^/  - /'
elif [ -f .gitlab-ci.yml ]; then
  say "GitLab CI config: .gitlab-ci.yml"
else
  say "(no CI config detected)"
fi

hdr "📝 Commit Conventions"
if ls -1 commitlint.config.* .commitlintrc* 2>/dev/null | grep -q .; then
  say "commitlint present; use Conventional Commits."
else
  say "(no commitlint detected)"
fi
if [ -d .husky ]; then
  say "Husky hooks detected (Node projects)."
fi

hdr "✅ Suggested Moves"
if [ "$pull_rebase" = "true" ] || [ "$autosetuprebase" = "always" ]; then
  say "• Use linear history: 'git pull --rebase', 'git rebase origin/$default_branch'"
fi
if [ "$has_develop" = "yes" ]; then
  say "• GitFlow style: feature/* → merge --no-ff into develop; releases/hotfixes tagged on main."
fi
if [ -f .gitmodules ]; then
  say "• Submodules: 'git submodule update --init --recursive' before building; commit submodule SHA bumps."
fi
if git check-attr -a -- . 2>/dev/null | grep -q 'filter: lfs'; then
  say "• LFS: install 'git lfs', track binaries, avoid large files outside LFS."
fi
if [ "$hint" = "yes" ]; then
  say "• Monorepo: use 'git sparse-checkout set <paths>' and consider 'git worktree add' per app/package."
fi
if [ -d .github/workflows ] || [ -f .gitlab-ci.yml ]; then
  say "• Use topic branches + PR/MR; let CI pass before merging."
fi

hdr "📚 Cheat-sheet by situation"
say "• Sync: 'git fetch --prune' + 'git rebase origin/$default_branch'"
say "• New work: 'git checkout -b <type>/<topic>' with Conventional Commits"
say "• Fixup tidy: 'git commit --fixup <sha>' + 'git rebase -i --autosquash'"
say "• Bisect bugs: 'git bisect start BAD GOOD' → test → 'git bisect good|bad'"
say "• Release: 'git tag -a vX.Y.Z -m ...' + push tags