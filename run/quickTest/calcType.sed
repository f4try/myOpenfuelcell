# (overwrites original):
# usage (overwrite)   sed -i -f calcType.sed files

# (saves backup "<file>.bak"):
# usage (backup)   sed -i.bak -f calcType.sed <file>

/calculated/,/}/  s/value   /gradient/;  s/calculated/fixedGradient/

## s/value           uniform 0;/gradient        uniform 0;/
## s/calculated/fixedGradient/
