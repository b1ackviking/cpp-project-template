module.exports = {
  "branches": "main",
  "tagFormat": "${version}",
  "plugins": [
    "@semantic-release/commit-analyzer",
    "@semantic-release/release-notes-generator", "@semantic-release/changelog",
    [
      "@semantic-release/git", {
        "message":
            "chore(release): ${lastRelease.version} → ${nextRelease.version} [skip ci]"
      }
    ]
  ]
};
