#!/bin/bash

# URL del ZIP de recursos (con dl=1)
URL="https://www.dropbox.com/scl/fo/yjhhdbypfd4sskygl49gz/AAHFQXeRvnQh3QuYG8iGpZ8?rlkey=xyt992ex4xdpc703uo56ds2ue&st=7qfksi19&dl=1"

# Carpeta destino
DEST="resources"

# Borrar recursos antiguos si existen
echo "Eliminando carpeta $DEST antigua..."
rm -rf "$DEST"

# Descargar ZIP
echo "Descargando recursos..."
wget -O recursos.zip "$URL"

# Descomprimir
echo "Descomprimiendo recursos..."
unzip recursos.zip -d "$DEST"

# Limpiar
rm recursos.zip

echo "¡Recursos listos!"
