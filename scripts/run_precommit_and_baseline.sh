#!/usr/bin/env bash
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
echo "Installing pre-commit and detect-secrets (user install)..."
python3 -m pip install --user pre-commit detect-secrets
echo "Running pre-commit across repository (may produce warnings)..."
python3 -m pre_commit run --all-files || true
echo "Generating detect-secrets baseline (.secrets.baseline)..."
python3 -m detect_secrets.scan --update "$REPO_ROOT/.secrets.baseline" || true
cd "$REPO_ROOT"
if [ -f .secrets.baseline ]; then
  git add .secrets.baseline || true
  git commit -m "chore(secrets): add detect-secrets baseline" || true
  git push origin main || true
  echo ".secrets.baseline committed (if new)"
else
  echo "No .secrets.baseline produced"
fi

echo "Done"
