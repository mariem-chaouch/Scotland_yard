# PLT

[![Actions Status](https://github.com/cbares/plt/workflows/PLT%20build/badge.svg)](https://github.com/cbares/plt/actions)

## Dépendances:

```consle
sudo apt install libboost-all-dev
```

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
