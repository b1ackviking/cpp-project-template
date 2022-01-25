module.exports = {
  "branches": "main",
  "tagFormat": "${version}",
  "plugins": [
    [
      "@semantic-release/commit-analyzer", {
        "preset": "conventionalcommits",
        "releaseRules": [{"type": "refactor", "release": "patch"}]
      }
    ],
    [
      "@semantic-release/release-notes-generator", {
        "preset": "conventionalcommits",
        "presetConfig": {
          "types": [
            {"type": "feat", "section": "Features"},
            {"type": "fix", "section": "Bug Fixes"},
            {"type": "refactor", "section": "Refactoring"}
          ]
        }
      }
    ],
    "@semantic-release/changelog",
    [
      "@semantic-release/git", {
        "message":
            "chore(release): ${lastRelease.version} → ${nextRelease.version} [skip ci]"
      }
    ]
  ]
};
