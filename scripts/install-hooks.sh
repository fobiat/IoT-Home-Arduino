#!/usr/bin/env bash
set -euo pipefail

# Install git hooks for this repository (sets core.hooksPath to .githooks)
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
echo "Installing git hooks for repo at $REPO_ROOT"
git -C "$REPO_ROOT" config core.hooksPath .githooks
chmod +x "$REPO_ROOT"/.githooks/pre-commit
echo "Hooks installed. To enable for other checkouts, run this script in that checkout."
