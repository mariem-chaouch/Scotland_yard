# PLT

[![Actions Status](https://github.com/cbares/plt/workflows/PLT%20build/badge.svg)](https://github.com/cbares/plt/actions)

## Dépendances:

```consle
sudo apt install libboost-all-dev
```

## Couverture des tests

Sur Linux, avec GCC, `lcov`, `gcovr` et `xvfb` installés, lancez depuis la racine du projet

```console
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_CODE_COVERAGE=ON
xvfb-run -a cmake --build build --target code-coverage
xvfb-run -a cmake --build build --target code-coverage-gcov
```

Le rapport HTML `build/code-coverage/index.html` et le rapport XML
`build/code-coverage-gcov.xml` indiquent la couverture des lignes et des
branches. Le HTML permet de repérer les branches manquantes dans chaque fichier.

## Mise à jour depuis le template:

* Si votre dépôt ne dépend pas du template:
```console
git remote add template https://github.com/cbares/plt
```

* Ensuite:
```console
git fetch --all
```

* puis récupérer les fichiers importants: 
```console
git checkout template/master CMakeLists.txt .github/workflows/main.yml .gitignore cmake extern/dia2code
```
