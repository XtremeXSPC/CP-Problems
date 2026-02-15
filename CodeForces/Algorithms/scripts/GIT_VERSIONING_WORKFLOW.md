# Template Versioning with Git (Replacement for `version_manager.py`)

`CodeForces/Algorithms/` is already tracked by Git, so template/module versioning
is handled with native Git commands.

## Create a Snapshot

```bash
git add CodeForces/Algorithms/templates CodeForces/Algorithms/modules
git commit -m "Update templates/modules"
git tag -a templates/v1.2 -m "Template snapshot v1.2"
```

## List Snapshots

```bash
git tag -l 'templates/*' --sort=-creatordate
```

## Compare Snapshots

```bash
git diff templates/v1.1..templates/v1.2 -- CodeForces/Algorithms/templates CodeForces/Algorithms/modules
```

## Restore from a Snapshot

```bash
git checkout templates/v1.1 -- CodeForces/Algorithms/templates CodeForces/Algorithms/modules
```

## Export a Snapshot

```bash
git archive --format=zip --output templates-v1.2.zip templates/v1.2 CodeForces/Algorithms/templates CodeForces/Algorithms/modules
```
